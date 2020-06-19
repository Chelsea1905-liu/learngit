/**************************************************************************//**
 * @file     MAIN.h
 * @brief    main文件全局变量，蓝牙协议定义
 * @version  V1.0.0
 * @date     2017.09
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par      NB-IOT模块
 *   
 ******************************************************************************/
#ifndef __MAIN_H
#define __MAIN_H

#include "nrf_gpio.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "app_gpio_in_out.h"
#include "boards.h"
#include "nrf_drv_uart.h"
#include "nrf_drv_timer.h"
#include "app_timer.h"      //定时器
#include "softdevice_handler.h"  //BLE协议栈
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_nus.h"
#include "bsp_btn_ble.h"
#include "ble_dis.h"
#include "pstorage.h"
#include "FLASH.h"
#include "Fun.h"
#include "FilterPeak.h"
#include "app_it.h"
#include "nrf_drv_wdt.h"
#include "ring_buffer.h"

extern uint8_t send_data_uart[5];    
//==============================函数申明====================================================


void SIMUART_Receive_Order_Dispose(uint8_t *dat);
void UART_Receive_Order_Analys(void);
void app_timer1_init(void);
void ExtClock_initialization(void);
void Power_ON(void);
void UART_Receive_Order_Analys(void);
void Server_Order_Dispose(uint8_t *dat);
void WDT_Config(void);
void wdt_event_handler(void);
void PeripheralsPowerClose(void);
void PeripheralsPowerOpen(void);

static void flash_pstorage_cb_handler(pstorage_handle_t  * handle,
                               uint8_t              op_code,
                               uint32_t             result,
                               uint8_t            * p_data,
                               uint32_t             data_len);


#endif
 