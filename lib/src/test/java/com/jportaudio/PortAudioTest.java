/*
 * This Java source file was generated by the Gradle 'init' task.
 */
package com.jportaudio;

import org.junit.Test;
import static org.junit.Assert.*;

public class PortAudioTest {
    @Test public void testInitPortAudio() {
        try {
            PortAudio.init();
            PortAudio.terminate();
        } catch (RuntimeException e) {
            System.out.println(e);
            fail();
        }
    }
}
