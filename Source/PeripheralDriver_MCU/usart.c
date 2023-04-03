
/* Includes ------------------------------------------------------------------*/
#include "usart.h"


/* USER CODE BEGIN 0 */
uint8_t UART_RxBuff3[UART_Rx_BUFF_SIZE];
USART_Buf_TypeDef UART_RxBuff_Struct3 = {UART_Rx_BUFF_SIZE,0,0,0,0,UART_RxBuff3};
uint8_t UART_TxBuff3[UART_Tx_BUFF_SIZE];
USART_Buf_TypeDef UART_TxBuff_Struct3 = {UART_Tx_BUFF_SIZE,0,0,0,0,UART_TxBuff3};
/* USER CODE END 0 */


/* USER CODE BEGIN 1 */
uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint8_t ucData;
	uint16_t  i;
	i = ucNum;
	if ((*ptRx).ucBufCnt > ucNum)
	{
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{	
			i -=((*ptRx).ucBufSize);
		}
		ucData = *((*ptRx).pcBufAddr + i);
	}
	else
	{
	  ucData=0;
	}
	
	return ucData;
}

void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint16_t i;
	if ((*ptRx).ucBufCnt >= ucNum)	
	{	
		i = ucNum;
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{
		  i -= (*ptRx).ucBufSize;
		}
		(*ptRx).ucBufRdInde = i;
		(*ptRx).ucBufCnt -= ucNum;
	}
	else
	{  
		(*ptRx).ucBufRdInde = (*ptRx).ucBufWrInde;
    (*ptRx).ucBufCnt = (*ptRx).ucBufWrInde - (*ptRx).ucBufRdInde;	
	}
}

void UART_PutChar(USART_Register_Struct *usart, USART_Buf_TypeDef * ptTx, uint8_t ucData)
{	
	while ((*ptTx).ucBufCnt == (*ptTx).ucBufSize);			 //ï¿½ï¿½ï¿½Í»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ó»º³ï¿½ï¿½ï¿?
	usart->CR1 &= ~(0x00000080);                         //disopen interrupt TXEIE
	if((*ptTx).ucBufCnt == 0)
	{
		if((usart->SR & 0x00000080) == 0x00000080) //check if transmit data register full or not
		{
			usart->DR = ucData;
			usart->CR1 |= (0x00000080);                         //open interrupt TXEIE
			return;
		}
	}	
	*((*ptTx).pcBufAddr + (*ptTx).ucBufWrInde) = ucData;
	if(++(*ptTx).ucBufWrInde == (*ptTx).ucBufSize)
	{
	  (*ptTx).ucBufWrInde = 0;
	}
	++(*ptTx).ucBufCnt;
	usart->CR1 |= (0x00000080);                         //´ò¿ª·¢ËÍÖÐ¶Ï TXEIE
}

void UART_SendData(USART_Register_Struct *usart, USART_Buf_TypeDef * ptTx, uint8_t* Data, uint8_t Length)
{
  uint8_t i;
	
	for(i=0; i<Length; i++)
	{
	  UART_PutChar(usart, ptTx, Data[i]);
	}
}

void USARTx_Rx_IRQ(USART_Register_Struct *usart, USART_Buf_TypeDef *USARTx_RxBuf_Struct)
{
	uint8_t ucData;	
	if(((usart->SR & 0x00000020) == 0x00000020) && ((usart->CR1 & 0x00000020) == 0x00000020))
	{
		usart->SR &= ~(0x00000020);
		
		ucData = (uint8_t)(usart->DR & 0x01FF);
		*((* USARTx_RxBuf_Struct).pcBufAddr + (* USARTx_RxBuf_Struct).ucBufWrInde) = ucData;         
		if(++(* USARTx_RxBuf_Struct).ucBufWrInde >= (* USARTx_RxBuf_Struct).ucBufSize)
		{
				(* USARTx_RxBuf_Struct).ucBufWrInde = 0; 
		}	
		if(++(* USARTx_RxBuf_Struct).ucBufCnt > (* USARTx_RxBuf_Struct).ucBufSize)  
		{
				(* USARTx_RxBuf_Struct).ucBufCnt = (* USARTx_RxBuf_Struct).ucBufSize;
				(* USARTx_RxBuf_Struct).ucBufRdInde = (* USARTx_RxBuf_Struct).ucBufSize - (* USARTx_RxBuf_Struct).ucBufWrInde;
				(* USARTx_RxBuf_Struct).ucBufOvf = 1;
		}
	}
}

void USARTx_Tx_IRQ(USART_Register_Struct *usart, USART_Buf_TypeDef *USARTx_TxBuf_Struct)
{
	if(((usart->SR & 0x00000080) == 0x00000080) && ((usart->CR1 & 0x00000080) == 0x00000080))        //TXEIE interrupt enable and TXE Flag set
	{    
		usart->SR &= ~(0x00000080);
		if((* USARTx_TxBuf_Struct).ucBufCnt)
		{
			--(* USARTx_TxBuf_Struct).ucBufCnt;
			usart->DR = *(USARTx_TxBuf_Struct->pcBufAddr + USARTx_TxBuf_Struct->ucBufRdInde);		
			if(++(* USARTx_TxBuf_Struct).ucBufRdInde >= (* USARTx_TxBuf_Struct).ucBufSize)
			{
					(* USARTx_TxBuf_Struct).ucBufRdInde = 0;
			}
		}
		else
		{
			/* Disable the UART Transmit Data Register Empty Interrupt */
			usart->CR1 &= ~(0x00000080);                         //¹Ø±Õ·¢ËÍÖÐ¶Ï TXEIE
		}
	} 
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
