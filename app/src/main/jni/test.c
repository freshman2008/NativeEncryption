#include "com_gd_deepsec_nativeencyption_NdkJNIUtils.h"

JNIEXPORT jstring JNICALL Java_com_gd_deepsec_nativeencyption_NdkJNIUtils_hello
  (JNIEnv *env, jobject instance)
{
    return (*env)->NewStringUTF(env, "helloJNI");
}