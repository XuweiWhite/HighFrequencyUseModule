
/* Includes ------------------------------------------------------------------*/
#include "usart_process.h"
#include "crc_cal.h"

__weak void UART_DataProcess(uint16_t id, uint8_t mode, uint16_t command, uint8_t *data)
{
	
}

//�������ݽ���
void Uart_RxData_Process(USART_Buf_TypeDef* ptUartRx, uint32_t currentTimeCount, uint16_t TimeOut)
{
  uint8_t FrameBegin1, FrameBegin2, Mode, CmdLength, DataLength, FrameEnd;
	uint16_t Cmd, i, ID;
	uint32_t CrcResult, CrcData;
	static uint32_t DelayTimeCnt = 0;
	static bool IsWaitRX_Flag = false;
	static 	uint8_t	Data[255];
	
	if(ptUartRx->ucBufCnt >= 13)
	{	  
		FrameBegin1 = UART_ReadChar(ptUartRx, 0);		//��ȡ֡ͷ1
		FrameBegin2 = UART_ReadChar(ptUartRx, 1);		//��ȡ֡ͷ2
		ID = (UART_ReadChar(ptUartRx, 2) << 8) + UART_ReadChar(ptUartRx, 3);		//��ȡID
		Mode = UART_ReadChar(ptUartRx, 4);					//��ȡ֡ģʽ					 
		CmdLength = UART_ReadChar(ptUartRx, 5);			//��ȡ����γ���
		Cmd = (UART_ReadChar(ptUartRx, 6) << 8) + UART_ReadChar(ptUartRx, 7); 	//��ȡ������
		DataLength = UART_ReadChar(ptUartRx, 7);		//��ȡ���ݶγ���
		if((FrameBegin1 == 0x55) && (FrameBegin2 == 0xAA)) 
		{ 	
			if((Mode == 0x11) || (Mode == 0x16) || (Mode == 0x0C))
			{
				if((CmdLength - DataLength) == 2)
				{
					if(ptUartRx->ucBufCnt < (CmdLength + 11))//֡ͷ2bytes  + ID2bytes ģʽ1byte + ����γ���1byte + У��λ4bytes + ֡β1byte
					{
					  if(IsWaitRX_Flag == false)
						{
						  DelayTimeCnt = currentTimeCount;
							IsWaitRX_Flag = true;
						}
						if((currentTimeCount - DelayTimeCnt) > TimeOut)//��ʱ����λms
						{
						  UART_DelChar(ptUartRx, ptUartRx->ucBufCnt);
							DelayTimeCnt = currentTimeCount;
							IsWaitRX_Flag = false;
						}
						return;
					}
					else
					{					  
						IsWaitRX_Flag = false;
						FrameEnd = UART_ReadChar(ptUartRx, 12 + DataLength);	//��ȡ֡β
						if(FrameEnd == 0xF0)
						{
							for(i=0; i<(CmdLength + 11); i++)//��ȡ���ݶ�
							{
								Data[i] = UART_ReadChar(ptUartRx, i);
							}
							CrcData = (UART_ReadChar(ptUartRx, 8 + DataLength) << 24) + \
												(UART_ReadChar(ptUartRx, 9 + DataLength) << 16) + \
												(UART_ReadChar(ptUartRx, 10 + DataLength) << 8) + \
												 UART_ReadChar(ptUartRx, 11 + DataLength);
							CrcResult = CRC32_Calculate(Data, 8 + DataLength);
							if((CrcData - CrcResult) == 0) // �Ƚ�У��
							{
								//���ݴ���
								UART_DataProcess(ID, Mode, Cmd, &Data[8]);//ModeΪ֡ģʽ��CmdΪ�����֣�DataΪ���ݶ�
								//�������
								UART_DelChar(ptUartRx, CmdLength + 11);
							}
							else UART_DelChar(ptUartRx, 1);
						}
						else UART_DelChar(ptUartRx, 1);
					}
				}
				else UART_DelChar(ptUartRx, 1);
			}
			else UART_DelChar(ptUartRx, 1);
		}
		else UART_DelChar(ptUartRx, 1);		
	}
}

//��������
void SendUartData(USART_Register_Struct *usart, USART_Buf_TypeDef * ptTx, uint16_t IDnum,uint8_t Mode, uint16_t Command, uint8_t* Data)
{
  uint8_t SendBuf[150] = {0};
	uint32_t CRC_Result = 0x00000000;
	uint8_t DataLength;
	
	DataLength = (uint8_t)(Command & 0xFF);
	SendBuf[0] = 0x55;
	SendBuf[1] = 0xAA;
	SendBuf[2] = (uint8_t)((IDnum >> 8) & 0xFF);
	SendBuf[3] = (uint8_t)(IDnum & 0xFF);;
	SendBuf[4] = Mode;
	SendBuf[5] = DataLength + 2;
	SendBuf[6] = (uint8_t)((Command >> 8) & 0xFF);
	SendBuf[7] = DataLength;
	memcpy(SendBuf + 8, Data, DataLength);
	CRC_Result = CRC32_Calculate(SendBuf, DataLength + 8);
	
	SendBuf[8 + DataLength] = (uint8_t)((CRC_Result >> 24) & 0xFF);
	SendBuf[9 + DataLength] = (uint8_t)((CRC_Result >> 16) & 0xFF);
	SendBuf[10 + DataLength] = (uint8_t)((CRC_Result >> 8) & 0xFF);
	SendBuf[11 + DataLength] = (uint8_t)(CRC_Result & 0xFF);
	 
	SendBuf[12 + DataLength] = 0xF0;
	
	UART_SendData(usart, ptTx, SendBuf, DataLength + 13);
}






