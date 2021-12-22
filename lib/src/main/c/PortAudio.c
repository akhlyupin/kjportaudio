/*
 *  PortAudio.c
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "com_jportaudio_PortAudio.h"
#include "portaudio.h"
#include "JniUtils.h"
#include "JpaUtils.h"
#include "Device.h"

JNIEXPORT jstring JNICALL 
Java_com_jportaudio_PortAudio_getJPAVersionText(JNIEnv * env, jclass c) {
    return (*env)->NewStringUTF(env, "0.3.0" );
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

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_init(JNIEnv * env, jclass c) {
    PaError err = Pa_Initialize();
    return JPA_CheckError( env, err );
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_terminate(JNIEnv * env , jclass c) {
    PaError err = Pa_Terminate();
    return JPA_CheckError( env, err );
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_getSampleSize(JNIEnv * env, jclass c, jobject sampleFormat) {
    jint format = JNI_GetIntField( env, sampleFormat, "value" );
    PaError err = Pa_GetSampleSize( format );
    return JPA_CheckError( env, err );
}

JNIEXPORT void JNICALL
Java_com_jportaudio_PortAudio_sleep(JNIEnv * env, jclass c, jlong ms) {
    Pa_Sleep(ms);
}

JNIEXPORT jint JNICALL
Java_com_jportaudio_PortAudio_getHostApiCount(JNIEnv * env, jclass c) {
    PaError err = Pa_GetHostApiCount();
    return JPA_CheckError( env, err );
}

JNIEXPORT jobject JNICALL
Java_com_jportaudio_PortAudio_getHostApi(JNIEnv * env, jclass c, jint index) {
    const PaHostApiInfo * hostApi = Pa_GetHostApiInfo(index);
    if (hostApi == NULL) {
        JNI_ThrowError( env, "Get HostApiInfo error.\n");
        return NULL;
    }

    jclass hostApiClass = (*env)->FindClass(env, "com/jportaudio/HostApi");
    if (hostApiClass == NULL) {
        (*env)->DeleteLocalRef(env, hostApiClass);
        JNI_ThrowError( env, "HostApi class found error.\n");
        return NULL;
    }

    jmethodID constructorHostApi = (*env)->GetMethodID(
        env, hostApiClass, "<init>", "(IILjava/lang/String;III)V"); 
    if (constructorHostApi == NULL) {
        (*env)->DeleteLocalRef(env, hostApiClass);
        JNI_ThrowError( env, "HostApi constructor found error.\n");
        return NULL;
    }

    jstring name = (*env)->NewStringUTF(env, hostApi->name);
    if (name == NULL) {
        (*env)->DeleteLocalRef(env, hostApiClass);
        (*env)->DeleteLocalRef(env, name);
        JNI_ThrowError( env, "HostApi name found error.\n");
        return NULL;
    }

    jobject hostApiObj = (*env)->NewObject(
        env, hostApiClass, constructorHostApi, 
        index, hostApi->type, name,
        hostApi->deviceCount, hostApi->defaultInputDevice, hostApi->defaultOutputDevice);
    if (hostApiObj == NULL) {
        (*env)->DeleteLocalRef(env, hostApiClass);
        (*env)->DeleteLocalRef(env, name);
        (*env)->DeleteLocalRef(env, hostApiObj);
        JNI_ThrowError( env, "HostApi object create error.\n");
        return NULL;
    }

    return hostApiObj;
}

JNIEXPORT jobject JNICALL 
Java_com_jportaudio_PortAudio_getDefaultHostApi(JNIEnv * env, jclass c) {
    PaError err = Pa_GetDefaultHostApi();
    if (err >= 0) {
        return Java_com_jportaudio_PortAudio_getHostApi(env, c, err); 
    }

    JPA_CheckError( env, err );
    return NULL;
}

JNIEXPORT jint JNICALL 
Java_com_jportaudio_PortAudio_getDeviceCount (JNIEnv * env, jclass c) {
    PaError err = Pa_GetDeviceCount();
    return JPA_CheckError( env, err );
}


JNIEXPORT jobject JNICALL 
Java_com_jportaudio_PortAudio_getDevice(JNIEnv * env, jclass c, jint index) {
    return getDevice(env, index);
}


JNIEXPORT jobject JNICALL 
Java_com_jportaudio_PortAudio_getDefaultInputDevice(JNIEnv * env, jclass c) {
    PaDeviceIndex index = Pa_GetDefaultInputDevice();
    JPA_CheckError( env, index );

    if (index > 0) {
        return getDevice(env, index);
    }
    return NULL; 
}


JNIEXPORT jobject JNICALL 
Java_com_jportaudio_PortAudio_getDefaultOutputDevice(JNIEnv * env, jclass c) {
    PaDeviceIndex index = Pa_GetDefaultOutputDevice();
    JPA_CheckError( env, index );

    if (index > 0) {
        return getDevice(env, index);
    }
    return NULL; 
}
