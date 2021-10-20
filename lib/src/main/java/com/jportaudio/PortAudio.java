/*
 *  PortAudio.java
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

public class PortAudio {
    static {
        System.loadLibrary("jportaudio");
    }
    public static native int getVersion();
    public static native String getVersionText();
    public static native String getVersionControlRevision();

    public static native String getErrorText(int error);

    public static native int init() throws RuntimeException;
    public static native int terminate() throws RuntimeException;

    /* Miscellaneous utilities */
    public static native int getSampleSize(SampleFormat format)
            throws RuntimeException;
    public static native void sleep(long msec);
}
