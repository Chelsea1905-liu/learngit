/******************************************************************************
 * @file     EMW3080.h
 * @brief   WiFi通讯函数
 * @version  
 * @date     2017.11
 * @note
 * Copyright (C)  
 *
 * @par      张炜201711
 *        EMW3080 WiFi模块通讯程序
*******************************************************************************/
#ifndef __EMW3080_H_
#define __EMW3080_H_

#include "boards.h"
#include "app_uart.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"
#include "Parameter.h"
#include "Fun.h"
#include "app_it.h"
#include "app_gpio_in_out.h"
#include "BLE_NRF.h"
#include <stdlib.h>
#include "string.h"



										
/****************************************************************/
#define WIFI_USART   USART2
#define SOCKETA         0
#define SOCKETB         1

#define  ATMODEIN       1     // 进入AT指令的方式，1为指令方式，0为gpio引脚电平方式

#define    WIFI_SUCCESS              0
#define    WIFI_FAIL                -1
#define    WIFI_CFG_ERROR           -2
					
#define    WIFI_CONNECT             1
#define    WIFI_NOCONNECT           2					
					
#define   WIFI_TCP_CONNECT           1
#define   WIFI_TCP_NOCONNECT         2
#define   WIFI_DATASEND_ATMODE       1
#define   WIFI_DATASEND_TRANSMODE    0

//----------- timeout Definition---------------------------------
       					 
#define WIFI_INC_TIMER               WIFI_Timer++
#define WIFI_RESET_TIMER             WIFI_Timer=0


#define WIFI_1S_TIMEOUT              1
#define WIFI_3S_TIMEOUT              3
#define WIFI_5S_TIMEOUT           	 5
#define WIFI_10S_TIMEOUT           	10
#define WIFI_20S_TIMEOUT           	20
#define WIFI_30S_TIMEOUT           	30
#define WIFI_1MIN_TIMEOUT           	60
#define WIFI_3MIN_TIMEOUT           	180
#define WIFI_5MIN_TIMEOUT           	300


extern uint8_t AP_Channel;          //AP连接的信道
/****************************************************************/

char WIFI_INIT(void);
char WIFI_Open_cmdMode(void);
char WIFI_STA_Status(void);
char WIFI_Query_STA_RSSI(void);
char WIFI_Set_TCPConnectIP(char *ip,char *port);
char WIFI_Query_TCPConnectSocket(void);
char WIFI_Send_data(uint8_t sock_fd, uint16_t send_len, uint8_t *buf);
char WIFI_Set_STAMode(void);
char WIFI_Open_STA(void);
char WIFI_Set_STASSID(char *stassid,char *stapwd);
char WIFI_Open_TCPConnect(void);
void WIFI_Query_macaddr(void);
void WIFI_Query_fwversion(void);
void WIFI_Reset(void);
void WIFI_Restore(void);
void WIFI_SaveConfig(void);
void WIFI_SetRFLowPower(void);
void WIFI_OpenDHCP(void);
void WIFI_Query_STAStatus(void);
void WIFI_Query_rssi(void);
void WIFI_Query_IPconifg(void);
char WIFI_Query_TCPStatus(void);
void WIFI_Open_Event(void);
void WIFI_Query_TCPConnectIP(void);
char WIFI_ConnectIPCompare(void);
void WIFI_USART_Send(char *tx_buf,uint16_t buflen);
char WIFI_Req_STASSID(void);

#endif