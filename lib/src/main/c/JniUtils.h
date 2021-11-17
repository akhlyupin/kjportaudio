/*
 *  JniUtils.h
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include <jni.h>

#ifndef JNI_UTILS_H
#define JNI_UTILS_H

extern jint JNI_GetIntField( JNIEnv *env, jobject obj, const char *fieldName );
extern void JNI_SetIntField( JNIEnv *env, jobject obj, const char *fieldName, jint value );

extern jlong JNI_GetLongField( JNIEnv *env, jobject obj, const char *fieldName );
extern void JNI_SetLongField( JNIEnv *env, jobject obj, const char *fieldName, jlong value );

extern void JNI_SetDoubleField( JNIEnv *env, jobject obj, const char *fieldName, jdouble value );
extern jdouble JNI_GetDoubleField( JNIEnv *env, jobject obj, const char *fieldName );

extern void JNI_SetStringField( JNIEnv *env, jobject obj, const char *fieldName, const char *value );

extern jobject JNI_GetObjectField( JNIEnv *env, jobject obj, const char *fieldName, const char * sig );

extern jbyteArray JNI_GetJByteArray(JNIEnv * env, const jbyte * bytes, jsize length);
extern void JNI_GetBytes(JNIEnv * env, jbyteArray arr, jbyte ** bytes, jsize * length);

extern jmethodID JNI_GetObjectMethod(JNIEnv *env, jobject obj, const char *name, const char *sig);

extern jint JNI_ThrowError( JNIEnv *env, const char *message );

#endif /* JNI_UTILS_H */
