/*
 *  Stream.java
 *
 *  Created by Artem Khlyupin on 10/21/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

import java.nio.ByteBuffer;

public class Stream {
    private long        nativeStream;
    private Parameters  inputParameters;
    private Parameters  outputParameters;
    private double      sampleRate;


    public Stream(Parameters inputParameters, Parameters outputParameters, double sampleRate) throws RuntimeException {
        this.inputParameters = inputParameters;
        this.outputParameters = outputParameters;
        this.sampleRate = sampleRate;

        //throws exception if not supported
        isFormatSupported(inputParameters, outputParameters, sampleRate);
    }

    public static class Parameters {
        private final int           deviceIndex;
        private final int           channelCount;
        private final long          sampleFormat;
        private final double        suggestedLatency;
        private final byte[]        hostApiSpecificStreamInfo;

        public Parameters(Device device,
                          int channelCount,
                          SampleFormat sampleFormat,
                          double suggestedLatency,
                          byte[] hostApiSpecificStreamInfo) {

            this.deviceIndex = device.index;
            this.channelCount = channelCount;
            this.sampleFormat = sampleFormat.getValue();
            this.suggestedLatency = suggestedLatency;
            this.hostApiSpecificStreamInfo = hostApiSpecificStreamInfo;
        }
    };

    private native boolean isFormatSupported(
            Parameters input, Parameters output, double sampleRate) throws RuntimeException;

    enum Flags {
        NoFlag(0),
        ClipOff(0x00000001),
        DitherOff(0x00000002),
        NeverDropInput(0x00000004),
        PrimeOutputBuffersUsingStreamCallback(0x00000008),
        PlatformSpecificFlags(0xFFFF0000);

        private final int value;

        private Flags(int value) {
            this.value = value;
        }

    };

    static class CallbackTimeInfo {
        double inputBufferAdcTime;  /**< The time when the first sample of the input buffer was captured at the ADC input */
        double currentTime;         /**< The time when the stream callback was invoked */
        double outputBufferDacTime; /**< The time when the first sample of the output buffer will output the DAC */
    };

    enum CallbackFlags {
        InputUnderflow(0x00000001),
        InputOverflow(0x00000002),
        OutputUnderflow(0x00000004),
        OutputOverflow(0x00000008),
        PrimingOutput(0x00000010);

        private final int value;

        private CallbackFlags(int value) {
            this.value = value;
        }
    };

    enum CallbackResult {
        Continue(9),   /**< Signal that the stream should continue invoking the callback and processing audio. */
        Complete(1),   /**< Signal that the stream should stop invoking the callback and finish once all output samples have played. */
        Abort(2);       /**< Signal that the stream should stop invoking the callback and finish as soon as possible. */

        private int value;

        private CallbackResult(int value) {
            this.value = value;
        }
    };

    interface Listener {
        int onProcess(
                byte[] input,
                byte[] output,
                long frameCount,
                Stream.CallbackTimeInfo timeInfo,
                Stream.CallbackFlags statusFlags,
                byte[] userData );
        int onFinished();
    }

    public native void open(
            long framesPerBuffer,
            Stream.Flags flags,
            Listener listener,
            byte[] userData) throws RuntimeException;

    public native void close() throws RuntimeException;
    public native void start() throws RuntimeException;
    public native void stop() throws RuntimeException;
    public native void abort() throws RuntimeException;

    public native boolean isStreamStopped() throws RuntimeException;
    public native boolean isStreamActive() throws RuntimeException;

    public native void read( byte[] buf, long frames ) throws RuntimeException;
    public native void write( byte[] buf, long frames ) throws RuntimeException;

    public native long getReadAvailable();
    public native long getWriteAvailable();

    static class Info {
        double inputLatency;
        double outputLatency;
        double sampleRate;

        private Info(double inputLatency, double outputLatency, double sampleRate) {
            this.inputLatency = inputLatency;
            this.outputLatency = outputLatency;
            this.sampleRate = sampleRate;
        }
    };
    public native Info getInfo() throws RuntimeException;

    public native double getTime() throws RuntimeException;
    public native double getCpuLoad();

    @Override
    public String toString() {
        return "stream = " + nativeStream;
    }


}
