/*
 *  HostApi.java
 *
 *  Created by Artem Khlyupin on 10/21/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

public class HostApi {
    private final int       index;
    private TypeId          type;
    private final String    name;
    private final int       deviceCount;
    private final int       defaultInputDevice;
    private final int       defaultOutputDevice;

    private HostApi(int index, int type, String name,
                   int deviceCount, int defaultInputDevice, int defaultOutputDevice) {
        this.index = index;

        for (TypeId typeId : TypeId.values()) {
            if (typeId.getValue() == type) {
                this.type = typeId;
                break;
            }
        }

        this.name = name;
        this.deviceCount = deviceCount;
        this.defaultInputDevice = defaultInputDevice;
        this.defaultOutputDevice = defaultOutputDevice;
    }

    public TypeId getType() {
        return type;
    }
    public String getName() {
        return name;
    }
    public int getDeviceCount() {
        return deviceCount;
    }

    public native Device getDevice(int index) throws RuntimeException;
    public native Device getDefaultInputDevice() throws RuntimeException;
    public native Device getDefaultOutputDevice() throws RuntimeException;

    @Override
    public String toString() {
        return getName() + ": type=" + getType().toString() + " device_count=" + getDeviceCount();
    }


    public enum TypeId {
        InDevelopment(0),
        DirectSound(1),
        MME(2),
        ASIO(3),
        S(4),
        CoreAudio(5),
        OSS(7),
        ALSA(8),
        AL(9),
        BeOS(10),
        WDMKS(11),
        JACK(12),
        WASAPI(13),
        AudioScienceHPI(14),
        AudioIO(15);

        private int value;

        private TypeId(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }
    }
}
