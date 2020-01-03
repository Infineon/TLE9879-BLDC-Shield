/** 
 * @file        library-ino.h
 * @brief       Arduino Library API
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef LIBRARY_INO_H_
#define LIBRARY_INO_H_

#include <stdint.h>
#include "utils/utility.h"
extern "C"
{
#include "library/library.h"
}

/**
 * @class Library 
 */
class LibraryClass
{
    private:
        Library_Handle_t handle;    /**< C struct handle */
        uint32_t attrA;             /**< Class attribute */
        uint16_t attrB;             /**< Class attribute */

    public:

        LibraryClass                       ();
        ~LibraryClass                      ();
        int                begin           ();
        int                end             ();
        int                someFunction    (Utility &u);
        
        /**
         * Additional Public Class Functions 
         * ...
         * 
         * ...
         */
};

#endif /** LIBRARY_INO_H_ **/