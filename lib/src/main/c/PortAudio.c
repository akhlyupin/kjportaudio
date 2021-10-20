/*
 *  PortAudio.c
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "com_jportaudio_PortAudio.h"
#include "portaudio.h"
#include "JniUtils.h"

static jint checkError(JNIEnv *env, PaError err) {
    if( err == -1 ) {
        return JNI_ThrowError( env, "-1, possibly no available default device" );

    } else if( err < 0 ) {

        if( err == paUnanticipatedHostError ) {
            const PaHostErrorInfo *hostErrorInfo = Pa_GetLastHostErrorInfo();
            return JNI_ThrowError( env, hostErrorInfo->errorText );

        } else {
            return JNI_ThrowError( env, Pa_GetErrorText( err ) );
        }

    } else {
        return err;
    }
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_getVersion(JNIEnv * env, jclass c) {
    return Pa_GetVersion();
}

JNIEXPORT jstring JNICALL
Java_com_jportaudio_PortAudio_getVersionText(JNIEnv * env, jclass c) {
    return (*env)->NewStringUTF(env, Pa_GetVersionInfo()->versionText );
}

JNIEXPORT jstring JNICALL
Java_com_jportaudio_PortAudio_getVersionControlRevision(JNIEnv * env, jclass c) {
    return (*env)->NewStringUTF(env, Pa_GetVersionInfo()->versionControlRevision );
}

JNIEXPORT jstring JNICALL
Java_com_jportaudio_PortAudio_getErrorText(JNIEnv * env, jclass c, jint errorCode) {
    return (*env)->NewStringUTF(env, Pa_GetErrorText( errorCode ) );
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_init(JNIEnv * env, jclass c) {
    PaError err = Pa_Initialize();
    return checkError( env, err );
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_terminate(JNIEnv * env , jclass c) {
    PaError err = Pa_Terminate();
    return checkError( env, err );
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_getSampleSize(JNIEnv * env, jclass c, jobject sampleFormat) {
    jint format = JNI_GetIntField( env, sampleFormat, "value" );
    PaError err = Pa_GetSampleSize( format );
    return checkError( env, err );
}

JNIEXPORT void JNICALL
Java_com_jportaudio_PortAudio_sleep(JNIEnv * env, jclass c, jlong ms) {
    Pa_Sleep(ms);
}
