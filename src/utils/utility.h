/** 
 * @file        utility.h
 * @brief       Additional utility
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdint.h>

class Utility
{
    public:
                    Utility(){};
        /**
         * @brief   Some Util Function
         *          
         * @return  Return code
         * @retval  'a' if something happens
         */
        virtual char someUtilFunction (char a);
};


#endif /** UTILITY_H_ **/