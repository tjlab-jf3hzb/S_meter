/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

#include "display.h"
#include "graph.h"
#include <stdio.h>

char s[64];

#define Nmav 32
ADC_SAMPLE v0[Nmav+1], v1[Nmav+1];
int pav=0;


#define SEL_TEXT0    PORTBbits.RB3
#define SEL_TEXT1    PORTAbits.RA2

#define SEL_IMG0     PORTAbits.RA3
#define SEL_IMG1     PORTBbits.RB4
#define SEL_IMG2     PORTAbits.RA4

#define SEL_ND0      PORTBbits.RB5
#define SEL_ND1      PORTBbits.RB7

#define SEL_NDWidth  PORTBbits.RB2

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
                DRV_ADC_ChannelScanInputsAdd( ADC_INPUT_SCAN_AN3 | ADC_INPUT_SCAN_AN2 );
                //AD1CSSL=0b0000000000110000;
                DRV_ADC_Open();                
                DRV_ADC_Start();               
                display_init();
                                
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }
        

        case APP_STATE_SERVICE_TASKS:
        {      
            /*------- Noise Rejection -----------*/
            #define Nav 512
            ADC_SAMPLE vt0=0, vt1=0, vt;
            int k; 
            for(k=0; k<Nav; k++){
                vt0+=DRV_ADC_SamplesRead(0);
                vt1+=DRV_ADC_SamplesRead(1);
                wait_1us(10);
            }
            
            /*--------------  Mav ----------------*/
            pav++; pav%=Nmav;

            if(vt0>v0[Nmav]){
                for(k=0; k<Nmav; k++) v0[k]=vt0;
            }else{
                v0[pav]=vt0;
            }
            
            if(vt1>v1[Nmav]){
                for(k=0; k<Nmav; k++) v1[k]=vt1;
            }else{
                v1[pav]=vt1;
            }
            
            v0[Nmav]=0; v1[Nmav]=0;            
            for(k=0; k<Nmav; k++){
                v0[Nmav]+=v0[k];
                v1[Nmav]+=v1[k];
            }
            vt0=v0[Nmav]/Nmav; vt1=v1[Nmav]/Nmav;             
            v0[Nmav]=vt0; v1[Nmav]=vt1;          
            
            GRAM_clr();
            
            
            char img_ID, str_ID, nd_ID;
            img_ID=(SEL_IMG0^1) | (SEL_IMG1^1)<<1 | (SEL_IMG2^1)<<2 ;
            str_ID=(SEL_TEXT0^1) | (SEL_TEXT1^1)<<1;
            nd_ID=(SEL_ND0^1) | (SEL_ND1^1)<<1;
            
            if(str_ID==0) sprintf(s, ""); 
            if(str_ID==1) sprintf(s, "S/RF");
            if(str_ID==2) sprintf(s, "Unit");
            if(str_ID==3) sprintf(s, " S  ");
            
            Copy_img(img_ID, s);
            
            /*--------- Draw Meter Needle ------------*/
            if(vt1>vt0) vt=vt1; else vt=vt0;
            
            if(nd_ID==0) needle(vt, 1024*Nav, SEL_NDWidth^1, 0xff0000);
            if(nd_ID==1) needle(vt, 1024*Nav, SEL_NDWidth^1, 0x1010c0);
            if(nd_ID==2) needle(vt, 1024*Nav, SEL_NDWidth^1, 0xff4000);
            if(nd_ID==3) needle(vt, 1024*Nav, SEL_NDWidth^1, 0x004000);
            
            Transfer_Image();
            
            
            
            break;
        }
        /* TODO: implement your application state machine.*/
        
        
        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
