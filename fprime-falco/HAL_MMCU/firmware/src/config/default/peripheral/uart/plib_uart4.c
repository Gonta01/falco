/*******************************************************************************
  UART4 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart4.c

  Summary:
    UART4 PLIB Implementation File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#include "device.h"
#include "plib_uart4.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART4 Implementation
// *****************************************************************************
// *****************************************************************************

volatile static UART_RING_BUFFER_OBJECT uart4Obj;

#define UART4_READ_BUFFER_SIZE      1024U
/* Disable Read, Overrun, Parity and Framing error interrupts */
#define UART4_RX_INT_DISABLE()      UART4_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);
/* Enable Read, Overrun, Parity and Framing error interrupts */
#define UART4_RX_INT_ENABLE()       UART4_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

volatile static uint8_t UART4_ReadBuffer[UART4_READ_BUFFER_SIZE];

#define UART4_WRITE_BUFFER_SIZE     2U
#define UART4_TX_INT_DISABLE()      UART4_REGS->UART_IDR = UART_IDR_TXEMPTY_Msk;
#define UART4_TX_INT_ENABLE()       UART4_REGS->UART_IER = UART_IER_TXEMPTY_Msk;

volatile static uint8_t UART4_WriteBuffer[UART4_WRITE_BUFFER_SIZE];

void UART4_Initialize( void )
{
    /* Reset UART4 */
    UART4_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART4 */
    UART4_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    /* Configure UART4 mode */
    UART4_REGS->UART_MR = ((UART_MR_BRSRCCK_PERIPH_CLK) | (UART_MR_PAR_NO) | (0U << UART_MR_FILTER_Pos));

    /* Configure UART4 Baud Rate */
    UART4_REGS->UART_BRGR = UART_BRGR_CD(81);

    /* Initialize instance object */
    uart4Obj.rdCallback = NULL;
    uart4Obj.rdInIndex = 0;
    uart4Obj.rdOutIndex = 0;
    uart4Obj.isRdNotificationEnabled = false;
    uart4Obj.isRdNotifyPersistently = false;
    uart4Obj.rdThreshold = 0;
    uart4Obj.wrCallback = NULL;
    uart4Obj.wrInIndex = 0;
    uart4Obj.wrOutIndex = 0;
    uart4Obj.isWrNotificationEnabled = false;
    uart4Obj.isWrNotifyPersistently = false;
    uart4Obj.wrThreshold = 0;

    /* Enable receive interrupt */
    UART4_RX_INT_ENABLE()
}

bool UART4_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t baud;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if (setup != NULL)
    {
        baud = setup->baudRate;

        if(srcClkFreq == 0U)
        {
            srcClkFreq = UART4_FrequencyGet();
        }

        /* Calculate BRG value */
        brgVal = srcClkFreq / (16U * baud);

        /* If the target baud rate is acheivable using this clock */
        if (brgVal <= 65535U)
        {
            /* Configure UART4 mode */
            uartMode = UART4_REGS->UART_MR;
            uartMode &= ~UART_MR_PAR_Msk;
            UART4_REGS->UART_MR = uartMode | setup->parity ;

            /* Configure UART4 Baud Rate */
            UART4_REGS->UART_BRGR = UART_BRGR_CD(brgVal);

            status = true;
        }
    }

    return status;
}

