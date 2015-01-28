#include "stm8s.h"
#include "iostm8s003f3.h"
#include "uart.h"
#include "gpio.h"
#include "string.h"

unsigned char Uart_Rx_Buf[256];
unsigned char KeypressFlag = 0;
unsigned char uatr_rx_count = 0;
uint8_t  Rec_OK = 0;

unsigned char FlashFlag = 0;

void UART1_SendByte(char Data)
{
    while(!UART1_GetFlagStatus(UART1_FLAG_TXE));
    UART1_SendData8(Data);      
}
void UART1_Send(char *buf,uint8_t len)
{
    unsigned char i;
    
    for(i=0;i<len;i++)
    {
        UART1_SendData8(buf[i]);
    }
    while(!UART1_GetFlagStatus(UART1_FLAG_TC));
}

void Uart_RxPacket(void)
{
    char i = 0;
    unsigned char buf[256];
    
    if(Rec_OK)
    {
      
        Rec_OK = 0;
        
        {
            for(i=0;i<uatr_rx_count-7;i++)
            {
                buf[i] = Uart_Rx_Buf[i+DataBase];
            }
            uatr_rx_count = 0;
            Uart_Rx_Buf[0] = 0;
            
            switch(Uart_Rx_Buf[DataCMD])
            {
                case Ask_Type:
                {
                    Send_Type();
                    break;
                }
                case Data_Type:
                {
                    if((Uart_Rx_Buf[DataLen]== 5) && (Uart_Rx_Buf[DataBase] == 'S'))
                    {
                        if(memcmp("STOP",buf,4) == 0)
                        {
                            FlashFlag = 0;
                        }
                    }
                    else if((Uart_Rx_Buf[DataLen]== 6) && (Uart_Rx_Buf[DataBase] == 'F'))
                    {
                        if(memcmp("FLASH",buf,5) == 0)
                        {
                            FlashFlag = 1;
                        }
                    }
                    else 
                    {
                        FlashFlag = 0;
                    } 
                    break;
                }            
                default :break;
            }
             
        }        
    }
}

void Send_Type(void)
{
    UART1_SendByte(HEAD_ONE);
    UART1_SendByte(HEAD_TWO);
    UART1_SendByte(DATA_LENH);
    UART1_SendByte(3);
    UART1_SendByte(Answer_Type);
    UART1_SendByte(0);
    UART1_SendByte(0);
    UART1_SendByte(END_ONE);
    UART1_SendByte(END_TWO);  
}

void Deal_Key(void)
{
    char buf[32];
    
    if(KeypressFlag)
    {
        memcpy(buf,"Hello_World!",12);
        KeypressFlag = 0;
        UART1_SendByte(HEAD_ONE);
        UART1_SendByte(HEAD_TWO);
        UART1_SendByte(DATA_LENH);
        UART1_SendByte(13);
        UART1_SendByte(Data_Type);
        UART1_Send(buf,12);
        
        UART1_SendByte(END_ONE);
        UART1_SendByte(END_TWO);
    }
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler,18)
{ 
    if(UART1_GetITStatus(UART1_IT_RXNE) == SET)
    {
        Uart_Rx_Buf[uatr_rx_count] = UART1_ReceiveData8();  
    }
    if(uatr_rx_count == 1)
    {
        if(Uart_Rx_Buf[1] != HEAD_TWO)
        {
            uatr_rx_count = 0;
            Uart_Rx_Buf[0] = Uart_Rx_Buf[1];
        }
    }
    if(Uart_Rx_Buf[0] == HEAD_ONE)
    {
        uatr_rx_count ++;
    }
    else 
    {
        uatr_rx_count = 0;
    }
    
    if(uatr_rx_count > 7)
    {
        if((Uart_Rx_Buf[uatr_rx_count - 2] == END_ONE) && (Uart_Rx_Buf[uatr_rx_count - 1] == END_TWO))
        {
            Rec_OK = 0xff;
        }
    }  
    if(uatr_rx_count >= 255)
    {
        uatr_rx_count = 0;
    }
    
    UART1_ClearITPendingBit(UART1_IT_RXNE);
}


INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler,5)
{
    if(KEY_IRQ_IN)
    {
        KeypressFlag = 0xff;
        LED1_TRIG;
    }
}



