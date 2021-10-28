/*
 *  Stream.c
 *
 *  Created by Artem Khlyupin on 10/26/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "com_jportaudio_Stream.h"
#include "portaudio.h"
#include "JniUtils.h"
#include "JpaUtils.h"

static void getStreamParam(
    JNIEnv * env, jobject streamParam, PaStreamParameters * paStreamParam) {

    if (streamParam != NULL) {
        paStreamParam->device = JNI_GetIntField(env, streamParam, "deviceIndex");
        paStreamParam->channelCount = JNI_GetIntField(env, streamParam, "channelCount");
        paStreamParam->sampleFormat = JNI_GetLongField(env, streamParam, "sampleFormat");
        paStreamParam->suggestedLatency = JNI_GetDoubleField(env, streamParam, "suggestedLatency");
        //TODO: implement get hostApiSpecificStreamInfo
        //paInputParams.hostApiSpecificStreamInfo = JNI_GetIntField(env, streamParam, "hostApiSpecificStreamInfo");
        paStreamParam->hostApiSpecificStreamInfo = NULL;
    }

}

JNIEXPORT jboolean JNICALL 
Java_com_jportaudio_Stream_isFormatSupported(
    JNIEnv * env, jobject o, 
    jobject inputParam, jobject outputParam, jdouble sampleRate) {
    
    PaStreamParameters paInputParam, paOutputParam;
    PaStreamParameters * paInputParamPointer = NULL; 
    PaStreamParameters * paOutputParamPointer = NULL;

    if (inputParam != NULL) {
        getStreamParam(env, inputParam, &paInputParam);
        paInputParamPointer = &paInputParam;
    }

    if (outputParam != NULL) {
        getStreamParam(env, outputParam, &paOutputParam);
        paOutputParamPointer = &paOutputParam;
    }

    if ((*env)->ExceptionCheck(env)) {
        return 0;
    }

    PaError err = Pa_IsFormatSupported(paInputParamPointer, paOutputParamPointer, sampleRate);
    if (err == 0) {
        return 1;
    }

    return JPA_CheckError(env, err);
}


JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_open(JNIEnv * env, jobject o, 
    jlong framesPerBuffer, jobject flags, jobject listener, jobject userData) {
    
    /* get stream pointer*/
    PaStream * paStream;

    /* get input&output stream params*/
    jobject inputParam = 
        JNI_GetObjectField(env, o, "inputParameters", "Lcom/jportaudio/Stream$Parameters;");
    jobject outputParam = 
        JNI_GetObjectField(env, o, "outputParameters", "Lcom/jportaudio/Stream$Parameters;");

    PaStreamParameters paInputParam, paOutputParam;
    const PaStreamParameters * paInputParamPointer = NULL; 
    const PaStreamParameters * paOutputParamPointer = NULL;

    if (inputParam != NULL) {
        getStreamParam(env, inputParam, &paInputParam);
        paInputParamPointer = &paInputParam;
    }

    if (outputParam != NULL) {
        getStreamParam(env, outputParam, &paOutputParam);
        paOutputParamPointer = &paOutputParam;
    }

    /* get sample rate */
    double sampleRate = JNI_GetDoubleField(env, o, "sampleRate");

    /* get stream flags */
    PaStreamFlags paFlags = JNI_GetIntField(env, flags, "value");

    PaError err = Pa_OpenStream(    &paStream,
                                    paInputParamPointer,
                                    paOutputParamPointer,
                                    sampleRate,
                                    framesPerBuffer,
                                    paFlags,
                                    NULL, //PaStreamCallback *streamCallback,
                                    NULL);
    JNI_SetLongField(env, o, "nativeStream", (jlong)paStream);

    JPA_CheckError(env, err);
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_close(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    PaError err = Pa_CloseStream(paStream);
    JPA_CheckError(env, err);
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_start(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    PaError err = Pa_StartStream(paStream);
    JPA_CheckError(env, err);
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_stop(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    PaError err = Pa_StopStream(paStream);
    JPA_CheckError(env, err);
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_abort(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    PaError err = Pa_AbortStream( paStream );
    JPA_CheckError(env, err);
}

JNIEXPORT jboolean JNICALL 
Java_com_jportaudio_Stream_isStreamStopped(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    PaError err = Pa_IsStreamStopped( paStream );
    if (err >= 0) {
        return err;
    }
    return JPA_CheckError(env, err);
}

JNIEXPORT jboolean JNICALL 
Java_com_jportaudio_Stream_isStreamActive(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    PaError err = Pa_IsStreamActive( paStream );
    if (err >= 0) {
        return err;
    }
    return JPA_CheckError(env, err);
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_read(JNIEnv * env , jobject o, jbyteArray buf, jlong frames) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    jboolean copy = 0;
    jbyte * d = (*env)->GetByteArrayElements(env, buf, &copy);

    PaError err = Pa_ReadStream( paStream, (void *)d, frames );
    (*env)->ReleaseByteArrayElements(env, buf, d, 0);

    JPA_CheckError(env, err);
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_write(JNIEnv * env, jobject o, jbyteArray buf, jlong frames) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    jboolean copy = 0;
    jbyte * d = (*env)->GetByteArrayElements(env, buf, &copy);

    PaError err = Pa_WriteStream( paStream, (const void *)d, frames );
    (*env)->ReleaseByteArrayElements(env, buf, d, 0);

    JPA_CheckError(env, err);
}


JNIEXPORT jlong JNICALL 
Java_com_jportaudio_Stream_getReadAvailable(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    long result = Pa_GetStreamReadAvailable( paStream );
    if (result >= 0) {
        return result;
    }
    return JPA_CheckError(env, result);
}

JNIEXPORT jlong JNICALL 
Java_com_jportaudio_Stream_getWriteAvailable(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    long result = Pa_GetStreamWriteAvailable( paStream );
    if (result >= 0) {
        return result;
    }
    return JPA_CheckError(env, result);
}

JNIEXPORT jobject JNICALL 
Java_com_jportaudio_Stream_getInfo(JNIEnv * env, jobject o) {
    //TODO: need implement
    return NULL;
}

JNIEXPORT jdouble JNICALL 
Java_com_jportaudio_Stream_getTime(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    return Pa_GetStreamTime( paStream );
}

JNIEXPORT jdouble JNICALL 
Java_com_jportaudio_Stream_getCpuLoad(JNIEnv * env, jobject o) {
    PaStream * paStream = (PaStream *)JNI_GetLongField(env, o, "nativeStream");

    return Pa_GetStreamCpuLoad( paStream );
}