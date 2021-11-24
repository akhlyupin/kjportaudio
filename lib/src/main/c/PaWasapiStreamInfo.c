/*
 *  PaWasapiStreamInfo.c
 *
 *  Created by Artem Khlyupin on 11/23/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "com_jportaudio_PaWasapiStreamInfo.h"
#include "portaudio.h"
#include "pa_win_wasapi.h"
#include <stddef.h>


JNIEXPORT jbyteArray JNICALL 
Java_com_jportaudio_PaWasapiStreamInfo_getData(JNIEnv * env, jclass class) {
    PaWasapiStreamInfo streamInfo;
    streamInfo.size = sizeof(PaWasapiStreamInfo);
    streamInfo.hostApiType = paWASAPI;
    streamInfo.version = 1;
    streamInfo.flags = (paWinWasapiExclusive | paWinWasapiThreadPriority);
    streamInfo.channelMask = 0;
    streamInfo.hostProcessorOutput = NULL;
    streamInfo.hostProcessorInput = NULL;
    streamInfo.threadPriority = eThreadPriorityProAudio;
    streamInfo.streamCategory =  eAudioCategoryOther;
    streamInfo.streamOption = eStreamOptionNone;

    jbyteArray data = (*env)->NewByteArray(env, streamInfo.size);
    if (data == NULL) {
        return data;
    }
    (*env)->SetByteArrayRegion(env, data, 0, streamInfo.size, (jbyte *)&streamInfo);

    printf("jni ul=%d\n", sizeof(unsigned long));
    printf("jni ui=%d\n", sizeof(unsigned int));
    printf("%d\n", offsetof(PaWasapiStreamInfo, hostApiType));
    printf("%d\n", offsetof(PaWasapiStreamInfo, version));
    printf("%d s=%d\n", offsetof(PaWasapiStreamInfo, flags), sizeof(streamInfo.flags));
    printf("%d s=%d\n", offsetof(PaWasapiStreamInfo, channelMask), sizeof(streamInfo.channelMask));
    printf("%d s=%d\n", offsetof(PaWasapiStreamInfo, hostProcessorOutput), sizeof(streamInfo.hostProcessorOutput));
    printf("%d\n", offsetof(PaWasapiStreamInfo, hostProcessorInput));
    printf("%d\n", offsetof(PaWasapiStreamInfo, threadPriority));
    printf("%d\n", offsetof(PaWasapiStreamInfo, streamCategory));
    printf("%d\n", offsetof(PaWasapiStreamInfo, streamOption));

    for (int i = 0; i < 56; i+=4) {
        jbyte * b = (jbyte *)&streamInfo;
        
        printf("%d %d %d %d\n", b[i], b[i+1], b[i+2], b[i+3]);
    }
    
    return data;
}