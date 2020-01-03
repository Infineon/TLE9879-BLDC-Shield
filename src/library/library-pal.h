/** 
 * @file        library-pal.h
 * @brief       C Library Platform Abstraction Layer
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef LIBRARY_PAL_H_
#define LIBRARY_PAL_H_

/**
 * @brief PAL Instance XX
 */
typedef struct
{
    /**
     * @brief   XX PAL Instance Initialization
     * 
     *          Example of PAL interface function as pointer.
     *          To be implemented for each particular platform.
     * @return  void 
     */
    void    (*init)(void);

    /**
     * @brief   XX PAL Instance Deinitialization
     * 
     *          Example of PAL interface function as pointer.
     *          To be implemented for each particular platform.
     * @return  void 
     */
    void    (*deinit)(void);

    /**
     * Some more interface functions
     */

}Library_PAL_XX_t;


/**
 * @brief Library PAL (All instances)
 */
typedef struct {
    
    Library_PAL_XX_t *pal_xx;
    
    /* MoRe PAL interfaces */
}Library_PAL_t;

/**
 * @brief   XX PAL Instance Function A
 * 
 *          Example of PAL function prototype.
 *          To be implementated for each particular platform. 
 * @return  void 
 */
void Library_PAL_XX_FunctionA();

/**
 * @brief   XX PAL Instance Function B
 * 
 *          Example of PAL function prototype.
 *          To be implementated for each particular platform. 
 * @return  return code 
 */
int  Library_PAL_XX_FunctionB();

#endif /** LIBRARY_PAL_H_ **/

