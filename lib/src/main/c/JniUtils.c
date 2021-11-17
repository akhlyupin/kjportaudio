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

jobject JNI_GetObjectField( JNIEnv *env, jobject obj, const char *fieldName, const char *sig ) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find object JNI field. GetObjectClass error." );
        return NULL;
    }

    jfieldID fid = (*env)->GetFieldID(env, c, fieldName, sig);
    if (fid == NULL) {
        JNI_ThrowError( env, "Cannot find object JNI field." );
        return NULL;
    } else {
        return (*env)->GetObjectField(env, obj, fid );
    }
}

jbyteArray JNI_GetJByteArray(JNIEnv * env, const jbyte * bytes, jsize length) {
    jbyteArray byteArray = (*env)->NewByteArray(env, length);
    (*env)->SetByteArrayRegion(env, byteArray, 0, length, (jbyte *)bytes);
    return byteArray;
}

void JNI_GetBytes(JNIEnv * env, jbyteArray arr, jbyte ** bytes, jsize * length) {
    *length = 0;
    *bytes = NULL;

    if (arr) {
        *length = (*env)->GetArrayLength(env, arr);

        if (*length > 0) {
            *bytes = (*env)->GetByteArrayElements(env, arr, NULL);
        }
    }
}

jmethodID JNI_GetObjectMethod(JNIEnv *env, jobject obj, const char *name, const char *sig) {
    jclass c = (*env)->GetObjectClass(env, obj);
    if (c == NULL) {
        JNI_ThrowError( env, "Cannot find object JNI method. GetObjectClass error." );
        return NULL;
    }

    jmethodID mid = (*env)->GetMethodID(env, c, name, sig);
    if (mid == NULL) {
        JNI_ThrowError( env, "Cannot find object JNI method." );
        return NULL;
    } else {
        return mid;
    }
}



jint JNI_ThrowError( JNIEnv *env, const char *message ) {
    return (*env)->ThrowNew(env,
                            (*env)->FindClass( env, "java/lang/RuntimeException"),
                            message);
}
