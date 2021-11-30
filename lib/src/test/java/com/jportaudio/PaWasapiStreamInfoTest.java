package com.jportaudio;

import org.junit.Assert;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

public class PaWasapiStreamInfoTest {
    @Test
    public void GetStreamInfoTest() {
        PortAudio.init();
        byte[] data = PaWasapiStreamInfo.getData();

        if ((data == null) || (data.length == 0)) {
            System.out.println("Length = NULL");
            fail();
        }

        System.out.println("Length = " + data.length);

        for (int i = 0; i < data.length; i += 4) {
            System.out.println(data[i] + " " + data[i + 1] + " " + data[i + 2] + " " + data[i + 3]);
        }

    }

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
            System.out.println("Stream: onFinished");
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

    private Device getWasapiInput() throws RuntimeException {
        for (int i = 0; i < PortAudio.getHostApiCount(); i++) {
            if (PortAudio.getHostApi(i).getType() == HostApi.TypeId.WASAPI) {
                return PortAudio.getHostApi(i).getDefaultInputDevice();
            }
        }
        return null;
    }

    @Test
    public void CallbackReadWithWasapiExclusiveTest() {
        try {
            PortAudio.init();
            Device inputDevice = getWasapiInput();
            System.out.println(inputDevice.toString());

            Stream.Parameters inputParams = new Stream.Parameters(
                    inputDevice, 1, SampleFormat.Float32, 0, PaWasapiStreamInfo.getData());
            Stream stream = new Stream(inputParams, null, 48000.0);

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