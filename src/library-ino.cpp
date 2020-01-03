/** 
 * @file        library-ino.cpp
 * @brief       Arduino Library API
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include <stdint.h>
#include "library-ino.h"
#include "utils/utility.h"

/**
 * @brief Library Class Constructor
 */
LibraryClass::LibraryClass()
{
    attrA  = 0;
    attrB  = 0;
}

/**
 * @brief Library Class Destructor
 */
LibraryClass::~LibraryClass()
{
    /**
     * DeInitialization, resource deallocation, processes stop
     */
}

/**
 * @brief Initialization
 * @return Return code
 */
int LibraryClass::begin()
{
    /**
     * Initialization, resource allocation, processes start
     */

    return 0;
}

/**
 * @brief Initialization
 * @return Return code
 */
int LibraryClass::end()
{
    /**
     * Deinitialization or resource dellocation
     */

    return 0;
}

/**
 * @brief   Some Function
 *          Function calling a Utility function 
 *          to be used as mock example
 * @return  Return code
 * @retval  1 if error
 * @rettval 0 if success
 */
int LibraryClass::someFunction(Utility &u)
{
    int ret = 0;
    char a = '\0';

    if(u.someUtilFunction(a) != 'a')
        ret = 1;

    return ret;
}

/**
 * Additional Public Class Functions 
 * ...
 * 
 * ...
 */
