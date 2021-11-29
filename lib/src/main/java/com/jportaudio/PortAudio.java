/*
 *  PortAudio.java
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

public class PortAudio {
    static {
        System.loadLibrary("portaudio");
        System.loadLibrary("jportaudio");
    }

    private PortAudio() {
    }

    public static native String getJPAVersionText();

    public static native int getVersion();
    public static native String getVersionText();
    public static native String getVersionControlRevision();

    public static native int init() throws RuntimeException;
    public static native int terminate() throws RuntimeException;

    /* Miscellaneous utilities */
    public static native int getSampleSize(SampleFormat format)
            throws RuntimeException;
    public static native void sleep(long msec);

    /* Host Api */
    public static native int      getHostApiCount() throws RuntimeException;
    public static native HostApi  getHostApi(int index) throws RuntimeException;
    public static native HostApi  getDefaultHostApi() throws RuntimeException;

    /* Device */
    public static native int getDeviceCount() throws RuntimeException;
    public static native Device getDevice(int index) throws RuntimeException;
    public static native Device getDefaultInputDevice() throws RuntimeException;;
    public static native Device getDefaultOutputDevice() throws RuntimeException;;

}
