/*
 *  HostApi.c
 *
 *  Created by Artem Khlyupin on 10/26/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "com_jportaudio_HostApi.h"
#include "portaudio.h"
#include "JniUtils.h"

static jobject getDevice(JNIEnv * env, int index) {
    const PaDeviceInfo * deviceInfo = Pa_GetDeviceInfo(index);
    if (deviceInfo == NULL) {
        return NULL;
    }

    /* Device object construction */
    jclass deviceClass = (*env)->FindClass(env, "com/jportaudio/Device");
    if (deviceClass == NULL) {
        (*env)->DeleteLocalRef(env, deviceClass);
        JNI_ThrowError( env, "Device class found error.\n");
        return NULL;
    }

    jmethodID constructorDevice = (*env)->GetMethodID(
        env, deviceClass, "<init>", "(ILjava/lang/String;IIIDDDDD)V"); 
    if (constructorDevice == NULL) {
        (*env)->DeleteLocalRef(env, deviceClass);
        JNI_ThrowError( env, "Device constructor found error.\n");
        return NULL;
    }

    jstring name = (*env)->NewStringUTF(env, deviceInfo->name);
    if (name == NULL) {
        (*env)->DeleteLocalRef(env, deviceClass);
        (*env)->DeleteLocalRef(env, name);
        JNI_ThrowError( env, "Device name construction error.\n");
        return NULL;
    }

    jobject deviceObj = (*env)->NewObject(env, deviceClass, constructorDevice, 
        index, name, deviceInfo->hostApi, 
        deviceInfo->maxInputChannels, deviceInfo->maxOutputChannels,
        deviceInfo->defaultLowInputLatency, deviceInfo->defaultLowOutputLatency,
        deviceInfo->defaultHighInputLatency, deviceInfo->defaultHighOutputLatency,
        deviceInfo->defaultSampleRate);

    if (deviceObj == NULL) {
        (*env)->DeleteLocalRef(env, deviceClass);
        (*env)->DeleteLocalRef(env, name);
        (*env)->DeleteLocalRef(env, deviceObj);
        JNI_ThrowError( env, "Device object construction error.\n");
        return NULL;
    }

    return deviceObj;
}

JNIEXPORT jobject JNICALL 
Java_com_jportaudio_HostApi_getDevice(
    JNIEnv * env, jobject hostApi, jint hostApiDeviceIndex) {

    int hostApiIndex = JNI_GetIntField(env, hostApi, "index");

    int deviceIndex = Pa_HostApiDeviceIndexToDeviceIndex(
        hostApiIndex, hostApiDeviceIndex );

    return getDevice(env, deviceIndex);
}

JNIEXPORT jobject JNICALL 
Java_com_jportaudio_HostApi_getDefaultInputDevice(
    JNIEnv * env, jobject hostApi) {

    int deviceIndex = JNI_GetIntField(env, hostApi, "defaultInputDevice");
    return getDevice(env, deviceIndex);
}

JNIEXPORT jobject JNICALL 
Java_com_jportaudio_HostApi_getDefaultOutputDevice(
    JNIEnv * env, jobject hostApi) {

    int deviceIndex = JNI_GetIntField(env, hostApi, "defaultOutputDevice");
    return getDevice(env, deviceIndex);
}
