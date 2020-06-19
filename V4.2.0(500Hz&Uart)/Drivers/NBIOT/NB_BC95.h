/****************************************************************************
 * @file     NB_BC95.h
 * @brief    NB_IOT模块BC95驱动程序
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par     NB-IOT模块驱动程序
 *  
*****************************************************************************/
#ifndef __NB_BC95_H__ 
#define __NB_BC95_H__

#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_drv_uart.h"
#include "boards.h"
#include "app_gpio_in_out.h"
#include "Parameter.h"
#include <stdlib.h>
#include "Fun.h"
#include "BLE_NRF.h"
#include "app_uart.h"
#include "app_fifo.h"
#include "nrf_drv_uart.h"



#define BC95_RESET_ON  nrf_gpio_pin_set(NB_BC95_RESET_PIN)
#define BC95_RESET_OFF  nrf_gpio_pin_clear(NB_BC95_RESET_PIN)
#define BC95_POWER_OFF  nrf_gpio_pin_set(NB_BC95_POWER_PIN)
#define BC95_POWER_ON   nrf_gpio_pin_clear(NB_BC95_POWER_PIN)

void BC95_GPIO_CONFIG(void);
void BC95_SEND_STRING(uint8_t *str);
char  BC95_Inquire_CEREG(void);
char BC95_InquireCSQ(void);
char  BC95_Set_CEREGReturnFormat(uint8_t format);
void StringToHex(uint8_t *str,uint8_t len,uint8_t *hexdata);
char SetAutoConnect(void);
char  AutoConnectConfig(void);
char BC95_InquireCGATT(void);
uint8_t BC95_CreateSocket(void);
char  BC95_Reset(void);
char  BC95_InquireIMSI(void);
uint8_t BC95_CloseSocket(uint8_t soc);
char BC95_SendDataByUDP(uint8_t soc, char *ip,uint8_t len,uint8_t *data);
void  HexToBCDstring(unsigned char *bcdstr,unsigned char *hexdata,uint8_t len);
char BC95_ReadUDPData(uint8_t soc,uint8_t len);
char  ExtractServerSenddata(uint8_t len);
char BC95_InquireNetworkTime(void);
char  BC95_Init(void);



#endif