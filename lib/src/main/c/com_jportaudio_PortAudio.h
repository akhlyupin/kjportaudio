/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_jportaudio_PortAudio */

#ifndef _Included_com_jportaudio_PortAudio
#define _Included_com_jportaudio_PortAudio
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getJPAVersionText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_jportaudio_PortAudio_getJPAVersionText
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jportaudio_PortAudio_getVersion
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getVersionText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_jportaudio_PortAudio_getVersionText
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getVersionControlRevision
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_jportaudio_PortAudio_getVersionControlRevision
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jportaudio_PortAudio_init
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    terminate
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jportaudio_PortAudio_terminate
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getSampleSize
 * Signature: (Lcom/jportaudio/SampleFormat;)I
 */
JNIEXPORT jint JNICALL Java_com_jportaudio_PortAudio_getSampleSize
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    sleep
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_jportaudio_PortAudio_sleep
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getHostApiCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jportaudio_PortAudio_getHostApiCount
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getHostApi
 * Signature: (I)Lcom/jportaudio/HostApi;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_PortAudio_getHostApi
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getDefaultHostApi
 * Signature: ()Lcom/jportaudio/HostApi;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_PortAudio_getDefaultHostApi
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getDeviceCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jportaudio_PortAudio_getDeviceCount
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getDevice
 * Signature: (I)Lcom/jportaudio/Device;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_PortAudio_getDevice
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getDefaultInputDevice
 * Signature: ()Lcom/jportaudio/Device;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_PortAudio_getDefaultInputDevice
  (JNIEnv *, jclass);

/*
 * Class:     com_jportaudio_PortAudio
 * Method:    getDefaultOutputDevice
 * Signature: ()Lcom/jportaudio/Device;
 */
JNIEXPORT jobject JNICALL Java_com_jportaudio_PortAudio_getDefaultOutputDevice
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
