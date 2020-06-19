/******************************************************************************
 * @file     Fun.c
 * @brief   ���ܺ���
 * @version  
 * @date     2017
 * @note
 * Copyright (C)  2017 ���
 *
 * @par       
*******************************************************************************/

#include "Fun.h"
#include "Parameter.h"
#include "main.h"
const static int MAX_COUNT_NUM = 150;
const static int SAMPLE_NUM = 12;
const static int MAX_VALUE = 255;
uint8_t CheckCount_ExtSensor = 0;    //�ⲿ������������
uint8_t CheckCount_Battery = 0;    //��ؼ�����
uint32_t ADCVol_ExtSensor = 0;
uint32_t ADCVol_Battery = 0;//��ؼ��ƽ����
//reapdata..max:679..min:.-781
static int sHrWaveBuf[MAX_COUNT_NUM];// 3��,ÿ��4���� ,
static int sRrWaveBuf[MAX_COUNT_NUM];// 3��,ÿ��4���� ,
static int sRrSampleWaveBuf[MAX_COUNT_NUM];// ������Ĳ��Σ����Ϊ255,
static int filterRrWave[SAMPLE_NUM];//ȡ����Ĳ���
static int index = 0;
static uint16_t HrWaveData_Len = 0;
static uint16_t RrWaveData_Len = 0;
static uint8_t *HrWaveData;
static uint8_t *RrWaveData;
/****************************************************************************
* 	�� �� ��:  CalculateHrRrData
* 	����˵��:  ���ʺͺ����˲��ͼ���
* 	��    �Σ�
* 	�� �� ֵ:
* 	˵    ����
*****************************************************************************/
//float signals[SHIFTEDLEN];
int signalsLen = 0;
int shilftPoint =0 ;
//��ʼ������
void Arg_init()
{
    respReduce = 0.3;//�������οsС����
}

