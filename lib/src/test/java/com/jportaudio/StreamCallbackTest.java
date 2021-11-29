/*
 *  StreamCallbackTest.java
 *
 *  Created by Artem Khlyupin on 11/17/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

import org.junit.Assert;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class StreamCallbackTest {

    Stream.Listener listener = new Stream.Listener() {
        @Override
        public int onProcess(byte[] input,
                             byte[] output,
                             long frameCount,
                             Stream.CallbackTimeInfo timeInfo,
                             Stream.CallbackFlags statusFlags,
                             byte[] userData) {

            System.out.println("onProc: frameCount=" + frameCount);

            assert(input != null);
            assertEquals(32768 << 2, input.length);

            //get float[] from byte[]
            float[] floatData = new float[input.length / 4];
            ByteBuffer bb = ByteBuffer.wrap(input).order(ByteOrder.LITTLE_ENDIAN);
            for (int i = 0; i < input.length; i += 4) {
                floatData[i / 4] = bb.getFloat(i);
            }
            System.out.println("Average = " + getAbsAverage(floatData));

            assert(userData != null);
            assertEquals(8, userData.length);

            for (int i = 0; i < 8; i++) {
                assertEquals(i, userData[i]);
            }

            return 0;
        }

        @Override
        public int onFinished() {
            return 0;
        }

        private float getAbsAverage(float[] data) {
            float avg = 0;

            for (float d : data) {
                avg += Math.abs(d);
            }
            return avg / data.length;
        }
    };

    @Test
    public void testStreamRead() {
        try {
            PortAudio.init();
            Device inputDevice = PortAudio.getDefaultHostApi().getDefaultInputDevice();
            Device outputDevice = PortAudio.getDefaultHostApi().getDefaultOutputDevice();
            System.out.println(inputDevice.toString());
            System.out.println(outputDevice.toString());

            Stream.Parameters inputParams = new Stream.Parameters(
                    inputDevice, 1, SampleFormat.Float32, 0, null);
            Stream.Parameters outputParams = new Stream.Parameters(
                    outputDevice, 1, SampleFormat.Float32, 0, null);
            Stream stream = new Stream(inputParams, outputParams, 44100.0);

            byte[] userData = new byte[]{0,1,2,3,4,5,6,7};
            stream.open(32768, Stream.Flags.ClipOff, listener, userData);
            System.out.println("Open stream!");

            assert(stream.isStreamStopped());

            stream.start();
            System.out.println("Start stream!");

            assert(!stream.isStreamStopped());

            double t = stream.getTime();
            while (stream.getTime() - t < 2);

            stream.stop();
            System.out.println("Stop stream!");
            stream.close();
            System.out.println("Close stream!");

            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            Assert.fail();
        }
    }
}
