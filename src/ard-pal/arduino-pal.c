/** 
 * @file        arduino-pal.c
 * @brief       Arduino Platform Implementation
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include "arduino-pal.h"
#include "../library/library-pal.h"

void Arduino_PAL_XX_init()
{
    /**
     * Arduino Reference Language Implementation
     */
}

void Arduino_PAL_XX_deinit()
{
    /**
     * Arduino Reference Language Implementation
     */
}

Library_PAL_XX_t Library_Arduino_PAL_XX =
{
    Arduino_PAL_XX_init,
    Arduino_PAL_XX_deinit,
};

Library_PAL_t Library_Arduino_PAL = 
{
    &Library_Arduino_PAL_XX
};

void Library_PAL_XX_FunctionA()
{
    /**
     * Arduino Reference Language Implementation
     */
}

int Library_PAL_XX_FunctionB()
{
    /**
     * Arduino Reference Language Implementation
     */

    return 0;
}