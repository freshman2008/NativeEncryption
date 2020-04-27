package com.gd.deepsec.nativeencyption;

import java.nio.ByteBuffer;

public class ByteArrayUtils {
    public static String byteArrayToString(byte[] a) {
        StringBuffer stringBuffer = new StringBuffer();
        for (int i = 0; i < a.length; i++) {
            String oneByte = null;
            oneByte = Integer.toHexString(a[i]);
            if (oneByte.length() == 1)
                //onebyte = "0" + onebyte;
                stringBuffer.append("0").append(oneByte);
            else
                //onebyte = onebyte.substring(onebyte.length() - 2);
                stringBuffer.append(oneByte.substring(oneByte.length() - 2));

            //result = result + onebyte.toUpperCase();
        }
        return stringBuffer.toString();
    }

    public static byte[] stringToByteArray(String s) {
        if (s == null || s == "") {
            return null;
        }
        java.util.Vector<Integer> v = new java.util.Vector<Integer>();
        String operate = new String(s);
        operate = operate.replaceAll(" ", "");
        operate = operate.replaceAll("\t", "");
        operate = operate.replaceAll("\n", "");
        if (operate.endsWith(";"))
            operate = operate.substring(0, operate.length() - 1);
        if (operate.length() % 2 != 0)
            return null;
        int num = 0;

        ByteBuffer byteBuffer = ByteBuffer.allocate(operate.length() / 2);

        StringBuffer stringBuffer = new StringBuffer();
        stringBuffer.append(operate);
        //stringBuffer.substring(0, 2);
        //stringBuffer.delete(0, 2);
        while (stringBuffer.length() > 0) {
            byteBuffer.put(new Integer(Integer.parseInt(stringBuffer.substring(0, 2), 16)).byteValue());
            stringBuffer.delete(0, 2);
        }
        return byteBuffer.array();

//        while (operate.length() > 0) {
//            try {
//                num = Integer.parseInt(operate.substring(0, 2), 16);
//            } catch (NumberFormatException nfe) {
//                return null;
//            }
//            v.add(new Integer(num));
//            operate = operate.substring(2);
//        }
//        byte[] result = new byte[v.size()];
//        java.util.Iterator<Integer> it = v.iterator();
//        int i = 0;
//        while (it.hasNext())
//            result[i++] = it.next().byteValue();
//        return result;
    }

    public static String bytesToHexString(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < bytes.length; i++) {
            String hex = Integer.toHexString(0xFF & bytes[i]);
            if (hex.length() == 1) {
                sb.append('0');
            }
            sb.append(hex);
        }
        return sb.toString();
    }

    public static byte[] hexStringToByteArray(String s) {
        s = s.trim();
        int len = s.length();
        byte[] b = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            b[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
        }
        return b;
    }


    public static String getCardNo(String str) {
        StringBuffer sb = new StringBuffer();
        int j = str.length();
        for (int i = 0; i < str.length() / 2; i++) {
            sb.append(str.substring(j - 2, j));
            j = j - 2;
        }
        String cardNo = Integer.parseInt(sb.toString(), 16) + "";
        return cardNo;
    }

    public static byte[] getBitReverseByteArray(byte[] orig) {
        int origLength = orig.length;
        if (origLength < 1) {
            return null;
        }
        byte[] ret = new byte[origLength];
        for (int i = 0; i < origLength; i++) {
            ret[i] = (byte) (0xFF ^ (byte) orig[i]);
        }
        return ret;
    }

    public static byte[] convertIntToByteArray(int org) {
        byte[] ret = new byte[4];
        ret[3] = (byte) (org & 0xFF);
        ret[2] = (byte) ((org >> 8) & 0xFF);
        ret[1] = (byte) ((org >> 16) & 0xFF);
        ret[0] = (byte) ((org >> 24) & 0xFF);
        return ret;
    }

    public static byte[] convertShortToByteArray(int org) {
        byte[] ret = new byte[2];
        ret[1] = (byte) (org & 0xFF);
        ret[0] = (byte) ((org >> 8) & 0xFF);
        return ret;
    }

    public static byte[] convertByteToByteArray(int org) {
        byte[] ret = new byte[1];
        ret[0] = (byte) (org & 0xFF);
        return ret;
    }

    public static byte[] concat(byte[] ba1, byte[] ba2) {
        int ba1Length = ba1.length;
        int ba2Length = ba2.length;
        byte[] ret = new byte[ba1Length + ba2Length];
        System.arraycopy(ba1, 0, ret, 0, ba1Length);
        System.arraycopy(ba2, 0, ret, ba1Length, ba2Length);
        return ret;
    }

    public static byte[] concat(byte[][] bas) {
        int retLength = 0;
        for (int i = 0; i < bas.length; i++) {
            retLength += bas[i].length;
        }
        byte[] ret = new byte[retLength];
        int index = 0;
        for (int j = 0; j < bas.length; j++) {
            int baLength = bas[j].length;
            System.arraycopy(bas[j], 0, ret, index, baLength);
            index += baLength;
        }
        return ret;
    }

    public static byte[] subBytes(byte[] origBytes, int offset, int limit) {
        if (origBytes.length < 1 || origBytes.length < offset || origBytes.length < (offset + limit)) {
            return null;
        }
        byte[] ret = new byte[limit];
        System.arraycopy(origBytes, offset, ret, 0, limit);
        return ret;
    }
}
