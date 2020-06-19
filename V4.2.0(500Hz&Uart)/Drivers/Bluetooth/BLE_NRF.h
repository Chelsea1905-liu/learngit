/****************************************************************************
 * @file     BLE_NRF.H
 * @brief    蓝牙数据传输
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par     NB-IOT模块
 *  
*****************************************************************************/
#ifndef __BLE_NRF_H__ 
#define __BLE_NRF_H__

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

//==================flash存储设置=====================================
#define		PSTORAGE_PAGES	1
#define   M_BLOCK_SIZE     32
#define   M_BLOCK_COUNT    2


void  APP_Receive_Order_Dispose(uint8_t *dat);
//uint32_t APP_Send_Order(uint8_t *pdata,uint8_t len);	
uint8_t BLE_Start(void);
void  BLE_SendString(uint8_t *str);
void  BLE_SendBufData(char *buf,uint8_t len);
static void ble_stack_init(void);
static void ble_evt_dispatch(ble_evt_t * p_ble_evt);
static void on_ble_evt(ble_evt_t * p_ble_evt);
static void gap_params_init(void);
static void services_init(void);
static void app_receivedata_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);
static void advertising_init(void);
static void on_adv_evt(ble_adv_evt_t ble_adv_evt);
void sleep_mode_enter(void);
static void conn_params_init(void);
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt);
static void conn_params_error_handler(uint32_t nrf_error);
static void sys_evt_dispatch(uint32_t sys_evt);
void register_flash(void);
void BLE_SendHRRRData(void);
void BLE_SendRRWaveData(void);
void Send_CurrentLinkStatus(void);
void advertising_status_data(uint8_t *count_datai,uint8_t len);
#endif
