/*******************************************************************************
* Company:
*   Microchip Technology Inc.
*
* File Name:
*   Arduino_MCP3564_vX(API).cpp
*
* Summary:
*   This file contains the source code for the MCP3x6x Delta-Sigma ADC 
*   Main Demo Code Loop function. 
*
* Description:
*   This file contains the source code for the MCP3x6x Delta-Sigma ADC Main 
*   Demo Code Loop for use the Arduino Zero Board and the ATSAMD21G18 MCU.  
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/
/*                                                  Section: Included Files                                                                                */
/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/

#include <Arduino.h>
#include "MCP3x6x_Type_Definitions.h"                                                           // Variable definitions header-file.
#include "MCP3x6x_ADC_Definitions.h"                                                            // ADC definitions header-file.
#include "MCP3x6x_Peripheral_Definitions.h"                                                     // Peripheral definitions header-file.
#include "MCP3x6x_SPI_Definitions.h"                                                            // SPI definitions header-file.

/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/
/*                                                  Section: Arduino Setup and Main Loop Functions                                                         */
/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/

void Microchip_Banner(void);                                                                    // Terminal "Microchip Technology Inc. Arduino Zero MCP3564 Code Example" Banner.
void Operation_Option_Menu(void);                                                               // Terminal Operations Main Menu options. 
void Register_Read_Option_Menu(void);                                                           // Terminal Register-Read Menu options. 
void Display_Output_Data(_u32data_t, _u32data_t, _u16data_t, uint64_t);                         // Terminal Data-Output Display function.  
void Cursor_Ln7_Col0(void);                                                                     // Terminal formatting function; Line 7 Column 0.
void Cursor_Ln9_Col0(void);                                                                     // Terminal formatting function; Line 9 Column 0.
void Cursor_Ln7_Col45(void);                                                                    // Terminal formatting function; Line 7 Column 45.
void Cursor_Ln9_Col45(void);                                                                    // Terminal formatting function; Line 9 Column 45.

/***********************************************************************************************************************************************************
  Function:
    void setup()

  Remarks:
    - Arduino, ADC, and Serial Port Configuration Functions. 
 */

void setup() 
{
    // put your setup code here, to run once:

    //ATSAMD21G18 SPI Configuration for Arduino Zero Board
    pinMode(PIN_SPI_nCS, OUTPUT);                                                               // nCS Pin is an output. See "MCP3x6x_SPI_Definitions.h."
                                                                                                // SCK, SDO and SDI Pins are configured via SPI.begin(). 

    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));                           // SPI Bus is 10MHz, MSb-First, Mode 0,0.  
    SPI.begin();                                                                                // SPI SCK and SDO Pins are Outputs.
                                                                                                // SPI SDI Pin is an Input.
    //MCP3564 Delta-Sigma Configuration
    MCP3x6x_CONFIG();                                                                           // MCP3564 Register Configuration.
}

/***********************************************************************************************************************************************************
  Function:
    void loop()

  Remarks:
    - Arduino Main-Loop Function. 
 */

void loop() 
{
    // put your main code here, to run repeatedly:

}

/**********************************************************************************************************************************************************
 End of File
 */