/****************************************************************************
*	�� �� ��: CalculateHrRrData
*	����˵��: ����˯������
*	��    �Σ�
*	�� �� ֵ: 
*****************************************************************************/
void CalculateHrRrData(void)
{
//    static unsigned int flagDownSampleRespHr = 0,flagDownSampleResp = 0;
//    static unsigned int shilftPoint = 0;
//    static unsigned int signalsLen = 0;
//	  uint16_t i,j ;
//    static uint16_t last_HwavLen=0;
//    char read_str[20]={0};
//    float test_signalh=0;
    static uint8_t last_heart_len=0;
//    static uint8_t last_resp_status=0;   
   
    uint32_t val_heart = 0;
    uint32_t val_breath = 0;	  
    int ret=0;
    float signal=0.0f;
    float signalrr=0.0f;	
		uint16_t ii=0;
		ii = writedata_timer;
		Arg_init();
		
    for(uint16_t read_conut=0; read_conut<ii; read_conut++)
    {
        ret = Read_RingBuff(&ringBuff_ori,&val_breath,&val_heart);
				if(ret == 0)
				{
					return; 
				}
        signal=val_breath;  
				signalrr=val_heart;                  
				
				arithmeticHearts(signalrr);
        arithmeticResps(signal);
//				printf("%d,%d\r\n",val_breath,val_heart);  //lqs test
    }
		
    writedata_timer=writedata_timer-ii;

    Flag_PeopleTurnforHert=move_body;
    Status_ExtSensorOnbed = gPeopleFlag;
  
	  
    //����
    if (hearts_len > 0) 
    {

        NoHeartTime = 0;

        if(last_heart_len!=hearts_len)
//        {
            last_heart_len=hearts_len;
//        if (move_body == 0)
//            //printf(" ��Ϣ\r\n");
//        ;
//        else
//           // printf(" �嶯\r\n");
//        ;
//        }                
        Last_Heart = hearts[0][hearts_len-1];
    }
//		Last_Heart = heartRateAvg;
    //����
    if(RespRate_len > 0)
    {
        NoRespTime = 0;
        if(RespRate_len>20)
            RespRate_len=0;
        
        Last_Resp = RespRate[0][hearts_len-1];
    }
}
/****************************************************************************
*	�� �� ��: Check_ExtSensor_PeopleOnBed
*	����˵��: ����ⲿ�������Ƿ������ڴ�
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����200ms���һ��״̬���������10��״̬һ�£��������״̬�ȶ�
*****************************************************************************/
void Check_ExtSensor_PeopleOnBed(void)
{
	float adcvlaue = 0;
	
	if(OnbedStatus_CountTimer >= 20)
	{		
		adcvlaue = adc_vdata[3]*3000/4096.0;

		if(adcvlaue>2500)
		{
			adcvlaue = 2500;
		}
		CheckCount_ExtSensor++;	
		OnbedStatus_CountTimer = 0;		
		if( abs((int)adcvlaue -(int)ADC_VoltageValue[3] ) >1000)   //���ݳ��������ʱ��״̬�����㣬��ֹ�źŸ���
		{
	//		printf("�źŸ���\r\n"); //lqs test
			ADC_VoltageValue[3] = adcvlaue;
			CheckCount_ExtSensor = 0;
			ExtSensor_OnbedStatus_Count = 0;
			ADCVol_ExtSensor = 0;
			return;
		}
		ADC_VoltageValue[3] = adcvlaue;
		ADCVol_ExtSensor += (uint32_t)adcvlaue;
		
		if(adcvlaue> 800)
		{
			ExtSensor_OnbedStatus_Count++;				
		}
		if(CheckCount_ExtSensor >= 25)
		{
			ADCVol_ExtSensor /= 25;
//			printf("ADCֵ��%d\r\n",ADCVol_ExtSensor);
//			if((ADCVol_ExtSensor>=1000)&&(Status_ExtSensorOnbed != EXTSENSOR_PEOPLEONBED))   //lqs test
			if((ADCVol_ExtSensor>=1000)&&(gPeopleFlag != EXTSENSOR_PEOPLEONBED))
			{
                pieValue = 1;
                SleepData_SendTime = SLEEPDATA_SENDTIME_3S;
                #ifdef ENABLE_SENDPRESSSERSONSTATUS
                Flag_SendPresensorStatus = 1;
                SendPresensorStatus_Time = 4;
                #endif
                if(Flag_UART_Debug == DEBUG_CMD_UART)
                {
                    printf("����\r\n");
                }
			}
			if((ExtSensor_OnbedStatus_Count<3)&&(ADCVol_ExtSensor<600) && (Status_ExtSensorOnbed != EXTSENSOR_PEOPLEOFFBED))
			{		
                pieValue = 0;
				#ifdef ENABLE_SENDPRESSSERSONSTATUS
				Flag_SendPresensorStatus = 1;
				SendPresensorStatus_Time = 4;
				#endif	
                if(Flag_UART_Debug == DEBUG_CMD_UART)
                {
                    printf("����\r\n");  
                }       
                RingBuff_Init(&ringBuff_ori,0);
//                RingBuff_Init(&ringBuff_heart_point,0);
//                RingBuff_Init(&ringBuff_resp_point,0);                
			}
			ExtSensor_OnbedStatus_Count = 0;
			CheckCount_ExtSensor = 0;
			ADCVol_ExtSensor = 0;
		}
	}
}
/**************************************************************************************
*	�� �� ��: BatteryDataCalculate
*	����˵��: ��ص�������
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����  
  1��200ms���һ��״̬���������10��״̬һ�£��������״̬�ȶ�
  2��4.2-4V 100%  4-3.8V 80%   3.8-3.6V 60%  3.6-3.4 40% 3.4V���µ������� 
  3��RESULT = [V(P) �C V(N) ] * GAIN/REFERENCE * 2(RESOLUTION - m)     GAIN = 1/6     REFERENCE = 0.6v
***************************************************************************************/
void BatteryDataCalculate(void)
{
	float adcvlaue = 0;


    if(BatteryCheck_CountTimer >= 20)
   {		
        BatteryCheck_CountTimer = 0;
        adcvlaue = adc_vdata[0]*3000/4096.0;


        if(adcvlaue<0)
        {
           adcvlaue = 0;
        }
        CheckCount_Battery++;	
        if( abs((int)adcvlaue -(int)ADC_VoltageValue[0] ) >300)   //���ݳ��������ʱ��״̬�����㣬��ֹ�źŸ���
        {
            ADC_VoltageValue[0] = adcvlaue;
            CheckCount_Battery = 0;
            ADCVol_Battery = 0;
            return;
        }
        ADC_VoltageValue[0] = adcvlaue;	
        ADCVol_Battery += (uint32_t)adcvlaue;		

        if(CheckCount_Battery >= 25)
        {
            ADCVol_Battery /= 25;
            if(ADCVol_Battery>=960)  //100%
            {	    
                CheckCount_Battery=0;	
                Battery_level = Battery_level_100;
            }
            else if((ADCVol_Battery >= 910) && (ADCVol_Battery < 960))//80%
            {
                CheckCount_Battery=0;	
                Battery_level = Battery_level_80;
            }
            else if((ADCVol_Battery >= 870) && (ADCVol_Battery < 910))//60%
            {
                CheckCount_Battery=0;	
                Battery_level = Battery_level_60;
            }
            else if((ADCVol_Battery >= 820) && (ADCVol_Battery < 870))//40%
            {
                CheckCount_Battery=0;	
                Battery_level = Battery_level_40;
            }
            else if(ADCVol_Battery < 820)//20%
            {
                CheckCount_Battery=0;
                Battery_level = Battery_level_20;                
            }

            
        }
        
    }
    
}
/****************************************************************************
*	�� �� ��: RenewRealTime
*	����˵��: ����ʱ��
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
void RenewRealTime(void)
{
	char i=0;
	uint8_t year = 0,month = 0,daysPerMonth = 0;

	 RealTime[5] += 1;						//���ʱ
	if(RealTime[5] >= 60)
	{			                  //�ֽ�λ
		RealTime[5]= 0;
		RealTime[4]++;
		if(RealTime[4] >= 60)
		{				                //ʱ��λ
			RealTime[4]=0;
			RealTime[3]++;
			if(Flag_TimeAdj == 0)
			{
				 Adj_Time = 0;
				 Flag_TimeAdj = 1;		  //����ָ��ͬ��������ʱ��
			}
		}
		if(RealTime[3] >= 24)
		{				//�ս�λ
			RealTime[3]=0;
			RealTime[2]++;
		}
		//�ж�ÿ���·ݵ�����
		year = RealTime[0];
		month = RealTime[1];
		if(year % 4 == 0 && month == 2)
		{
			daysPerMonth= 29;
		}
		else
		{
			switch(month)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					daysPerMonth = 31;
					break;
				case 2:
					daysPerMonth = 28;
					break;
				default:
					daysPerMonth = 30;
					break;
			}
		}
		if(RealTime[2] > daysPerMonth)
		{				//�½�λ
			RealTime[2]=1;
			RealTime[1] ++;
		}
		if(RealTime[1]  >  12)
		{				//���λ
			RealTime[1] =1;
			RealTime[0]++;
		}
		if(RealTime[0]>=  99)
		{				//�긴λ
			RealTime[0] = 0;
		}
	}	
}
/****************************************************************************
*	�� �� ��: HexToStr
*	����˵��: 16��������תΪ�ַ���
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************//*
void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}*/
/****************************************************************************
*	�� �� ��: HexToLowerStr
*	����˵��: 16��������תΪ�ַ���
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void HexToLowerStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 39;//'9'��'a'���39
        if (ddl > 57) ddl = ddl + 39;
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}
/****************************************************************************
*	�� �� ��:  GetDataHead
*	����˵��:  ��ȡ���ݰ�������ͷ�ֽ�
*	��    �Σ�
*	�� �� ֵ:
* ˵    ����
*****************************************************************************/
unsigned char GetDataHead(unsigned char* datas,unsigned char len)
{
	uint8_t DH = 0x00;
	uint8_t i = 0;
	
	for(i=0;i<len;i++)
	{
		DH |= (*(datas+i)&0x80)>>(7-i);
	}
	return  DH;
}
/****************************************************************************
*	�� �� ��: SendPressSenserStatus
*	����˵��: ����ѹ��������״̬
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
void SendPressSenserStatus(uint8_t status)
{
	 uint8_t senddata[3]={0};
	 	 	   
	senddata[0] = CMD_PRESSSENSER;
    senddata[1] = SendPressSenserStatusTimes;
	if(status == EXTSENSOR_PEOPLEONBED)
		senddata[2] = PRESSSENSER_ON|0x80;
	else
		senddata[2] = PRESSSENSER_OFF|0x80;
   
    SendPressSenserStatusTimes++;

    BLE_Send_data(senddata,sizeof(senddata));
}
/****************************************************************************
*	�� �� ��: SendBatteryData
*	����˵��: ���͵�ص�ѹ
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
void SendBatteryData(uint8_t battlev)
{
	uint8_t senddata[3]={0};
	 	 	   
	senddata[0] = BATTERY_DATA;
    senddata[1] = SendBatteryDataTimes;
    senddata[2] = battlev;
    SendBatteryDataTimes++;
if(SendBatteryDataTimes>255)
    SendBatteryDataTimes=0;
    BLE_Send_data(senddata,sizeof(senddata));
}
/****************************************************************************
*	�� �� ��: SendRealTimeHRData
*	����˵��: ������������
*	��    �Σ�dat:���͵��������飬len  ���鳤��
*	�� �� ֵ: 
* 	˵    ����
*****************************************************************************/
void SendRealTimeHRData(void)
{
	uint16_t i=0;
	uint8_t HeartCal = 0;
	uint8_t HRData[1024];
	unsigned int heart_sum=0;
	uint16_t len = hearts_len;
	uint16_t data_len=0;

	HRData[0] = HREARTRATE_DATA;
    HRData[1] = SendHrDataToServerTimes;

	if(Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)
        HRData[2] =1;//�ٴ�
//	if((Flag_PeopleTurnforHert>0))//lqs test
	if(move_body>0)  
		HRData[2] =2;//�嶯
	if(resp_status ==1)
			HRData[2] =7;//������ͣ
	if(resp_status ==2)
			HRData[2] =9;
	if(Status_ExtSensorOnbed != EXTSENSOR_PEOPLEONBED)
			HRData[2] =0;//�봲

	data_len = 3;
	for(i=0;i<len;i++)
	{
//        if(Flag_UART_Debug == DEBUG_CMD_UART)
//        {
//            printf("����ֵ��%u,%u ,��������\r\n",getHeartRate(heartRateAvg),heartRateAvg);
//        }
//        HRData[data_len++] =(uint8_t)(getHeartRate(hearts[0][0]));  //lqs test
//        HRData[data_len++] = getHeartRate(Last_Heart);
		heart_sum = getMedValue_test(hearts[0][i]);  //lqs test
	}
	HeartCal = getHeartRate(heart_sum);
	HRData[data_len++] = HeartCal;
	hearts_len = 0;
	printf(" ����ֵ: %d    hearts_len: %d\r\n", HeartCal,len); //lqs test

	BLE_Send_data(HRData,data_len);
}
/****************************************************************************
*	�� �� ��: SendNoHRData
*	����˵��: ����״̬�·���ȫ0����
*	��    �Σ�
*	�� �� ֵ: 
* 	˵    ����
*****************************************************************************/
void SendNoHRData(uint8_t status)
{
	uint8_t i=0;
	uint8_t jiaoyan = 0;
	uint8_t HRData[50];
    uint16_t data_len=0;

    HRData[0] = HREARTRATE_DATA;
    HRData[1] = SendHrDataToServerTimes;

	if(status == STATUS_NOPEOPEL)
	{
		HRData[2] = 0;  //����໤
		HRData[3] = 0;
	}
	if(status == STATUS_NOHR)  //����������
	{
		if(resp_status ==1)
			HRData[2] = 7;//������ͣ	
        else if(resp_status ==2)
            HRData[2] =9;
		else
			HRData[2] = 5;//�����޷�����
		HRData[3] = 0;

	}
    if(status == STATUS_HRCANNOTCAL)
    {
        HRData[2] = 5;//�����޷�����
		HRData[3] = 0;
    }
	
    BLE_Send_data(HRData,4);
}
/****************************************************************************
*	�� �� ��: SendRealTimeRRData
*	����˵��: ���ͺ�������
*	��    �Σ�dat:���͵��������飬len  ���鳤��
*	�� �� ֵ: 
* 	˵    ����
*****************************************************************************/
void SendRealTimeRRData(void)
{
	uint16_t i=0;
	uint8_t jiaoyan = 0;
	uint8_t RRData[256];
	uint16_t len = RespRate_len;
	uint16_t data_len=0;
    uint16_t breath_sum=0;
uint16_t resprate=0;
	RRData[0] = BREATHE_DATA;//SEND_RRINTERTIME;
    RRData[1] = SendRrDataToServerTimes;

	
	if(Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)
        RRData[2] =1;//�ٴ�
//	if((Flag_PeopleTurnforHert>0))  //lqs test
		if(move_body>0)  
			RRData[2] =2;//�嶯
    if(Status_ExtSensorOnbed != EXTSENSOR_PEOPLEONBED)
        RRData[2] =0;//�봲
    
    data_len=3;
	for(i=0;i<len;i++)
	{
//        if(Flag_UART_Debug == DEBUG_CMD_UART)
//        {
//             if (RespRate[1][i] == 0) 
//             {
//                  printf("����ֵ��%u ,��������\r\n",getRespRate(RespRate[0][i]));
//             }
//             else
//             {
//                 printf("����ֵ��0 ,������ͣ");
//             }
//        }	
        

        //resprate = (MIN * MS / RespRate[0][i]);
        RRData[data_len++] = (uint8_t)getRespRate(RespRate[0][i]);
	}
    RespRate_len =0;

    BLE_Send_data(RRData,data_len);																	 	 
}
/****************************************************************************
*	�� �� ��: SendNoRRData
*	����˵��: ����״̬�·���ȫ0����
*	��    �Σ�
*	�� �� ֵ: 
* 	˵    ����
*****************************************************************************/
void SendNoRRData(uint8_t status)
{
	uint8_t i=0;
	uint8_t jiaoyan = 0;
	uint8_t RRData[50];
    uint16_t data_len=0;

	RRData[0] = BREATHE_DATA;
    RRData[1] = SendRrDataToServerTimes;


	if(status == STATUS_NOPEOPEL)//����
	{
		RRData[2] = 0;
		RRData[3] = 0;
        if(Flag_UART_Debug == DEBUG_CMD_UART)
        {
            printf("����:����\r\n");
        }
	
	}
	if(status == STATUS_NORR)//�������޺���
	{
		RRData[2] = 8;
		RRData[3] = 0;          
        if(Flag_UART_Debug == DEBUG_CMD_UART)
        {
            printf("����:�������޺���\r\n");
        }
    }
	if(status == STATUS_APNOEA)//������ͣ
	{
		RRData[2] = 7;
		RRData[3] = 0;
        if(Flag_UART_Debug == DEBUG_CMD_UART)
        {
            printf("����:������ͣ\r\n");
        }
	}
    if(status == STATUS_RRCANNOTCAL)//�����޷�����
	{
		RRData[2] = 8;
		RRData[3] = 0;
        if(Flag_UART_Debug == DEBUG_CMD_UART)
        {
            printf("����:�����޷�����\r\n");
        }
	}
    if(status == STATUS_OBSAPNOEA)//�����Ժ�����ͣ
	{
		RRData[2] = 9;
		RRData[3] = 0;
        if(Flag_UART_Debug == DEBUG_CMD_UART)
        {
            printf("����:�����Ժ�����ͣ\r\n");
        }
	}
	
	BLE_Send_data(RRData,4);
}
/****************************************************************************
*	�� �� ��:  SendHrDataToServer
*	����˵��:  ������������
*	��    �Σ�
*	�� �� ֵ:
* 	˵    ����
*****************************************************************************/
void SendHrDataToServer(void)
{
	uint8_t i = 0;
	float averageresp  = 0;
	unsigned int heart_sum=0;
	
	if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(Last_Heart == 0)&&(NoHeartTime<15))  //15s���������������ʹ����һ�ε�����ֵ
	{
		 hearts[0][0] = Last_Heart;
		 hearts_len = 1;
//        heartRateAvg = Last_Heart;
	}
  
