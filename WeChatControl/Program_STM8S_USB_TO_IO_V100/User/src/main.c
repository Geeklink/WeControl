#include "stm8s.h"
#include "iostm8s003f3.h"
#include "gpio.h"
#include "uart.h"
#include "stm8s_flash.h"
#include "stm8s_tim4.h"

unsigned char Tim4Counter = 0;
unsigned char Flash_count = 1;

void Read_Protect_Flash(void)
{
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);    
    while(FLASH_ReadOptionByte(0x4800) != 0xAA)
    {
        FLASH_Unlock(FLASH_MEMTYPE_DATA);
        
        FLASH_EraseOptionByte(0x4800);
        FLASH_ProgramOptionByte(0x4800,0xAA);
        
        FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
	
}

void Timer4_Init(void)
{
    TIM4_TimeBaseInit(TIM4_PRESCALER_128,255);//128PRESCALER,2ms
    TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);
    TIM4_Cmd(ENABLE);  
}



void IWDG_Init(void)
{
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(0xff);      //1.02S
  IWDG_ReloadCounter();
  IWDG_Enable(); 
}


void LED_Flash(void)
{
    if(FlashFlag)
    {
        switch(Flash_count)
        {
            case 1:
            {
                LED1_ON;LED2_OFF;LED3_OFF;
                Flash_count = 2;
                break;
            }
            case 2:
            {
                LED1_OFF;LED2_ON;LED3_OFF;
                Flash_count = 3;
                break;
            }
            case 3:
            {
                LED1_OFF;LED2_OFF;LED3_ON;
                Flash_count = 4;
                break;
            }
            case 4:
            {
                LED1_ON;LED2_ON;LED3_ON;
                Flash_count = 5;
                break;
            }
            case 5:
            {
                LED1_OFF;LED2_OFF;LED3_OFF;
                Flash_count = 1;
                break;
            }
            default :
            {
                break; 
            }
        }
    }
    else 
    {
        LED1_OFF;LED2_OFF;LED3_ON;
    }
}



void main(void)
{
    
    CLK_HSICmd(ENABLE); //enable HSI

    sim();
    
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//0 PRESCALER£¬16MHZ
    
//    Read_Protect_Flash();   
    Init_GPIO();
  
    UART1_Init(115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE); 
    UART1_ITConfig(UART1_IT_RXNE,ENABLE);
    
    IWDG_Init();
    rim();
    
    Timer4_Init();
    
    LED1_OFF;
    LED2_OFF;
    LED3_ON;
       
    
    while(1)
    {  
      
        IWDG_ReloadCounter();// reload watchdog
        
        Uart_RxPacket();
        
        Deal_Key();
        
        if(Tim4Counter >= 200)
        {
            Tim4Counter = 0;
            LED_Flash();
        }
               
    }    
}


INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
//  TIM4_SR_UIF = 0;
    Tim4Counter ++;
}
