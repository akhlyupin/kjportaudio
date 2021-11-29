/*
 *  HostApi.c
 *
 *  Created by Artem Khlyupin on 10/26/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "com_jportaudio_HostApi.h"
#include "portaudio.h"
#include "JniUtils.h"
#include "Device.h"

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
