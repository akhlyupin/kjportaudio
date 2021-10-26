/*
 *  JpaUtils.h
 *
 *  Created by Artem Khlyupin on 10/26/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
#ifndef JPA_UTILS_H
#define JPA_UTILS_H

#include "JniUtils.h"
#include "portaudio.h"

extern jint JPA_CheckError(JNIEnv *env, PaError err);

#endif /* JPA_UTILS_H */