#ifndef __CRC_CAL_H
#define __CRC_CAL_H
#include <stdint.h>

extern uint32_t CRC32_Calculate(uint8_t *pData, uint16_t Length);
extern uint16_t CRC16_Xmodem(uint8_t *pData, uint16_t Length);
#endif
