/** 
 * @file        library.c
 * @brief       C Library
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include "library.h"
#include "library-pal.h"

/**
 * @brief           Instance configuration
 * @param[in,out]   *handle
 * @return          void 
 */
void Library_Config(Library_Handle_t *handle)
{
    /**
     * Some code goes here
     * ...
     */
}

/**
 * @brief           Instance initialization
 * @param[in,out]   *handle
 * @return          Library error code 
 * @retval          LIBRARY_OK if success
 */
Library_Error_t Library_Init(Library_Handle_t *handle)
{
    /**
     * Some code goes here
     * ...
     */
    return LIBRARY_OK;
}

/**
 * @brief           Instance deinitializion
 * @param[in,out]   *handle
 * @return          Library error code 
 * @retval          LIBRARY_OK if success
 */
Library_Error_t Library_Deinit(Library_Handle_t *handle)
{
    /**
     * Some code goes here
     * ...
     */
    return LIBRARY_OK;
}

/**
 * @brief           Some function
 * @param[in,out]   *handle
 * @return          Library error code 
 * @retval          LIBRARY_OK if success
 */
Library_Error_t Library_SomeFunction(Library_Handle_t *handle)
{
    /**
     * Some code goes here
     * ...
     */

    /**
     * Call to pal function
     */
    Library_PAL_XX_FunctionA();

    return LIBRARY_OK;
}