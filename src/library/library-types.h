/** 
 * @file        library-types.h
 * @brief       C Library Types
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef LIBRARY_TYPES_H_
#define LIBRARY_TYPES_H_

#include "library-pal.h"

/**
 * @brief Library Error Return Codes
 */
typedef enum
{
    LIBRARY_OK          =  0,   /**< Success */
    LIBRARY_ERROR_A     = -1,   /**< Some error code */
    /**
     * Some more error/return Codes
     * ...
     */
}Library_Error_t;

/**
 * @brief Library Handle Instance
 */
typedef struct
{
    /**
     * Instances members
     * ...
     */

    Library_PAL_t *pal;     /**< Library PAL instance */
}Library_Handle_t;

#endif /** LIBRARY_TYPES_H_ **/