//	printf("gPeopleFlag:%d    Status_ExtSensorOnbed:%d   ���߼�ʱ:%d    pieValue:%d\r\n", gPeopleFlag,Status_ExtSensorOnbed,sleep_mode_timer,pieValue);
	//--------------��������----------------------- 
	if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(Last_Heart > 0))
	{
		if(DeviceMode == Mode_Uart)
		{
			for(i=0;i<hearts_len;i++)
			{
				heart_sum = getMedValue_test(hearts[0][i]);
			}
			send_data_uart[0] = getHeartRate(heart_sum);
			hearts_len = 0;
			return;
		}
		SendRealTimeHRData();				//������������		
	}
	else if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(Last_Heart == 0))    //���������ʺ���
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[0] = 0;
			return;
		}
		SendNoHRData(STATUS_NOHR);						
	}
    else if(Status_ExtSensorOnbed == EXTSENSOR_CANNOTCAL)
    {
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[0] = 0;
			return;
		}
        SendNoHRData(STATUS_HRCANNOTCAL);
    }
	else
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[0] = 0;
			return;
		}
		SendNoHRData(STATUS_NOPEOPEL);
	}

	for(i=0;i<6;i++)
	{
		HR_StartTime[i] = RealTime[i];
	}
    SendHrDataToServerTimes++;
    if(SendHrDataToServerTimes>255)
        SendHrDataToServerTimes=0;
}
/****************************************************************************
*	�� �� ��:  SendRrDataToServer
*	����˵��:  ���ͺ�������
*	��    �Σ�
*	�� �� ֵ:
* 	˵    ����
*****************************************************************************/
void SendRrDataToServer(void)
{
    uint8_t i = 0;
	float averageresp  = 0;

    if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(RespRate_len == 0)&&(NoRespTime <6))   //15s��������޺���ֵ����ʹ����һ�εĺ���������15s��Ϊ�޺���
	{
		RespRate[0][0] = Last_Resp;
		RespRate_len = 1;
	}
	//------------���ͺ���--------------------------
	if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(resp_status ==1))  //������ͣ
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[1] = 0;
			return;
		}
		SendNoRRData(STATUS_APNOEA);
	}		
    if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(resp_status ==2))  //�����Ժ�����ͣ
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[1] = 0;
			return;
		}
		SendNoRRData(STATUS_OBSAPNOEA);
	}	
	if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(RespRate_len > 0)&&(resp_status ==0))
	{
		if(DeviceMode == Mode_Uart)
		{
			for(i=0;i<RespRate_len;i++)
			{
				send_data_uart[1] = (uint8_t)getRespRate(RespRate[0][i]);
			}
			RespRate_len = 0;
			return;
		}
		SendRealTimeRRData();				//���ͺ�������	
	}
	else if((Status_ExtSensorOnbed == EXTSENSOR_PEOPLEONBED)&&(Last_Heart>0)&&(RespRate_len == 0)&&(resp_status ==0))    //�����������޺���
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[1] = 0;
			return;
		}
		SendNoRRData(STATUS_NORR);			
	}
	else if(Status_ExtSensorOnbed == EXTSENSOR_PEOPLEOFFBED)//
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[1] = 0;
			return;
		}
		SendNoRRData(STATUS_NOPEOPEL);

	}			
	else if(Status_ExtSensorOnbed == EXTSENSOR_CANNOTCAL)//
	{
		if(DeviceMode == Mode_Uart)
		{
			send_data_uart[1] = 0;
			return;
		}
		SendNoRRData(STATUS_RRCANNOTCAL);

	}	  
  
	for( i=0;i<6;i++)
	{
		RR_StartTime[i] = RealTime[i];
	}
    SendRrDataToServerTimes++;
    if(SendRrDataToServerTimes>255)
    SendRrDataToServerTimes=0;
}
/****************************************************************************
*	�� �� ��:  BLE_Send_data
*	����˵��:  ��������
*	��    �Σ�
*	�� �� ֵ:
* 	˵    ����
*****************************************************************************/
void BLE_Send_data(uint8_t *buf, uint16_t send_len)
{
    #define PACKAGE_LEN 8
    uint8_t packages=0;
    uint16_t i;
    uint16_t remainds_data=0;
    char err;

    sd_ble_gap_adv_stop();
    advertising_status_data(buf,send_len);
    ble_advertising_start(BLE_ADV_MODE_FAST);
}

