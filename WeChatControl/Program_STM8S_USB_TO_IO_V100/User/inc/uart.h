#ifndef __UART_H__
#define __UART_H__

#define HEAD_ONE        0xAA
#define HEAD_TWO        0xBB
#define DATA_LENH       0

#define END_ONE         0xAB
#define END_TWO         0xBA

#define DataBase        5
#define DataLen         3
#define DataCMD         4


#define Answer_Type        0x11
#define Ask_Type        0x10
#define Data_Type       0x21



extern void Send_Type(void);
extern unsigned char FlashFlag;
extern void UART1_SendByte(char Data);
extern void UART1_Send(char *buf,uint8_t len);
extern void Uart_RxPacket(void);
extern void Deal_Key(void);

#endif /* __UART_H */