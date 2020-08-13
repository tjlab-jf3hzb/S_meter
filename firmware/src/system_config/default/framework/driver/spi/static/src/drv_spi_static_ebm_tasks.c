/*******************************************************************************
  SPI Driver Functions for Static Enhanced Buffer Driver Tasks Functions

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static_ebm_tasks.c

  Summary:
    SPI driver tasks functions

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file contains implemenation
    for the SPI driver.

  Remarks:
  This file is generated from framework/driver/spi/template/drv_spi_static_ebm_tasks.c.ftl
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
//DOM-IGNORE-END

#include "system_config.h"
#include "system_definitions.h"
#include "driver/spi/static/src/drv_spi_static_local.h"
int32_t DRV_SPI0_MasterEBMSend8BitPolled( struct DRV_SPI_OBJ * pDrvObj )
{
    register DRV_SPI_JOB_OBJECT * currentJob = pDrvObj->currentJob;

    /* Determine the maximum number of bytes we can send to the FIFO*/
        uint8_t symbolsInTransit = MAX(pDrvObj->symbolsInProgress, PLIB_SPI_FIFOCountGet(SPI_ID_1, SPI_FIFO_TYPE_TRANSMIT));
        uint8_t bufferBytes = PLIB_SPI_TX_8BIT_FIFO_SIZE(SPI_ID_1) - symbolsInTransit;
    /* Figure out how much data we can send*/
    size_t dataUnits = MIN(currentJob->dataLeftToTx, bufferBytes);

    size_t counter;

    if (dataUnits != 0)
    {
        /* Adjust the maximum buffer size downwards based on how much data we'll be sending*/
        bufferBytes -= dataUnits;
        currentJob->dataLeftToTx -= dataUnits;
        /* Set the location in the buffer of where to start sending from*/
        uint8_t *bufferLoc = &(currentJob->txBuffer[currentJob->dataTxed]);
        for (counter = 0; counter < dataUnits; counter++)
        {
            /* Send a unit/symbol of data*/
            PLIB_SPI_BufferWrite(SPI_ID_1, bufferLoc[counter]);
        }
        /* Update the number of bytes transmitted*/
        currentJob->dataTxed += dataUnits;
        /* Adjust the symbols in progress */
        pDrvObj->symbolsInProgress += dataUnits;
    }
    size_t dummyUnits = MIN(currentJob->dummyLeftToTx, bufferBytes);
    if (dummyUnits != 0)
    {
        currentJob->dummyLeftToTx -= dummyUnits;

        /* Adjust the symbols in progress */
        pDrvObj->symbolsInProgress += dummyUnits;
        
        for (counter = 0; counter < dummyUnits; counter++)
        {
            PLIB_SPI_BufferWrite(SPI_ID_1, (uint8_t)pDrvObj->dummyByteValue);
        }
    }
    return 0;
}

int32_t DRV_SPI0_MasterEBMReceive8BitPolled( struct DRV_SPI_OBJ * pDrvObj )
{
    register DRV_SPI_JOB_OBJECT * currentJob = pDrvObj->currentJob;

    /* Figure out how many bytes are waiting to be received."*/
    uint8_t bufferBytes = PLIB_SPI_FIFOCountGet(SPI_ID_1, SPI_FIFO_TYPE_RECEIVE);
    /* Calculate the maximum number of data bytes that can be received*/
    size_t dataUnits = MIN(currentJob->dataLeftToRx, bufferBytes);
    size_t counter;

    if (dataUnits != 0)
    {
        bufferBytes -= dataUnits;
        currentJob->dataLeftToRx -= dataUnits;
        /* Set the buffer location to receive bytes from the SPI to*/
        uint8_t *bufferLoc = &(currentJob->rxBuffer[currentJob->dataRxed]);
        for (counter = 0; counter < dataUnits; counter++)
        {
            /* Receive the data from the SPI */
            bufferLoc[counter] = PLIB_SPI_BufferRead(SPI_ID_1);
        }
        /* Adjust the amount of data that has been received */
        currentJob->dataRxed += dataUnits;
        /* Update the symbols in progress so we can send more units later */
        pDrvObj->symbolsInProgress -= dataUnits;
    }

    /* Figure out the maximum number of dummy data to be received */
    size_t dummyUnits = MIN(currentJob->dummyLeftToRx, bufferBytes);
    if (dummyUnits != 0)
    {
        /* Lower the number of dummy bytes to be received */
        currentJob->dummyLeftToRx -= dummyUnits;
        pDrvObj->symbolsInProgress -= dummyUnits;
        for (counter = 0; counter < dummyUnits; counter++)
        {
            /* Receive and throw away the byte.  Note: We cannot just clear the
               buffer because we have to keep track of how many symbols/units we
               have received, and the number may have increased since we checked
               how full the buffer is.*/
            PLIB_SPI_BufferRead(SPI_ID_1);
        }
    }

    return 0;
}