//���ܵľ�ֵ�˲�
#define AVG_RESP_LEN 4
static float avgResps[AVG_RESP_LEN];
static int avgRespLen = 0;

unsigned int avgRespFilter_lqs(float x) {
    if (avgRespLen >= AVG_RESP_LEN)
        avgRespLen = 0;
    avgResps[avgRespLen] = x;
    avgRespLen++;

    float sum = 0;
    int num = 0;
    for (int i = 0; i < AVG_RESP_LEN; ++i) {
        if(avgResps[i] > 0){
            sum += avgResps[i];
            num++;
        }
    }

    if (num < 1)
        return 0;
    return (unsigned int) (sum / num);
}



#define MEDFILT_LEN 7//��ֵ�˲����鳤��
static int med_sorts_s[MEDFILT_LEN]={0};
unsigned char temp_len_test = 0;
char ts_test = 0;
/***
 * ѡ������,����Ԫ��5��
 */
int getMedValue_test(int rate){
    if(temp_len_test > MEDFILT_LEN-1){
        ts_test = 1;
        temp_len_test = 0;
    }
    med_sorts_s[temp_len_test++] = rate;

    int temp_sort[MEDFILT_LEN] = {0};
    memcpy(temp_sort, med_sorts_s, MEDFILT_LEN * sizeof(float));//��sorts���鸳ֵ��temp_sort����


    char blc = 0;
    if(ts_test == 0)
        blc = temp_len_test;
    else
        blc = MEDFILT_LEN;

    for (int i = 0; i < blc; i++) {
        int minIndex = i;
        for (int j = i; j < blc; j++) {
            if (temp_sort[j] < temp_sort[minIndex]) //�ҵ���С����
                minIndex = j; //����С������������
        }
        int temp = temp_sort[minIndex];
        temp_sort[minIndex] = temp_sort[i];
        temp_sort[i] = temp;
    }
    int medIndex = (int)((blc-1)*0.5f);
    return temp_sort[medIndex];
}