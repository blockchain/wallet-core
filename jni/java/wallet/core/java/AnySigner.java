// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package wallet.core.java;

import com.google.protobuf.Message;
import com.google.protobuf.Parser;
import wallet.core.jni.CoinType;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;

import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

public class AnySigner {
    private static String tempFolderPrefix = "trustwallet-static";
    private static Path tempDirWithPrefix;

    private static boolean isInitialized = false;

    public static void initialize() {
        if(isInitialized) {
            return;
        }

        synchronized (tempFolderPrefix) {
            if(isInitialized) {
                return;
            }

            // Load the static library from the resource path somehow...
            try {
                tempDirWithPrefix = Files.createTempDirectory(tempFolderPrefix);
                String resourceName = getResourceName();
                String resourcePath = writeResourceToFile(resourceName);
                System.load(resourcePath);
            } catch (Exception e) {
                System.out.println("Unable to load static library, " + e.getMessage());
                e.printStackTrace();
            }
        }
    }

    private static String getResourceName() {
        String os = System.getProperty("os.name").toLowerCase();
        if (os.contains("mac")) {
            return "libTrustWalletCore-osx.so";
        } else {
            return "libTrustWalletCore-linux.so";
        }
    }

    private static String writeResourceToFile(String resourceName) throws IOException {
        ClassLoader loader = AnySigner.class.getClassLoader();
        InputStream configStream = loader.getResourceAsStream(resourceName);
        File tmpFile = new File(tempDirWithPrefix.toFile().getAbsolutePath() + "/" + resourceName);
        Files.copy(configStream, tmpFile.toPath(), REPLACE_EXISTING);
        return tmpFile.getAbsolutePath();
    }

    public static <T extends Message> T sign(Message input, CoinType coin, Parser<T> parser) throws Exception {
        byte[] data = input.toByteArray();
        byte[] outputData = nativeSign(data, coin.value());
        T output = parser.parseFrom(outputData);
        outputData = null;
        return output;
    }

    public static native byte[] nativeSign(byte[] data, int coin);

    public static byte[] encode(Message input, CoinType coin) throws Exception {
        byte[] data = input.toByteArray();
        return nativeEncode(data, coin.value());
    }

    public static native byte[] nativeEncode(byte[] data, int coin);

    public static byte[] decode(byte[] data, CoinType coin) throws Exception {
        return nativeDecode(data, coin.value());
    }

    public static native byte[] nativeDecode(byte[] data, int coin);

    public static native String signJSON(String json, byte[] key, int coin);

    public static native boolean supportsJSON(int coin);

    public static <T extends Message> T plan(Message input, CoinType coin, Parser<T> parser) throws Exception {
        byte[] data = input.toByteArray();
        byte[] outputData = nativePlan(data, coin.value());
        T output = parser.parseFrom(outputData);
        outputData = null;
        return output;
    }

    public static native byte[] nativePlan(byte[] data, int coin);
}
