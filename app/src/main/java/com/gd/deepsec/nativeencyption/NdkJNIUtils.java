package com.gd.deepsec.nativeencyption;

public class NdkJNIUtils {
    private static final NdkJNIUtils instance = new NdkJNIUtils();

    static {
        System.loadLibrary("HelloJNI");//之前在build.gradle里面设置的so名字，必须一致
    }

    public static NdkJNIUtils getInstance() {
        return instance;
    }

    public native String helloFromJNI();
}
