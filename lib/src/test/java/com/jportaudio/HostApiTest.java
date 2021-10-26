/*
 *  HostApiTest.java
 *
 *  Created by Artem Khlyupin on 10/21/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

import org.junit.Test;

import static org.junit.Assert.fail;

public class HostApiTest {
    @Test
    public void testHostApi() {
        try {
            PortAudio.init();
            PortAudio pa = PortAudio.instance;

            for (int i = 0; i < pa.getHostApiCount(); i++) {
                HostApi hostApi = pa.getHostApi(i);
                System.out.println("HostApi[" + i + "]:" + hostApi.toString());

                for (int u = 0; u < hostApi.getDeviceCount(); u++) {
                    System.out.println("[" + u + "]" +hostApi.getDevice(u).toString());
                }

                System.out.println("Default input device:");
                Device defInputDevice = hostApi.getDefaultInputDevice();
                System.out.println(defInputDevice.toString());

                System.out.println("Default output device:");
                Device defOutputDevice = hostApi.getDefaultOutputDevice();
                System.out.println(defOutputDevice.toString());
            }


            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            fail();
        }
    }
}
