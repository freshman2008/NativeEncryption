#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <android/log.h>
#include <openssl/aes.h>
#include <malloc.h>

const char iv[AES_BLOCK_SIZE] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 * c++层 native函数
 */
JNIEXPORT jstring JNICALL sayHello(JNIEnv *env, jobject clazz) {
    char *in = "hello";
    char *key = "1122334455667788";
    char enc[1024] = {0x00};
    int ret = aes_encrypt(in, key, enc, strlen(in), iv);

    char dec[1024] = {0x00};
    ret = aes_decrypt(enc, key, dec, strlen(enc), iv);
    return (*env)->NewStringUTF(env, "Hello, I'm native c++ function : sayHello()!");
}

JNIEXPORT jbyteArray JNICALL aesEncrypt(JNIEnv *env, jobject clazz, jbyteArray data, jbyteArray key, jint len) {
    char *pData = (*env)->GetByteArrayElements(env, data, NULL);
    char *pKey = (*env)->GetByteArrayElements(env, key, NULL);
    char out[1024] = {0x00};

    int ret = aes_encrypt(pData, pKey, out, len, iv);
    int outLen = strlen(out);
    jbyteArray array = (*env)->NewByteArray(env, outLen);
    (*env)->SetByteArrayRegion(env, array, 0, outLen, out);

    //3. 释放内存
    (*env)->ReleaseByteArrayElements(env, data, pData, 0);

    return array;
}

JNIEXPORT jbyteArray JNICALL aesDecrypt(JNIEnv *env, jobject clazz, jbyteArray data, jbyteArray key, jint len) {
    char *pData = (*env)->GetByteArrayElements(env, data, NULL);
    char *pKey = (*env)->GetByteArrayElements(env, key, NULL);
    char out[1024] = {0x00};

    int ret = aes_decrypt(pData, pKey, out, len, iv);
    int outLen = strlen(out);
    jbyteArray array = (*env)->NewByteArray(env, outLen);
    (*env)->SetByteArrayRegion(env, array, 0, outLen, out);

    //3. 释放内存
    (*env)->ReleaseByteArrayElements(env, data, pData, 0);
    return array;
}

int aes_encrypt(char* in, char* key, char* out,int len, char *iv)
{
    if(!in || !key || !out) {
        return 0;
    }

    //此处需要把iv赋值给本地ivec，因为调用AES_cbc_encrypt时ivec参与计算，ivec的值会被改变，如果是const的，会导致异常
    char ivec[AES_BLOCK_SIZE];
    if (iv != NULL) {
        memcpy(ivec, iv, AES_BLOCK_SIZE);
    } else {
        memset(ivec, 0, AES_BLOCK_SIZE);
    }

    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)key, AES_BLOCK_SIZE*8, &aes) < 0) {
        return 0;
    }

    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, ivec, AES_ENCRYPT);
    return 1;
}

int aes_decrypt(char* in, char* key, char* out,int len, char *iv)
{
    if(!in || !key || !out) {
        return 0;
    }

    //此处需要把iv赋值给本地ivec，因为调用AES_cbc_encrypt时ivec参与计算，ivec的值会被改变，如果是const的，会导致异常
    char ivec[AES_BLOCK_SIZE];
    if (iv != NULL) {
        memcpy(ivec, iv, AES_BLOCK_SIZE);
    } else {
        memset(ivec, 0, AES_BLOCK_SIZE);
    }

    AES_KEY aes;
    if(AES_set_decrypt_key((unsigned char*)key, AES_BLOCK_SIZE*8, &aes) < 0) {
        return 0;
    }

    char iiv[16];
    for (int i=0; i<16; i++) {
        iiv[i] = '\0';
    }

    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, ivec, AES_DECRYPT);
    return 1;
}

/**
 * Java和JNI函数的绑定表
 * JNINativeMethod由三部分组成,可添加多组对应:
 * (1)Java中的函数名;
 * (2)函数签名,格式为(输入参数类型)返回值类型;
 *    ()Ljava/lang/String; ()表示无参，Ljava/lang/String;表示返回String，在对象类名（包括包名，‘/’间隔）前面加L，分号结尾
 * (3)native函数指针
 */
static JNINativeMethod native_methods_table[] = {
        { "helloFromJNI", "()Ljava/lang/String;", (void*) sayHello },
        { "aesEncrypt", "([B[BI)[B", (void*) aesEncrypt },
        { "aesDecrypt", "([B[BI)[B", (void*) aesDecrypt },
};

/**
 * 注册native方法到java中
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods) {
    jclass clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static const int JNI_METHODS_NUM = sizeof(native_methods_table)
                                   / sizeof(native_methods_table[0]);

/**
 * System.loadLibrary过程中会自动调用JNI_OnLoad，在此进行动态注册
 */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }

    /**
     * Java class name
     * consist of packageName and className
     * For com.example.testjni.MainActivity, classname is com/example/testjni/MainActivity
     */
    char classname[] = "com/gd/deepsec/nativeencyption/NdkJNIUtils"; //Java class name, consist of packageName.className
    if (registerNativeMethods(env, classname, native_methods_table, JNI_METHODS_NUM)
        == JNI_FALSE) {
        __android_log_print(ANDROID_LOG_ERROR, "JNITag",
                            "JNI: failed to register methods for %s", classname);
        return -1;
    }

    // 返回jni的版本
    return JNI_VERSION_1_6;
}