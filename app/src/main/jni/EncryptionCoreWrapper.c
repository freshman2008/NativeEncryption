#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <android/log.h>

/**
 * c++层 native函数
 */
JNIEXPORT jstring JNICALL sayHello(JNIEnv *env, jobject clazz) {
    return (*env)->NewStringUTF(env, "Hello, I'm native c++ function : sayHello()!");
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