/*
 *  StereoStreamTest.java
 *
 *  Created by Artem Khlyupin on 12/22/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

import org.junit.Assert;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class StereoStreamTest {
    Stream.Listener writeListener = new Stream.Listener() {
        @Override
        public int onProcess(byte[] input,
                             byte[] output,
                             long frameCount,
                             Stream.CallbackTimeInfo timeInfo,
                             Stream.CallbackFlags statusFlags,
                             byte[] userData) {

            System.out.println("onProc: frameCount=" + frameCount);

            assert(output != null);

            assertEquals(32768 << 3, output.length);

            byte[] d = SinData.getData();

            for (int i = 0; i < (output.length >> 3); i++) {
                for (int k = 0; k < 4; k++) {
                    output[(i << 3) + k] = d[(i << 2) + k];     //left channel
                    output[(i << 3) + k + 4] = d[(i << 2) + k]; //right channel
                }
            }


            assert(userData != null);
            assertEquals(8, userData.length);

            for (int i = 0; i < 8; i++) {
                assertEquals(i, userData[i]);
            }

            return 0;
        }

        @Override
        public int onFinished() {
            System.out.println("Stream: onFinished");
            return 0;
        }
    };

    @Test
    public void testStreamWrite() {
        try {
            PortAudio.init();
            Device outputDevice = PortAudio.getDefaultHostApi().getDefaultOutputDevice();
            System.out.println(outputDevice.toString());

            Stream.Parameters outputParams = new Stream.Parameters(
                    outputDevice, 2, SampleFormat.Float32, 0, null);
            Stream stream = new Stream(null, outputParams, outputDevice.getDefaultSampleRate());

            byte[] userData = new byte[]{0,1,2,3,4,5,6,7};
            stream.open(32768, Stream.Flags.ClipOff, writeListener, userData);
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
