package com.jportaudio;

import org.junit.Assert;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class StreamTest {

    @Test
    public void testConstruction() {
        try {
            PortAudio.init();

            Device inputDevice = PortAudio.getDefaultHostApi().getDefaultInputDevice();
            System.out.println(inputDevice.toString());

            Stream.Parameters params = new Stream.Parameters(
                    inputDevice, 1, SampleFormat.Float32, 0, null);
            Stream s = new Stream(params, null, 44100.0);



            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            Assert.fail();
        }
    }

    private Device getWasapiInput() throws RuntimeException {

        for (int i = 0; i < PortAudio.getHostApiCount(); i++) {
            if (PortAudio.getHostApi(i).getType() == HostApi.TypeId.WASAPI) {
                return PortAudio.getHostApi(i).getDefaultInputDevice();
            }
        }
        return null;
    }

    @Test
    public void testWasapiConstruction() {
        try {
            PortAudio.init();
            Device inputDevice = getWasapiInput();
            System.out.println(inputDevice.toString());

            Stream.Parameters params = new Stream.Parameters(
                    inputDevice, 1, SampleFormat.Float32, 0, null);
            Stream s = new Stream(params, null, 48000.0);



            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            Assert.fail();
        }
    }

    private byte[] getSinData() {
        int length = 32768 * 2;
        ByteBuffer bb = ByteBuffer.allocate(length * 4).order(ByteOrder.LITTLE_ENDIAN);
        for (int i = 0; i < length; i++) {
            bb.putFloat(i * 4, (float)Math.sin((float)i / 44));  // ~1000Hz
        }
        return bb.array();
    }

    @Test
    public void testBlockingStreamWrite() {
        try {
            PortAudio.init();
            Device inputDevice = PortAudio.getDefaultHostApi().getDefaultInputDevice();
            Device outputDevice = PortAudio.getDefaultHostApi().getDefaultOutputDevice();
            System.out.println(inputDevice.toString());
            System.out.println(outputDevice.toString());

            Stream.Parameters inputParams = new Stream.Parameters(
                    inputDevice, 1, SampleFormat.Float32, 0, null);
            Stream.Parameters outputParams = new Stream.Parameters(
                    outputDevice, 1, SampleFormat.Float32, 0,null);
            Stream stream = new Stream(inputParams, outputParams, 44100.0);

            System.out.println(stream);
            stream.open(32768, Stream.Flags.ClipOff, null, null);
            System.out.println(stream);

            assert(stream.isStreamStopped());

            stream.start();
            System.out.println(stream);

            assert(!stream.isStreamStopped());

            byte[] data = getSinData();
            stream.write(data, 65536);


            double t = stream.getTime();
            while (stream.getTime() - t < 1);

            stream.stop();
            System.out.println(stream);
            stream.close();
            System.out.println(stream);

            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            Assert.fail();
        }
    }

    private float getAbsAverage(float[] data) {
        float avg = 0;

        for (float d : data) {
            avg += Math.abs(d);
        }
        return avg / data.length;
    }

    @Test
    public void testBlockingStreamRead() {
        try {
            PortAudio.init();
            Device inputDevice = PortAudio.getDefaultHostApi().getDefaultInputDevice();
            Device outputDevice = PortAudio.getDefaultHostApi().getDefaultOutputDevice();
            System.out.println(inputDevice.toString());

            Stream.Parameters inputParams = new Stream.Parameters(
                    inputDevice, 1, SampleFormat.Float32, 0, null);
            Stream.Parameters outputParams = new Stream.Parameters(
                    outputDevice, 1, SampleFormat.Float32, 0, null);
            Stream stream = new Stream(inputParams, outputParams, 44100.0);

            System.out.println(stream);
            stream.open(65536, Stream.Flags.ClipOff, null, null);

            assert(stream.isStreamStopped());

            stream.start();
            System.out.println(stream);

            assert(!stream.isStreamStopped());

            //wait for available 65536 buf
            while (stream.getReadAvailable() < 65536) {
                PortAudio.sleep(500);
                System.out.println(stream.getReadAvailable());
            }

            //read 65536 floats
            byte[] data = new byte[65536 * 4];
            stream.read(data, 65536);

            //get float[] from byte[]
            float[] floatData = new float[data.length / 4];
            ByteBuffer bb = ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);
            for (int i = 0; i < data.length; i += 4) {
                floatData[i / 4] = bb.getFloat(i);
            }
            System.out.println("Average = " + getAbsAverage(floatData));

            stream.stop();
            stream.close();

            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            Assert.fail();
        }
    }

}