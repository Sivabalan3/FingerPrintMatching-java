//----CryptoUtil.java--
package com.biometric.util;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;

public class CryptoUtil {

    private static final String ALGO = "AES";
    private static final byte[] keyBytes = "0123456789abcdef".getBytes(); // simple demo key

    public static byte[] encrypt(byte[] data) throws Exception {
        SecretKey key = new SecretKeySpec(keyBytes, ALGO);
        Cipher cipher = Cipher.getInstance(ALGO);
        cipher.init(Cipher.ENCRYPT_MODE, key);
        return cipher.doFinal(data);
    }

    public static byte[] decrypt(byte[] encrypted) throws Exception {
        SecretKey key = new SecretKeySpec(keyBytes, ALGO);
        Cipher cipher = Cipher.getInstance(ALGO);
        cipher.init(Cipher.DECRYPT_MODE, key);
        return cipher.doFinal(encrypted);
    }
}
