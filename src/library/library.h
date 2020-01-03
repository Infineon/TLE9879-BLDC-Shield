/** 
 * @file        library.h
 * @brief       C Library
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "library-types.h"
#include "library-pal.h"

void                Library_Config         (Library_Handle_t      *handle);
Library_Error_t     Library_Init           (Library_Handle_t      *handle);
Library_Error_t     Library_Deinit         (Library_Handle_t      *handle);
Library_Error_t     Library_SomeFunction   (Library_Handle_t      *handle);

#endif /** LIBRARY_H_ **/