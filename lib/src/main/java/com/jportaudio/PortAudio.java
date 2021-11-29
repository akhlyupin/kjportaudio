/*
 *  PortAudio.java
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

public class PortAudio {
    public static PortAudio instance = new PortAudio();

    static {
        System.loadLibrary("portaudio");
        System.loadLibrary("jportaudio");
    }

    private PortAudio() {
        update();
    }

    public static native String getJPAVersionText();

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

    /* Host Api */
    public native int      getHostApiCount() throws RuntimeException;
    public native HostApi  getHostApi(int index) throws RuntimeException;
    public native HostApi  getDefaultHostApi() throws RuntimeException;

    /* Device */
    /*private Device[] device;
    public int getDeviceCount() {
        return device.length;
    }
    public Device getDevice(int index) {
        return device[index];
    }
    public Device getDefaultInputDevice() {
        return null;
    }
    public Device getDefaultOutputDevice() {
        return null;
    }*/

    /* update hostApi and device objects */
    public void update() {

    }
}
