

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "usart.h"


extern void Uart_RxData_Process(USART_Buf_TypeDef* ptUartRx, uint32_t currentTimeCount, uint16_t TimeOut);
extern void SendUartData(USART_Register_Struct *usart, USART_Buf_TypeDef * ptTx, uint16_t IDnum,uint8_t Mode, uint16_t Command, uint8_t* Data);







