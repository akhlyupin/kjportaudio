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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    jobject listener;
    unsigned int sampleLength;
    unsigned int channelCount;
    unsigned int userDataLength;
} CallbackUserData_t;

static JavaVM * javaVM = NULL;

static void getStreamParam(
    JNIEnv * env, jobject streamParam, PaStreamParameters * paStreamParam) {

    if (streamParam != NULL) {
        paStreamParam->device = JNI_GetIntField(env, streamParam, "deviceIndex");
        paStreamParam->channelCount = JNI_GetIntField(env, streamParam, "channelCount");
        paStreamParam->sampleFormat = JNI_GetLongField(env, streamParam, "sampleFormat");
        paStreamParam->suggestedLatency = JNI_GetDoubleField(env, streamParam, "suggestedLatency");
        paStreamParam->hostApiSpecificStreamInfo = NULL;

        jbyteArray specificStreamInfoArray = 
            JNI_GetObjectField( env, streamParam, "hostApiSpecificStreamInfo", "[B" );

        if (specificStreamInfoArray) {
            jbyte * specificStreamInfo;
            jsize specificStreamInfoLength;
            
            JNI_GetBytes(env, specificStreamInfoArray, &specificStreamInfo, &specificStreamInfoLength);
            paStreamParam->hostApiSpecificStreamInfo = specificStreamInfo;

            printf("specificStreamInfoArray :\n");
            for (int i = 0; i < specificStreamInfoLength; i+=4) {
                jbyte * b = specificStreamInfo;
                printf("%d %d %d %d\n", b[i], b[i+1], b[i+2], b[i+3]);
            }
            printf("specificStreamInfoArray END\n");

        } else {
            printf("specificStreamInfoArray == NULL\n");
        }
        
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

static int streamCallback( const void *input, void *output, 
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData ) {
    
    CallbackUserData_t * cbUserData = (CallbackUserData_t *)userData;
    if (cbUserData->listener == NULL) {
        return paAbort;
    }

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_8;
    args.name = "AudioStreamCallbackThread";
    args.group = NULL;
 
    JNIEnv * env = NULL;

    if ((*javaVM)->AttachCurrentThread(javaVM, (void**)&env, &args) != JNI_OK) {
        JNI_ThrowError( env, "Attach audio stream thread error." );
        return paAbort;
    }

    if (env == NULL) {
        (*javaVM)->DetachCurrentThread(javaVM);
        JNI_ThrowError( env, "Get jni environment in stream thread error." );
        return paAbort;
    }

    jmethodID onProcMethod = JNI_GetObjectMethod(env, cbUserData->listener, "onProcess", 
            "([B[BJLcom/jportaudio/Stream$CallbackTimeInfo;Lcom/jportaudio/Stream$CallbackFlags;[B)I");

    if (onProcMethod == NULL) {
        (*javaVM)->DetachCurrentThread(javaVM);
        JNI_ThrowError( env, "Cannot find onProcMethod." );
        return paAbort;
    }

    jbyteArray jUserData = NULL;
    if (cbUserData->userDataLength > 0) {
        jUserData = JNI_GetJByteArray(env, 
                                        userData + sizeof(CallbackUserData_t), 
                                        cbUserData->userDataLength);
    }
    
    
    unsigned long dataLength = frameCount * cbUserData->sampleLength * cbUserData->channelCount;
    jbyteArray inputArray = (input) ? JNI_GetJByteArray(env, input, dataLength) : NULL;
    jbyteArray outputArray = (output) ? (*env)->NewByteArray(env, dataLength) : NULL;

    jint result = (*env)->CallIntMethod(
        env, cbUserData->listener, onProcMethod, 
        inputArray, outputArray, (jlong)frameCount, NULL, NULL, jUserData);

    if (output) {
        jbyte * jOutput;
        jsize length;
        JNI_GetBytes(env, outputArray, &jOutput, &length);

        if (length != dataLength) {
            (*javaVM)->DetachCurrentThread(javaVM);
            JNI_ThrowError( env, "Output data length error." );
            return paAbort;
        } else {
            memcpy(output, jOutput, length);
        }
    }
    

    (*javaVM)->DetachCurrentThread(javaVM);

    return result;
    
}

static void streamFinishedCallback( void* userData ) {
    CallbackUserData_t * cbUserData = (CallbackUserData_t *)userData;

    if (cbUserData->listener) {
        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_8;
        args.name = "AudioStreamFinishCallbackThread";
        args.group = NULL;

        JNIEnv * env = NULL;
        if ((*javaVM)->AttachCurrentThread(javaVM, (void**)&env, &args) != JNI_OK) {
            free(cbUserData);
            JNI_ThrowError( env, "Attach audio finish stream thread error." );
            return;
        } 
        
        jmethodID onFinishedMethod = JNI_GetObjectMethod(env, cbUserData->listener, "onFinished", "()I");
        if (onFinishedMethod) {
            (*env)->CallIntMethod(env, cbUserData->listener, onFinishedMethod);
            
        } else {
            JNI_ThrowError( env, "Cannot find onFinishedMethod." );
        }

        (*env)->DeleteGlobalRef(env, cbUserData->listener);
        (*javaVM)->DetachCurrentThread(javaVM);
    }   
    
    free(cbUserData);
}

static unsigned int getSampleLength(PaSampleFormat sf) {
    switch(sf) {
        case paFloat32: 
            return 4;
        case paInt32:
            return 4;
        case paInt24:
            return 3;
        case paInt16:
            return 2;
        default:
            return 1;
    }
}

static CallbackUserData_t * getUserData(JNIEnv * env, 
                                        jobject listener, 
                                        unsigned int sampleLength, 
                                        unsigned int channelCount,
                                        jbyteArray jUserData) {

    if (listener == NULL) return NULL;

    jsize jUserDataLength = 0;
    jbyte * ud = NULL;
    JNI_GetBytes(env, jUserData, &ud, &jUserDataLength);

    /* make and fill CallbackUserData structure */
    uint8_t * cbUserData = malloc(sizeof(CallbackUserData_t) + jUserDataLength);
    CallbackUserData_t * header = (CallbackUserData_t *)cbUserData;
    header->listener = (*env)->NewGlobalRef(env, listener);
    header->sampleLength = sampleLength;
    header->channelCount = channelCount;
    header->userDataLength = jUserDataLength;

    if (jUserDataLength > 0) {
        memcpy(cbUserData + sizeof(CallbackUserData_t), ud, jUserDataLength);
        (*env)->ReleaseByteArrayElements(env, jUserData, ud, 0);
    }

    return header;    
}

JNIEXPORT void JNICALL 
Java_com_jportaudio_Stream_open(JNIEnv * env, jobject o, 
    jlong framesPerBuffer, jobject flags, jobject listener, jbyteArray jUserData) {
    
    /* get JavaVM */
    if ((*env)->GetJavaVM(env, &javaVM) != JNI_OK) {
        JNI_ThrowError( env, "Cannot get JavaVM." );
        return;
    }
    
    /* get input&output stream params*/
    jobject inputParam = 
        JNI_GetObjectField(env, o, "inputParameters", "Lcom/jportaudio/Stream$Parameters;");
    jobject outputParam = 
        JNI_GetObjectField(env, o, "outputParameters", "Lcom/jportaudio/Stream$Parameters;");

    PaStreamParameters paInputParam, paOutputParam;
    const PaStreamParameters * paInputParamPointer = NULL; 
    const PaStreamParameters * paOutputParamPointer = NULL;
    unsigned int sampleLength = 4;
    unsigned int channelCount = 0;

    if (inputParam != NULL) {
        getStreamParam(env, inputParam, &paInputParam);
        paInputParamPointer = &paInputParam;

        sampleLength = getSampleLength(paInputParam.sampleFormat); 
        channelCount = paInputParam.channelCount;
    }

    if (outputParam != NULL) {
        getStreamParam(env, outputParam, &paOutputParam);
        paOutputParamPointer = &paOutputParam;

        sampleLength = getSampleLength(paOutputParam.sampleFormat); 
        if (inputParam != NULL) {
            if (paOutputParam.channelCount != paInputParam.channelCount) {
                JNI_ThrowError( env, "Input and Output stream channel count are not equals." );
                return;
            }

        } else {
            channelCount = paOutputParam.channelCount;
        }
        
    }

    /* get sample rate */
    double sampleRate = JNI_GetDoubleField(env, o, "sampleRate");

    /* get stream flags */
    PaStreamFlags paFlags = JNI_GetIntField(env, flags, "value");
        
    /* get user data*/
    CallbackUserData_t * ud = getUserData(env, 
                                            listener, 
                                            sampleLength, 
                                            channelCount, 
                                            jUserData);

    /* open stream */
    PaStream * paStream;
    PaError err = Pa_OpenStream(    &paStream,
                                    paInputParamPointer,
                                    paOutputParamPointer,
                                    sampleRate,
                                    framesPerBuffer,
                                    paFlags,
                                    (listener) ? streamCallback : NULL, 
                                    ud);

    JNI_SetLongField(env, o, "nativeStream", (jlong)paStream);
    JPA_CheckError(env, err);
    if ((*env)->ExceptionOccurred(env)) return;

    /* set finish callback */
    err = Pa_SetStreamFinishedCallback( paStream, streamFinishedCallback );
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