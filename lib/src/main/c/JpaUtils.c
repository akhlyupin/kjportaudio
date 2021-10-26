/*
 *  JpaUtils.c
 *
 *  Created by Artem Khlyupin on 10/26/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#include "JpaUtils.h"

jint JPA_CheckError(JNIEnv *env, PaError err) {
    if( err < 0 ) {

        if( err == paUnanticipatedHostError ) {
            const PaHostErrorInfo *hostErrorInfo = Pa_GetLastHostErrorInfo();
            return JNI_ThrowError( env, hostErrorInfo->errorText );

        } else {
            return JNI_ThrowError( env, Pa_GetErrorText( err ) );
        }
    }  

    return err; 
}