/*
 *  JniUtils.c
 *
 *  Created by Artem Khlyupin on 10/20/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "JniUtils.h"

jint JNI_GetIntField(JNIEnv *env, jobject obj, const char *fieldName) {
     jclass c = (*env)->GetObjectClass(env, obj);
     if (c == NULL) {
        JNI_ThrowError( env, "Cannot find integer JNI field. GetObjectClass error." );
        return 0;
     }

     jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "I");
     if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find integer JNI field." );
        return 0;

     } else {
        return (*env)->GetIntField(env, obj, fid );
     }
}

void JNI_SetIntField( JNIEnv *env, jobject obj, const char *fieldName, jint value ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find integer JNI field. GetObjectClass error." );
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "I");
    if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find integer JNI field." );


    } else {
        (*env)->SetIntField(env, obj, fid, value );
    }
}

jlong JNI_GetLongField( JNIEnv *env, jobject obj, const char *fieldName ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find long JNI field. GetObjectClass error." );
        return 0L;
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "J");
    if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find long JNI field." );
        return 0L;

    } else {
        return (*env)->GetLongField(env, obj, fid );
    }
}

void JNI_SetLongField( JNIEnv *env, jobject obj, const char *fieldName, jlong value ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find long JNI field. GetObjectClass error." );
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "J");
    if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find long JNI field." );

    } else {
        (*env)->SetLongField(env, obj, fid, value );
    }
}


void JNI_SetDoubleField( JNIEnv *env, jobject obj, const char *fieldName, jdouble value ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find double JNI field. GetObjectClass error." );
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "D");
    if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find double JNI field." );
    } else {
        (*env)->SetDoubleField(env, obj, fid, value );
    }
}


jdouble JNI_GetDoubleField( JNIEnv *env, jobject obj, const char *fieldName ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find double JNI field. GetObjectClass error." );
        return 0;
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "D");
    if (fid == NULL) {
       JNI_ThrowError( env, "Cannot find double JNI field." );
        return 0;
    } else {
        return (*env)->GetDoubleField(env, obj, fid );
    }
}

void JNI_SetStringField( JNIEnv *env, jobject obj, const char *fieldName, const char *value ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find String JNI field. GetObjectClass error." );
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, "Ljava/lang/String;");
    if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find String JNI field." );

    } else {
        jstring jstr = (*env)->NewStringUTF(env, value);
        if (jstr == NULL) {
            JNI_ThrowError( env, "Cannot create new String." );
        } else {
            (*env)->SetObjectField(env, obj, fid, jstr );
        }
     }
}

jint JNI_ThrowError( JNIEnv *env, const char *message ) {
    return (*env)->ThrowNew(env,
                            (*env)->FindClass( env, "java/lang/RuntimeException"),
                            message);
}
