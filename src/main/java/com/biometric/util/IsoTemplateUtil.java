//---IsoTemplateUtil.java--
package com.biometric.util;

import java.util.Base64;

public class IsoTemplateUtil {

    public static byte[] decodeBase64(String isoBase64) {
        return Base64.getDecoder().decode(isoBase64);
    }

    public static String encodeBase64(byte[] iso) {
        return Base64.getEncoder().encodeToString(iso);
    }
}