static void UART4_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    UART4_REGS->UART_CR = UART_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( UART_SR_RXRDY_Msk == (UART4_REGS->UART_SR & UART_SR_RXRDY_Msk) )
    {
        dummyData = (uint8_t)(UART4_REGS->UART_RHR & UART_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

UART_ERROR UART4_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = UART4_REGS->UART_SR;

    errors = (UART_ERROR)(status & (UART_SR_OVRE_Msk | UART_SR_PARE_Msk | UART_SR_FRAME_Msk));

    if(errors != UART_ERROR_NONE)
    {
        UART4_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static inline bool UART4_RxPushByte(uint8_t rdByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;

    tempInIndex = uart4Obj.rdInIndex + 1U;

    if (tempInIndex >= UART4_READ_BUFFER_SIZE)
    {
        tempInIndex = 0U;
    }

    if (tempInIndex == uart4Obj.rdOutIndex)
    {
        /* Queue is full - Report it to the application. Application gets a chance to free up space by reading data out from the RX ring buffer */
        if(uart4Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = uart4Obj.rdContext;

            uart4Obj.rdCallback(UART_EVENT_READ_BUFFER_FULL, rdContext);
        }

        /* Read the indices again in case application has freed up space in RX ring buffer */
        tempInIndex = uart4Obj.rdInIndex + 1U;

        if (tempInIndex >= UART4_READ_BUFFER_SIZE)
        {
            tempInIndex = 0U;
        }

    }

    if (tempInIndex != uart4Obj.rdOutIndex)
    {
        uint32_t rdInIndex = uart4Obj.rdInIndex;

        UART4_ReadBuffer[rdInIndex] = rdByte;
        uart4Obj.rdInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Data will be lost. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void UART4_ReadNotificationSend(void)
{
    uint32_t nUnreadBytesAvailable;

    if (uart4Obj.isRdNotificationEnabled == true)
    {
        nUnreadBytesAvailable = UART4_ReadCountGet();

        if(uart4Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = uart4Obj.rdContext;

            if (uart4Obj.isRdNotifyPersistently == true)
            {
                if (nUnreadBytesAvailable >= uart4Obj.rdThreshold)
                {
                    uart4Obj.rdCallback(UART_EVENT_READ_THRESHOLD_REACHED, rdContext);
                }
            }
            else
            {
                if (nUnreadBytesAvailable == uart4Obj.rdThreshold)
                {
                    uart4Obj.rdCallback(UART_EVENT_READ_THRESHOLD_REACHED, rdContext);
                }
            }
        }
    }
}

size_t UART4_Read(uint8_t* pRdBuffer, const size_t size)
{
    size_t nBytesRead = 0;
    uint32_t rdOutIndex;
    uint32_t rdInIndex;

    while (nBytesRead < size)
    {
        UART4_RX_INT_DISABLE();

        rdOutIndex = uart4Obj.rdOutIndex;
        rdInIndex = uart4Obj.rdInIndex;

        if (rdOutIndex != rdInIndex)
        {
            pRdBuffer[nBytesRead] = UART4_ReadBuffer[rdOutIndex];
            nBytesRead++;
            uart4Obj.rdOutIndex++;

            if (uart4Obj.rdOutIndex >= UART4_READ_BUFFER_SIZE)
            {
                uart4Obj.rdOutIndex = 0;
            }
            UART4_RX_INT_ENABLE();
        }
        else
        {
            UART4_RX_INT_ENABLE();
            break;
        }
    }

    return nBytesRead;
}

size_t UART4_ReadCountGet(void)
{
    size_t nUnreadBytesAvailable;
    uint32_t rdInIndex;
    uint32_t rdOutIndex;

    /* Take  snapshot of indices to avoid creation of critical section */
    rdInIndex = uart4Obj.rdInIndex;
    rdOutIndex = uart4Obj.rdOutIndex;

    if ( rdInIndex >=  rdOutIndex)
    {
        nUnreadBytesAvailable =  rdInIndex - rdOutIndex;
    }
    else
    {
        nUnreadBytesAvailable =  (UART4_READ_BUFFER_SIZE -  rdOutIndex) + rdInIndex;
    }

    return nUnreadBytesAvailable;
}

size_t UART4_ReadFreeBufferCountGet(void)
{
    return (UART4_READ_BUFFER_SIZE - 1U) - UART4_ReadCountGet();
}

size_t UART4_ReadBufferSizeGet(void)
{
    return (UART4_READ_BUFFER_SIZE - 1U);
}

bool UART4_ReadNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = uart4Obj.isRdNotificationEnabled;

    uart4Obj.isRdNotificationEnabled = isEnabled;

    uart4Obj.isRdNotifyPersistently = isPersistent;

    return previousStatus;
}

void UART4_ReadThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        uart4Obj.rdThreshold = nBytesThreshold;
    }
}

void UART4_ReadCallbackRegister( UART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    uart4Obj.rdCallback = callback;

    uart4Obj.rdContext = context;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static bool UART4_TxPullByte(uint8_t* pWrByte)
{
    bool isSuccess = false;
    uint32_t wrOutIndex = uart4Obj.wrOutIndex;
    uint32_t wrInIndex = uart4Obj.wrInIndex;

    if (wrOutIndex != wrInIndex)
    {
        *pWrByte = UART4_WriteBuffer[wrOutIndex];
        uart4Obj.wrOutIndex++;

        if (uart4Obj.wrOutIndex >= UART4_WRITE_BUFFER_SIZE)
        {
            uart4Obj.wrOutIndex = 0;
        }
        isSuccess = true;
    }

    return isSuccess;
}

static inline bool UART4_TxPushByte(uint8_t wrByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;

    tempInIndex = uart4Obj.wrInIndex + 1U;

    if (tempInIndex >= UART4_WRITE_BUFFER_SIZE)
    {
        tempInIndex = 0U;
    }
    if (tempInIndex != uart4Obj.wrOutIndex)
    {
        uint32_t wrInIndex = uart4Obj.wrInIndex;

        UART4_WriteBuffer[wrInIndex] = wrByte;
        uart4Obj.wrInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Report Error. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void UART4_WriteNotificationSend(void)
{
    uint32_t nFreeWrBufferCount;

    if (uart4Obj.isWrNotificationEnabled == true)
    {
        nFreeWrBufferCount = UART4_WriteFreeBufferCountGet();

        if(uart4Obj.wrCallback != NULL)
        {
            uintptr_t wrContext = uart4Obj.wrContext;

            if (uart4Obj.isWrNotifyPersistently == true)
            {
                if (nFreeWrBufferCount >= uart4Obj.wrThreshold)
                {
                    uart4Obj.wrCallback(UART_EVENT_WRITE_THRESHOLD_REACHED, wrContext);
                }
            }
            else
            {
                if (nFreeWrBufferCount == uart4Obj.wrThreshold)
                {
                    uart4Obj.wrCallback(UART_EVENT_WRITE_THRESHOLD_REACHED, wrContext);
                }
            }
        }
    }
}

static size_t UART4_WritePendingBytesGet(void)
{
    size_t nPendingTxBytes;

    /* Take a snapshot of indices to avoid creation of critical section */
    uint32_t wrOutIndex = uart4Obj.wrOutIndex;
    uint32_t wrInIndex = uart4Obj.wrInIndex;

    if ( wrInIndex >=  wrOutIndex)
    {
        nPendingTxBytes =  wrInIndex -  wrOutIndex;
    }
    else
    {
        nPendingTxBytes =  (UART4_WRITE_BUFFER_SIZE -  wrOutIndex) + wrInIndex;
    }

    return nPendingTxBytes;
}

size_t UART4_WriteCountGet(void)
{
    size_t nPendingTxBytes;

    nPendingTxBytes = UART4_WritePendingBytesGet();

    return nPendingTxBytes;
}

size_t UART4_Write(uint8_t* pWrBuffer, const size_t size )
{
    size_t nBytesWritten  = 0;

    UART4_TX_INT_DISABLE();

    while (nBytesWritten < size)
    {
        if (UART4_TxPushByte(pWrBuffer[nBytesWritten]) == true)
        {
            nBytesWritten++;
        }
        else
        {
            /* Queue is full, exit the loop */
            break;
        }
    }

    /* Check if any data is pending for transmission */
    if (UART4_WritePendingBytesGet() > 0U)
    {
        /* Enable TX interrupt as data is pending for transmission */
        UART4_TX_INT_ENABLE();
    }

    return nBytesWritten;
}

size_t UART4_WriteFreeBufferCountGet(void)
{
    return (UART4_WRITE_BUFFER_SIZE - 1U) - UART4_WriteCountGet();
}

size_t UART4_WriteBufferSizeGet(void)
{
    return (UART4_WRITE_BUFFER_SIZE - 1U);
}

bool UART4_TransmitComplete(void)
{
    bool TxC = false;
    if(UART_SR_TXEMPTY_Msk == (UART4_REGS->UART_SR & UART_SR_TXEMPTY_Msk))
    {
        TxC = true;
    }

    return TxC;

}

bool UART4_WriteNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = uart4Obj.isWrNotificationEnabled;

    uart4Obj.isWrNotificationEnabled = isEnabled;

    uart4Obj.isWrNotifyPersistently = isPersistent;

    return previousStatus;
}

void UART4_WriteThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        uart4Obj.wrThreshold = nBytesThreshold;
    }
}

void UART4_WriteCallbackRegister( UART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    uart4Obj.wrCallback = callback;

    uart4Obj.wrContext = context;
}

static void __attribute__((used)) UART4_ISR_RX_Handler( void )
{
    /* Keep reading until there is a character availabe in the RX FIFO */
    while(UART_SR_RXRDY_Msk == (UART4_REGS->UART_SR& UART_SR_RXRDY_Msk))
    {
        if (UART4_RxPushByte( (uint8_t )(UART4_REGS->UART_RHR& UART_RHR_RXCHR_Msk) ) == true)
        {
            UART4_ReadNotificationSend();
        }
        else
        {
            /* UART RX buffer is full */
        }
    }
}

static void __attribute__((used)) UART4_ISR_TX_Handler( void )
{
    uint8_t wrByte;

    /* Keep writing to the TX FIFO as long as there is space */
    while(UART_SR_TXRDY_Msk == (UART4_REGS->UART_SR & UART_SR_TXRDY_Msk))
    {
        if (UART4_TxPullByte(&wrByte) == true)
        {
            UART4_REGS->UART_THR |= wrByte;

            /* Send notification */
            UART4_WriteNotificationSend();
        }
        else
        {
            /* Nothing to transmit. Disable the data register empty interrupt. */
            UART4_TX_INT_DISABLE();
            break;
        }
    }
}

void __attribute__((used)) UART4_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (UART4_REGS->UART_SR & (UART_SR_OVRE_Msk | UART_SR_FRAME_Msk | UART_SR_PARE_Msk));

    if(errorStatus != 0U)
    {
        /* Client must call UARTx_ErrorGet() function to clear the errors */

        /* Disable Read, Overrun, Parity and Framing error interrupts */

        UART4_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        /* UART errors are normally associated with the receiver, hence calling
         * receiver callback */
        if( uart4Obj.rdCallback != NULL )
        {
            uintptr_t rdContext = uart4Obj.rdContext;
            uart4Obj.rdCallback(UART_EVENT_READ_ERROR, rdContext);
        }
    }

    /* Receiver status */
    if(UART_SR_RXRDY_Msk == (UART4_REGS->UART_SR & UART_SR_RXRDY_Msk))
    {
        UART4_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(UART_SR_TXRDY_Msk == (UART4_REGS->UART_SR & UART_SR_TXRDY_Msk))
    {
        UART4_ISR_TX_Handler();
    }

}