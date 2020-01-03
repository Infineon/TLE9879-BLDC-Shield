/** 
 * @file        exampleA.ino
 * @brief       Library Example Sketch
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include <Arduino.h>
#include <library-ino.h>

int ret = 0;              /**< Return code */
LibraryClass libOjbect;   /**< Library object */


void setup()
{
  delay(500);
  
  /**
   * Serial Initiliazion 
   */ 
  Serial.begin(115200);
  Serial.println("Serial initialized.");

  /**
   * Library object initialization 
   */ 
   ret = libOjbect.begin();
   if(ret != LIBRARY_OK)
  {
      Serial.print("Error: ");
      Serial.println(ret);
  }

  /**
   * Some other init functions
   */
}

void loop()
{
    /**
     * Library API examples
     */
}