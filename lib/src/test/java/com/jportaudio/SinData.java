/*
 *  SinData.java
 *
 *  Created by Artem Khlyupin on 12/22/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.jportaudio;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class SinData {
    static byte[] getData() {
        int length = 32768 * 2;
        ByteBuffer bb = ByteBuffer.allocate(length * 4).order(ByteOrder.LITTLE_ENDIAN);
        for (int i = 0; i < length; i++) {
            bb.putFloat(i * 4, (float)Math.sin((float)i / 44));  // ~1000Hz
        }
        return bb.array();
    }
}
