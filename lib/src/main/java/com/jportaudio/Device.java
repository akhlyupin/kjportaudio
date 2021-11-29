/*
 *  Device.java
 *
 *  Created by Artem Khlyupin on 10/21/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

import java.util.Objects;

public class Device {
    final int               index;
    private final String    name;
    private final int       hostApiIndex;

    private final int maxInputChannels;
    private final int maxOutputChannels;

    /** Default latency values for interactive performance. */
    private final double defaultLowInputLatency;
    private final double defaultLowOutputLatency;
    /** Default latency values for robust non-interactive applications (eg. playing sound files). */
    private final double defaultHighInputLatency;
    private final double defaultHighOutputLatency;

    private final double defaultSampleRate;

    private Device(int index, String name, int hostApiIndex,
                  int maxInputChannels, int maxOutputChannels,
                  double defaultLowInputLatency, double defaultLowOutputLatency,
                  double defaultHighInputLatency, double defaultHighOutputLatency,
                  double defaultSampleRate) {
        this.index = index;
        this.name = name;
        this.hostApiIndex = hostApiIndex;
        this.maxInputChannels = maxInputChannels;
        this.maxOutputChannels = maxOutputChannels;
        this.defaultLowInputLatency = defaultLowInputLatency;
        this.defaultLowOutputLatency = defaultLowOutputLatency;
        this.defaultHighInputLatency = defaultHighInputLatency;
        this.defaultHighOutputLatency = defaultHighOutputLatency;
        this.defaultSampleRate = defaultSampleRate;
    }

    public String getName() {
        return name;
    }

    public int getMaxInputChannels() {
        return maxInputChannels;
    }

    public int getMaxOutputChannels() {
        return maxOutputChannels;
    }

    public double getDefaultLowInputLatency() {
        return defaultLowInputLatency;
    }

    public double getDefaultLowOutputLatency() {
        return defaultLowOutputLatency;
    }

    public double getDefaultHighInputLatency() {
        return defaultHighInputLatency;
    }

    public double getDefaultHighOutputLatency() {
        return defaultHighOutputLatency;
    }

    public double getDefaultSampleRate() {
        return defaultSampleRate;
    }

    @Override
    public String toString() {
        return name + " HostApi=" + hostApiIndex + " MaxInputCh=" + maxInputChannels + " MaxOutputCh=" + maxOutputChannels +
                " defSampleRate=" + defaultSampleRate;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Device device = (Device) o;
        return (index == device.index) &&
                (hostApiIndex == device.hostApiIndex) &&
                (maxInputChannels == device.maxInputChannels) &&
                (maxOutputChannels == device.maxOutputChannels) &&
                Double.compare(device.defaultLowInputLatency, defaultLowInputLatency) == 0 &&
                Double.compare(device.defaultLowOutputLatency, defaultLowOutputLatency) == 0 &&
                Double.compare(device.defaultHighInputLatency, defaultHighInputLatency) == 0 &&
                Double.compare(device.defaultHighOutputLatency, defaultHighOutputLatency) == 0 &&
                Double.compare(device.defaultSampleRate, defaultSampleRate) == 0 &&
                name.equals(device.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(index, name, hostApiIndex, maxInputChannels, maxOutputChannels, defaultLowInputLatency, defaultLowOutputLatency, defaultHighInputLatency, defaultHighOutputLatency, defaultSampleRate);
    }
}
