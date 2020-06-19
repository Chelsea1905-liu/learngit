/******************************************************************************
 * @file     Fun.h
 * @brief   ���ܺ���
 * @version  
 * @date     
 * @note
 * Copyright (C)  
 *
 * @par       
*******************************************************************************/
#ifndef __FUN_H_
#define __FUN_H_

#include <stdint.h>
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_drv_uart.h"
#include "boards.h"
#include "app_gpio_in_out.h"
#include "Parameter.h"
#include <stdlib.h>
#include "MD5.h"
#include "ring_buffer.h"
#include "FilterPeak.h"
#include "BLE_NRF.h"
#include "Parameter.h"
typedef struct status{
	unsigned char isOnbed;//�Ƿ��ڴ�, 0: �� 1:��
	unsigned char isMove;//�Ƿ��嶯, 0: �� 1:��
	unsigned char isStopBreath;//�Ƿ�ֹͣ����0:�� 1:��
}STATUS;
void Check_ExtSensor_PeopleOnBed(void);
void CalculateHrRrData(void);
void BatteryDataCalculate(void);
void RenewRealTime();
//void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);
void HexToLowerStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);
unsigned char GetDataHead(unsigned char* datas,unsigned char len);
void SendPressSenserStatus(uint8_t status);
unsigned int avgRespFilter_lqs(float x);  //��ֵ�˲�
int getMedValue_test(int rate);              //��ֵ�˲�
/***********************
*���ʺ�������
***********************/
uint16_t SendHeartWaveToserver(uint8_t *HRWData);
//uint16_t SendRespWaveToserver(uint8_t *RRWData);
uint16_t SendSampleRespWaveToserver(uint8_t *RRWData,float *rrv,uint8_t SampInv);
void SendWaveDataToserver(void);

void Arg_init();
/***********************
*���ʺ�������
***********************/
void SendRealTimeHRData(void);
void SendRealTimeRRData(void);	
void SendNoHRData(uint8_t status);
void SendNoRRData(uint8_t status);
void SendRrDataToServer(void);
void SendHrDataToServer(void);
void SendBatteryData(uint8_t battlev);
void BLE_Send_data(uint8_t *buf, uint16_t send_len);
#endif

