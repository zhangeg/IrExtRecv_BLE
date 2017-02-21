/*
 *update by zhangengui 2017-02-21
 *brief: UART API
 */
#include "OSAL.h"
#include "npi.h"
#include "simpleUart.h"

static void simple_UART_Callback(uint8 port, uint8 events);
bool simple_UART_Process_Handle(uint8 *pBuffer, uint16 length);

void PrintString(uint8 *str)
{
    NPI_WriteTransport(str, (osal_strlen((char*)str)));
}

void PrintValue(char *title, uint32 value, uint8 format)
{
    uint8 tmpLen;
    uint8 buf[128];
    uint32 err;

    tmpLen = (uint8)osal_strlen((char*)title);
    osal_memcpy(buf, title, tmpLen);
    err = (uint32)(value);
    _ltoa(err, &buf[tmpLen], format);
    PrintString(buf);
}

void simple_UART_init(void)
{
    NPI_InitTransport(simple_UART_Callback);
    //PrintString("123414");
}

static void simple_UART_Callback(uint8 port, uint8 events)
{
    (void)port;

    static uint32 old_time;
    static uint32 old_time_data_len = 0;
    uint32 new_time;
    bool ret;
    uint16 readMaxBytes = UART_MAX_LENGTH;

#if 1
    if (events & HAL_UART_RX_TIMEOUT)
    {
        (void)port;
        uint8 numBytes = 0;

        numBytes = NPI_RxBufLen();
        if(numBytes == 0)
        {
            old_time_data_len = 0;
            return;
        }
        if(old_time_data_len == 0)
        {
            old_time = osal_GetSystemClock();
            old_time_data_len = numBytes;
        }
        else
        {
            new_time = osal_GetSystemClock();
            if( (numBytes >= readMaxBytes) || ( (new_time - old_time) > 20/*ms*/))
            {
                uint8 sendBytes = 0;
                uint8 buffer[UART_MAX_LENGTH] = {0};

                if(numBytes > readMaxBytes)
                {
                    sendBytes = readMaxBytes;
                }
                else
                {
                    sendBytes = numBytes;
                }

                NPI_ReadTransport(buffer, sendBytes);
                if(sendBytes > 2
                   && buffer[sendBytes - 2] == '\r'
                   && buffer[sendBytes - 1] == '\n')
                {
                    ret = simple_UART_Process_Handle(buffer, sendBytes);
                }
                else
                {
                    ret = FALSE;
                }

                if(ret == FALSE)
                {
                }

                old_time = new_time;
                old_time_data_len = numBytes - sendBytes;
            }
        }
    }
#endif
}

bool simple_UART_Process_Handle(uint8 *pBuffer, uint16 length)
{
    bool ret = TRUE;
    bool restart = FALSE;
//    uint8 *pbuf = NULL;

    PrintString("Recv:");
    PrintString(pBuffer);
    PrintString("\r\n");

    if(restart)
    {
        // SerialDelay_1ms(100);
        HAL_SYSTEM_RESET();
    }
    return ret;
}

