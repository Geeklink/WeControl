#ifndef __GPIO_H__
#define __GPIO_H__




#define TX_IO          (uint8_t)5
#define RX_IO          (uint8_t)6


#define LED1_OFF         GPIO_WriteHigh(GPIOA,GPIO_PIN_1)
#define LED1_ON          GPIO_WriteLow(GPIOA,GPIO_PIN_1)
#define LED1_TRIG        GPIO_WriteReverse(GPIOA,GPIO_PIN_1)

#define LED2_OFF         GPIO_WriteHigh(GPIOA,GPIO_PIN_2)
#define LED2_ON          GPIO_WriteLow(GPIOA,GPIO_PIN_2)
#define LED2_TRIG        GPIO_WriteReverse(GPIOA,GPIO_PIN_2)

#define LED3_OFF         GPIO_WriteHigh(GPIOA,GPIO_PIN_3)
#define LED3_ON          GPIO_WriteLow(GPIOA,GPIO_PIN_3)
#define LED3_TRIG        GPIO_WriteReverse(GPIOA,GPIO_PIN_3)

#define KEY_IRQ_IN      PC_IDR_IDR3


extern void Init_GPIO(void);


#endif /* __GPIO_H */