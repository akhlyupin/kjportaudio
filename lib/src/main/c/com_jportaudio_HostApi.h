/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_jportaudio_HostApi */

#ifndef _Included_com_jportaudio_HostApi
#define _Included_com_jportaudio_HostApi
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jportaudio_HostApi
 * Method:    getDevice
 * Signature: (I)Lcom/jportaudio/Device;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_HostApi_getDevice
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_jportaudio_HostApi
 * Method:    getDefaultInputDevice
 * Signature: ()Lcom/jportaudio/Device;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_HostApi_getDefaultInputDevice
  (JNIEnv *, jobject);

/*
 * Class:     com_jportaudio_HostApi
 * Method:    getDefaultOutputDevice
 * Signature: ()Lcom/jportaudio/Device;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_HostApi_getDefaultOutputDevice
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif