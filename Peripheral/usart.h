
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* USER CODE BEGIN Includes */
#define UART_Rx_BUFF_SIZE 1024
#define UART_Tx_BUFF_SIZE 256
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef struct
{
  volatile uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  volatile uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  volatile uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  volatile uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  volatile uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  volatile uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  volatile uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_Register_Struct;

typedef struct								   
{	
		uint16_t  ucBufSize;							
		uint16_t  ucBufWrInde;					
		uint16_t  ucBufRdInde;						
		uint16_t  ucBufCnt;						
		uint8_t  ucBufOvf;							
		uint8_t  * pcBufAddr;						
} USART_Buf_TypeDef;
/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
extern uint8_t UART_RxBuff3[UART_Rx_BUFF_SIZE];
extern USART_Buf_TypeDef UART_RxBuff_Struct3;
extern uint8_t UART_TxBuff3[UART_Tx_BUFF_SIZE];
extern USART_Buf_TypeDef UART_TxBuff_Struct3;

extern uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum);
extern void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum);
extern void UART_PutChar(USART_Register_Struct *usart, USART_Buf_TypeDef * ptTx, uint8_t ucData);
extern void UART_SendData(USART_Register_Struct *usart, USART_Buf_TypeDef * ptTx, uint8_t* Data, uint8_t Length);
extern void USARTx_Rx_IRQ(USART_Register_Struct *usart, USART_Buf_TypeDef *USARTx_RxBuf_Struct);
extern void USARTx_Tx_IRQ(USART_Register_Struct *usart, USART_Buf_TypeDef *USARTx_TxBuf_Struct);
/* USER CODE END Prototypes */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
