/******************************************************************************
 * @file     EMW3080.c
 * @brief   WiFiͨѶ����
 * @version  
 * @date     2017.11
 * @note
 * Copyright (C)  
 *
 * @par      ���201711
 *    V1.0    EMW3080 WiFiģ��ͨѶ����
*******************************************************************************/


#include "EMW3080.h"



#define htons(n) 				(((n & 0xff) << 8) | ((n & 0xff00) >> 8))
#define htonl(n) 				(((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000UL) >> 8) | ((n & 0xff000000UL) >> 24))
#define ntohs(n) 				htons(n)
#define ntohl(n) 				htonl(n)


uint8_t AP_Channel=0;          //AP���ӵ��ŵ�

/****************************************************************************
*	�� �� ��: WIFI_INIT
*	����˵��: ����ģ���ʼ��
*	��    �Σ�
*	�� �� ֵ:���س�ʼ����ɵĲ���
* ˵    ����
*****************************************************************************/
char WIFI_INIT(void)
{
	 uint8_t buf[5];
	
//-------����ATָ��ģʽ---------------------------------
	 if(Flag_WIFIInitStep == WIFI_INIT_STEP1)
	 {	 
		 if(WIFI_Open_cmdMode()==WIFI_SUCCESS)
		 {
			 if(Flag_PowerOn == 1)
			   Flag_WIFIInitStep = WIFI_INIT_STEP2;
			 else
				 Flag_WIFIInitStep = WIFI_INIT_STEP3;
		 }
		 else     //ATָ��ģʽ�޷����룬��λģ��
		 {
				if(WiFi_InitTime>=NETWORKLINK_TIMEOUT_30S)
				 {
					  app_uart_close();
						WIFI_POWER(0);
						nrf_delay_ms(5000);
						WIFI_POWER(1);
					  nrf_delay_ms(500);	
	          UART_CONFIG(UART_WIFI);
						Flag_DataSendMode = WIFI_DATASEND_TRANSMODE;   //͸��ģʽ
						WiFi_InitTime = 0;
				 }
        nrf_delay_ms(2000);				 
			  return WIFI_INIT_STEP1;
		 } 
   }	 
//-------wifiģ������---------------------------------
	 if(Flag_WIFIInitStep == WIFI_INIT_STEP2)
	 {
		 WIFI_Open_Event();
		 WIFI_Open_TCPConnect();
		 WIFI_SetRFLowPower();
		 WIFI_Set_STAMode();
		 WIFI_Open_STA();
		 Flag_WIFIInitStep = WIFI_INIT_STEP3;
		 WiFi_InitTime = 0;
	 }
//-------��ѯ·��������״̬---------------------------------	 
	 if(Flag_WIFIInitStep == WIFI_INIT_STEP3)
	 {
		  if(WIFI_STA_Status()== WIFI_SUCCESS)
			{
				  if(Flag_Reset_RouterSSID ==1)
					{
					   buf[0] = SET_ROUTER_NAME;
		         buf[1] = MODULESTATUS_LINKSERVER;			 
             APP_Send_Order(buf,2);	
					}
				 Flag_WIFIInitStep = WIFI_INIT_STEP4;	     
			}
			else
			{
				 if(WiFi_InitTime==NETWORKLINK_TIMEOUT_30S)
					{
						if(ReadRouterNameFromFlash()==1)
						{
							WIFI_Set_STASSID(Router_Name,Router_PWD);  //ʹ�ñ����wifi������������
						}
						else
						{
							WIFI_Reset();
              nrf_delay_ms(500);							
						}
						Flag_WIFIInitStep = WIFI_INIT_STEP1;
						return  WIFI_INIT_STEP1;
					}
          if(WiFi_InitTime>=NETWORKLINK_TIMEOUT_60S)
					{
						 if(Flag_Reset_RouterSSID ==1)
							{
								 buf[0] = SET_ROUTER_NAME;
								 buf[1] = MODULESTATUS_ROUTERERR;			 
								 APP_Send_Order(buf,2);	
							}
						if(ReadRouterNameFromFlash()==1)
						{
							WIFI_Set_STASSID(Router_Name,Router_PWD);  //ʹ�ñ����wifi������������
						}
						else
						{
							WIFI_Reset();
							nrf_delay_ms(500);
						}
						WiFi_InitTime = 0;
						Flag_WIFIInitStep = WIFI_INIT_STEP1;
						return  WIFI_INIT_STEP1;
					}					
					nrf_delay_ms(1000);					
				  return  WIFI_INIT_STEP3;
			}					
   }
	 if(Flag_WIFIInitStep == WIFI_INIT_STEP4)
	 {
			WIFI_Query_STA_RSSI();
		  Flag_WIFIInitStep = WIFI_INIT_STEP5;
			return WIFI_INIT_STEP5;
	 }
}

/****************************************************************************
*	�� �� ��: WIFI_Open_cmdMode
*	����˵��: enter AT command mode from easytxrx mode
*	��    �Σ�
*	�� �� ֵ: 0��ʾ�ɹ�
* ˵    ����
*****************************************************************************/
char WIFI_Open_cmdMode(void)
{
    int8_t	     retval=0;
    uint8_t	     retry_times=0;	   //���Դ�������
	  char buf[50];
	
	  if(Flag_Debug == DEBUG_BLE)
		{
			 BLE_SendString("Set wifi in at mode!\r\n");
		}                                
		 if(Flag_DataSendMode == WIFI_DATASEND_ATMODE)
		 {
			 if(Flag_Debug == DEBUG_BLE)
				{
					 BLE_SendString("WIFI has in at mode!\r\n");
				}
			 return WIFI_SUCCESS;
		 }
   	ClearUSARTBUF();
    WIFI_USART_Send("+++",3);//���ʹ�ģ������ӿ�����
		WiFi_Time = 0;
		nrf_delay_ms(300);   //�ȴ�300ms�鿴ģ����û�з���A;	
    while(1)
    {			
			if(strstr(UART_RX_BUFFER,"a")!=NULL)
			{				
				ClearUSARTBUF();
				WIFI_USART_Send("a",1);
        delaytime1=0;
				if(Flag_Debug == DEBUG_BLE)
							{							
								 BLE_SendString("Send a\r\n");
							}
        while(1)
				{					
					if(strstr(UART_RX_BUFFER,"OK")!=NULL)  //�ɹ�����ATָ��ģʽ
						{					
							Flag_DataSendMode = WIFI_DATASEND_ATMODE;
							if(Flag_Debug == DEBUG_BLE)
							{							
								 BLE_SendString("WiFi in AT command mode\r\n");
							}
							return  WIFI_SUCCESS;					
						}
						if(delaytime1>1)
						{
							 break;
						}
			  }				
			 }
			 if(strstr(UART_RX_BUFFER,"ERR")!=NULL)
			 {
				  WIFI_Reset();				  
				  return WIFI_FAIL;
			 }
       if((WiFi_Time==2)||(WiFi_Time==4)||(WiFi_Time==6))
			 {
				  ClearUSARTBUF();
          WIFI_USART_Send("+++",3);//���ʹ�ģ������ӿ�����
				  retry_times++;
					WiFi_Time=0;				 
		      nrf_delay_ms(300);   //�ȴ�300ms�鿴ģ����û�з���A;	
			 }				 
			if(retry_times>3)				
			 {
				 if(Flag_Debug == DEBUG_BLE)
					{
							 sprintf(buf,"Time OUT:Set WIFI in at mode:%s!\r\n",UART_RX_BUFFER);
							 BLE_SendBufData(buf,strlen(buf));
					}
				 return WIFI_FAIL;
			 }					 			
    }
}
/****************************************************************************
*	�� �� ��: WIFI_STA_Status
*	����˵��: ��ѯSTA����״̬
*	��    �Σ�
*	�� �� ֵ: 0��ʾ�ɹ�
* ˵    ����
*****************************************************************************/
char WIFI_STA_Status(void)
{
	 char *pstr = NULL;
	 int8_t retval=0;
	 char buf[50];
	
	if(Flag_Debug == DEBUG_BLE)
		{
				BLE_SendString("Inquire WiFi STA link status!\r\n");
		}
	 ClearUSARTBUF();
	 WIFI_Query_STAStatus();
	 nrf_delay_ms(50);
	 WiFi_Time = 0;
	 while(1)
	 {
		  if(strstr(UART_RX_BUFFER,"STATION_UP")!=NULL)
			{
				  ClearUSARTBUF();
				  if(Flag_Debug == DEBUG_BLE)
					{
							BLE_SendString("WiFi has connect!\r\n");
					}
				  return WIFI_SUCCESS;
			}	
     if(strstr(UART_RX_BUFFER,"STATION_DOWN")!=NULL)
			{
				  ClearUSARTBUF();
				  if(Flag_Debug == DEBUG_BLE)
					{
							BLE_SendString("WiFi has not connect!\r\n");
					}
				  return WIFI_FAIL;
			}				
			if(WiFi_Time > WIFI_20S_TIMEOUT )
			{
				  ClearUSARTBUF();
				  if(Flag_Debug == DEBUG_BLE)
					{
							BLE_SendString("Read sta status time out!\r\n");
					}
				  return WIFI_FAIL;			
			}
			if(strstr(UART_RX_BUFFER,"ERR") != NULL)
			{
				 ClearUSARTBUF();
	       WIFI_Query_STAStatus();
				 nrf_delay_ms(50);
			}
	 }
}

/****************************************************************************
*	�� �� ��: WIFI_Query_STA_RSSI
*	����˵��: ��ѯSTAģʽ��rssi���������ź�ǿ��
*	��    �Σ�
*	�� �� ֵ: �����ź�ǿ������
* ˵    ������ѯSTA����״̬��AP�ź�ǿ�ȣ�APͨ�� 
*           +OK=1,-45,11
*****************************************************************************/
char WIFI_Query_STA_RSSI(void)
{
	  char *pstr = NULL;
	  char rebuf[10];
	  char csqstring[5]={0};
	  char apchstring[5]={0};
	  uint8_t len1=0;
	  uint8_t len2=0;
	  char buf[50];
	 
	  ClearUSARTBUF();
	  WIFI_Query_rssi();
	  WiFi_Time =0;
    nrf_delay_ms(10);	
	  while(1)
		{
			 pstr = strstr(UART_RX_BUFFER,"OK");
			if(pstr !=NULL)
			 {
				  if(pstr[3] == '1')   //STA������ ����ȡ��+OK=1,-20,1
					{
						 len1 = strcspn(UART_RX_BUFFER,",")+1;		 //��һ����������λ��			
						 memcpy(rebuf,pstr+len1,UART_RX_LEN-len1); //��ȡ���ַ���20,1
						len2 = strcspn(rebuf,",");                 //�ַ���20,1�С�������λ��
						memcpy(csqstring,rebuf,len2);              //��ȡ��20
						memcpy(apchstring,&rebuf[len2+1],UART_RX_LEN-len1-len2-1);//��ȡ��1
						CSQNual = atoi(csqstring);
						AP_Channel = atoi(apchstring);
						CSQNual = 113- CSQNual;
						CSQNual /= 2;
						if(Flag_Debug == DEBUG_BLE)
						{
							sprintf(buf,"csq:%d apch:%d \r\n",CSQNual,AP_Channel);
							BLE_SendBufData(buf,strlen(buf));
						}
					
						 return WIFI_CONNECT;
					}
					if(pstr[3] == '0')
					{					
						 return WIFI_NOCONNECT;
					}
			 }
			if(WiFi_Time>WIFI_5S_TIMEOUT) 
		  {
				if(Flag_Debug == DEBUG_BLE)
					{
							BLE_SendString("Read rssi error:Time OUT!\r\n");
					}
			  ClearUSARTBUF();
			  return WIFI_FAIL ;
		  }	
			if(strstr(UART_RX_BUFFER,"ERR") != NULL)
			{
				 ClearUSARTBUF();
				 WIFI_Query_rssi();
         nrf_delay_ms(10);	
			}
		}
	
}
/****************************************************************************
*	�� �� ��: WIFI_Open_TCPConnect
*	����˵��: ��TCP����
*	��    �Σ�
*	�� �� ֵ:0��ʾδ���ӣ�1��ʾ���ӣ�-1��ʾ����
* ˵    ����
*****************************************************************************/
char WIFI_Open_TCPConnect(void)
{
	  char cmd[30]="";
	
	
    sprintf(cmd, "AT+CONF=1\r");	
    WIFI_USART_Send(cmd, strlen(cmd));
	  
	  ClearUSARTBUF();
	  WiFi_Time =0;
    nrf_delay_ms(50);
	  while(1)
		{
			if(strstr(UART_RX_BUFFER,"ON") !=NULL)
			{
				 if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("TCP has opened!\r\n");
				  }
				 return WIFI_SUCCESS;			
			}	
      if(strstr(UART_RX_BUFFER,"OFF") !=NULL)
			{
          sprintf(cmd, "AT+CONF=1,ON\r");	
          WIFI_USART_Send(cmd, strlen(cmd));
				  nrf_delay_ms(50);
				  return WIFI_SUCCESS;	
			}				
			if(WiFi_Time>WIFI_5S_TIMEOUT)
			{
				 if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Open tcp  time out!\r\n");
				  }
				 return WIFI_FAIL ;
			}
		}
	
}
/****************************************************************************
*	�� �� ��: WIFI_Open_STA
*	����˵��: ����sta
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ��������STAģʽ
*****************************************************************************/
char WIFI_Open_STA(void)
{
	  uint8_t count=0;
	  char cmd[30]="";

		
	  sprintf(cmd, "AT+WLANF\r");
    WIFI_USART_Send(cmd, strlen(cmd));
		count++;
		ClearUSARTBUF();
		nrf_delay_ms(500);
		WiFi_Time = 0;
		while(1)
		{
			 if(strstr(UART_RX_BUFFER,"STA,ON") != NULL)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Open STA Mode Successed!\r\n");
				  }				
				  return WIFI_SUCCESS ;
			 }
			 if(strstr(UART_RX_BUFFER,"AP,ON") != NULL)
			 {
          sprintf(cmd, "AT+WLANF=AP,OFF\r");
          WIFI_USART_Send(cmd, strlen(cmd));
				  nrf_delay_ms(500);
				  sprintf(cmd, "AT+WLANF=STA,ON\r");
          WIFI_USART_Send(cmd, strlen(cmd));
				  ClearUSARTBUF();
				  nrf_delay_ms(500);
			 }
			 if(strstr(UART_RX_BUFFER,"STA,OFF") != NULL)  //���ô������·���
			 {
				  sprintf(cmd, "AT+WLANF=STA,ON\r");
          WIFI_USART_Send(cmd, strlen(cmd));
				  ClearUSARTBUF();
				  nrf_delay_ms(500);
			 }
			 if(WiFi_Time>WIFI_5S_TIMEOUT)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Open STA Mode time out!\r\n");
				  }
				  return WIFI_FAIL ;
			 }
		}
}
/****************************************************************************
*	�� �� ��: WIFI_Set_STAMode
*	����˵��: ����STAģʽ
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ��������ʱ���豸����ΪSTAģʽ
*****************************************************************************/
char WIFI_Set_STAMode(void)
{
	 uint8_t count=0;
	 char cmd[30]="";
		
	  sprintf(cmd, "AT+WMODE\r");
    WIFI_USART_Send(cmd, strlen(cmd));
		ClearUSARTBUF();
		nrf_delay_ms(100);
		WiFi_Time = 0;
		while(1)
		{
			 if(strstr(UART_RX_BUFFER,"OK=STA") != NULL)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("WIFI has in sta mode!\r\n");
				  }					
				  return WIFI_SUCCESS ;
			 }
			 else
			 {
				  break;
			 }
			 if(WiFi_Time>WIFI_5S_TIMEOUT)
			 {
				 break;
			 }
		}
		sprintf(cmd, "AT+WMODE=STA\r");
    WIFI_USART_Send(cmd, strlen(cmd));
		ClearUSARTBUF();
		nrf_delay_ms(200);
		WiFi_Time = 0;
		count = 0;
		while(1)
		{
			 if(strstr(UART_RX_BUFFER,"OK") != NULL)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Set WIFI sta mode Successed!\r\n");
				  }
          WIFI_SaveConfig();
					nrf_delay_ms(100);
					WIFI_Reset();
					nrf_delay_ms(2000);
					WIFI_Open_cmdMode();
				  return WIFI_SUCCESS ;
			 }
			 else
			 {
				  WIFI_USART_Send(cmd, strlen(cmd));
		      ClearUSARTBUF();
		      nrf_delay_ms(200);
          count++;
			 }
			 if((count>5)||(WiFi_Time>WIFI_5S_TIMEOUT))
			 {
				 if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Set WIFI sta mode time out!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
		}
		
}
/****************************************************************************
*	�� �� ��: WIFI_Set_STASSID
*	����˵��: ����STAģʽ���ӵ�·����ssid������
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
char WIFI_Set_STASSID(char *stassid,char *stapwd)
{
	  char cmd[100]="";
			
	  WIFI_Open_cmdMode();
		sprintf(cmd,"AT+WSTA=%s,%s\r",stassid,stapwd);
    WIFI_USART_Send(cmd, strlen(cmd));
		ClearUSARTBUF();
		nrf_delay_ms(500);
	  delaytime = 0;
		while(1)
		{
			 if(strstr(UART_RX_BUFFER,"OK") != NULL)
			 {				  				
					WIFI_OpenDHCP();
					nrf_delay_ms(100);
					WIFI_SaveConfig();
					nrf_delay_ms(100);
					WIFI_Reset();
					nrf_delay_ms(2000);
					WIFI_Open_cmdMode();
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Set ssid  Successed!\r\n");
				  }
				  return WIFI_SUCCESS ;
			 }
			 if(strstr(UART_RX_BUFFER,"+ERR=-2") != NULL)
			 {
				   WIFI_USART_Send(cmd, strlen(cmd));
		       ClearUSARTBUF();
			 }
			 if(delaytime>WIFI_5S_TIMEOUT)
			 {			 
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Set ssid  failed!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
		}
}
/****************************************************************************
*	�� �� ��: WIFI_Req_STASSID
*	����˵��: ��ѯwifiģ�����ӵ�·�������ƺ�����
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
char WIFI_Req_STASSID(void)
{
	 char cmd[20]="";
   char *pstr=NULL;
	 uint8_t len=0;
	 uint8_t i=0;
	 char buf[50];
	
	  WIFI_Open_cmdMode();
	  nrf_delay_ms (10);
		sprintf(cmd,"AT+WSTA\r");
    WIFI_USART_Send(cmd, strlen(cmd));
	  ClearUSARTBUF();
	  delaytime = 0;
	  while(1)
		{			
			 pstr = strstr(UART_RX_BUFFER,"+OK");
			 if(pstr != NULL)
			 {
				  nrf_delay_ms (100);
				  memcpy(RouterName_InWIFI,0x00,75);
				  for(i=0;i<UART_RX_LEN-6;i++)
				 {
					 RouterName_InWIFI[i]=pstr[i+4];
				 }
				  ClearUSARTBUF();
				  return WIFI_SUCCESS ;
			 }
			 if(strstr(UART_RX_BUFFER,"+ERR=-2\r\n")!=NULL)
			 {			   
				   WIFI_USART_Send(cmd, strlen(cmd));
	         ClearUSARTBUF();
			 }
			 if(delaytime>WIFI_5S_TIMEOUT)
			 {			 
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Req ssid  failed!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
		}
	
	
}
/****************************************************************************
*	�� �� ��: WIFI_Set_TCPConnectIP
*	����˵��: ����ip�Ͷ˿ں�
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
char WIFI_Set_TCPConnectIP(char *ip,char *port)
{
	  char cmd[100]= "";
	
	  sprintf(cmd,"AT+CON1=CLIENT,,%s,%s\r",port,ip);
	  WIFI_USART_Send(cmd, strlen(cmd));
	  ClearUSARTBUF();
		nrf_delay_ms(50);
	  WiFi_Time = 0;
	  if(Flag_Debug == DEBUG_BLE)
		{
			BLE_SendString(cmd);
		}
	  while(1)
		{
			 if(strstr(UART_RX_BUFFER,"OK") != NULL)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("\r\nSet IP successed!\r\n");
				  }
					WIFI_SaveConfig();
					nrf_delay_ms(100);
					WIFI_Reset();         					
				  return WIFI_SUCCESS ;
			 }
			 if(WiFi_Time>WIFI_5S_TIMEOUT)
			 {	       				 
				  WIFI_Open_cmdMode();
//				  RePowerOn_WIFI();
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Set IP failed!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
		}
	
}
/****************************************************************************
*	�� �� ��: WIFI_Query_TCPConnectSocket
*	����˵��: ��ѯSocket
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
char WIFI_Query_TCPConnectSocket(void)
{	
	  char cmd[50]="";
	  char buf[50];
	  char *pstr;
    sprintf(cmd, "AT+CONSOCKET=1\r");
    WIFI_USART_Send(cmd, strlen(cmd));
	  
	  ClearUSARTBUF();
		nrf_delay_ms(50);
	  WiFi_Time = 0;
		while(1)
		{
			 pstr=strstr(UART_RX_BUFFER,"OK");
			 if( pstr != NULL)
			 {
				 if(pstr[3] != '-')
				   Socket = pstr[3]-0x30;
				 else
					 Socket = 2;
				 if(Flag_Debug == DEBUG_BLE)
				  {
						sprintf(cmd,"Socket is:%d\r\n",Socket);
						 BLE_SendString(cmd);
				  }
				 return WIFI_SUCCESS ; 			  
			 }
			 if((strstr(UART_RX_BUFFER,"ERR") != NULL)||(WiFi_Time>WIFI_5S_TIMEOUT))
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 sprintf(buf,"Read socket failed:%s\r\n",UART_RX_BUFFER);
						 BLE_SendString(buf);
				  }
				 return WIFI_FAIL ;
			 }
			 
		}
	
}
/****************************************************************************
*	�� �� ��: WIFI_ConnectIPCompare
*	����˵��: �豸�����IP��wifi���ӵ�ip���бȽ�
*	��    �Σ�
*	�� �� ֵ:1��ʾ�ɹ�,0��ʾʧ�ܣ�-1��ʾһ������ʧ��
* ˵    ����
*****************************************************************************/
char WIFI_ConnectIPCompare(void)
{
   char err;
	 char buf[50];
	  ClearUSARTBUF();
	  WIFI_Query_TCPConnectIP();
	  WiFi_Time = 0;
	  nrf_delay_ms(50);
	  while(1)
		{
			 if( strstr(UART_RX_BUFFER,IP2) != NULL)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						sprintf(buf,"IP2 has connected!\r\n");
						BLE_SendString(buf);
				  }					
				  return WIFI_SUCCESS ;
			 }
			 if(strstr(UART_RX_BUFFER,IP2) == NULL)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						sprintf(buf,"Wifi connected error IP:%s\r\n",UART_RX_BUFFER);
						 BLE_SendString(buf);
				  }
				  return WIFI_FAIL ;
			 }
			 if(WiFi_Time>WIFI_5S_TIMEOUT)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Read wifi IP time out!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
		}
}
/****************************************************************************
*	�� �� ��: WIFI_Query_MACaddr
*	����˵��: ��ѯwifiģ��MAC��ַ
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Query_macaddr(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WMAC\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_Query_TCPConnectIP
*	����˵��: ��ѯwifiģ�����ӵķ�����IP
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Query_TCPConnectIP(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+CON1\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}

/****************************************************************************
*	�� �� ��: WIFI_Query_fwversion
*	����˵��: ��ѯ�̼��汾
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Query_fwversion(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WFVER\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_Reset
*	����˵��: reset module
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Reset(void)
{	 
    char cmd[30]="";
	   
	  ClearUSARTBUF();
    sprintf(cmd, "AT+REBOOT\r");
    WIFI_USART_Send(cmd, strlen(cmd));
	  delaytime = 0;
	  while(1)
		{
			if(strstr(UART_RX_BUFFER,"ERR") != NULL)
			{
				 WIFI_USART_Send(cmd, strlen(cmd));
				 ClearUSARTBUF();
			}
			if(strstr(UART_RX_BUFFER,"OK") != NULL)
			{
				 Flag_DataSendMode = WIFI_DATASEND_TRANSMODE;
         return ;
			}
			if(delaytime>WIFI_5S_TIMEOUT)
			{
				 return ;
			}
		}
	  
}
/****************************************************************************
*	�� �� ��: WIFI_Restore
*	����˵��: restore to factory mode
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Restore(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+FCLR\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_SaveConfig
*	����˵��: ����������Ϣ
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_SaveConfig(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+SAVE\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_SetRFLowPower
*	����˵��: ����RF�͹���
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_SetRFLowPower(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+PRSLP=ON\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_OpenDHCP
*	����˵��: ����DHCP
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_OpenDHCP(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+DHCP=ON\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_Query_constatus
*	����˵��: ��ѯSTA����״̬
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����ģ�鷵��,+OK=UAP_DOWN,STATION_UP
*****************************************************************************/
void WIFI_Query_STAStatus(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WSTATUS\r");
    WIFI_USART_Send(cmd, strlen(cmd));
	 
}

/****************************************************************************
*	�� �� ��: WIFI_Query_rssi
*	����˵��: ��ѯ��ǰ���ӵ��ź�ǿ�� -- sta mode only
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����+OK=1,-45,11
*****************************************************************************/
void WIFI_Query_rssi(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WLINK\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}

/****************************************************************************
*	�� �� ��: WIFI_Query_IPconifg
*	����˵��: query module's ip info
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Query_IPconifg(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+IPCONFIG\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}

/****************************************************************************
*	�� �� ��: WIFI_Query_tcpstatus
*	����˵��: query tcp status
*	��    �Σ�
*	�� �� ֵ: 0��ʾδ���ӣ�1��ʾ���ӣ�-1��ʾ����
* ˵    ����+OK=0��1��0��0
*****************************************************************************/
char WIFI_Query_TCPStatus(void)
{
    char cmd[30]="";
	  char *pstr;
    sprintf(cmd, "AT+CONS\r");	
    WIFI_USART_Send(cmd, strlen(cmd));
	
	  ClearUSARTBUF();
		nrf_delay_ms(50);
	  WiFi_Time = 0;
		while(1)
		{
	     pstr=strstr(UART_RX_BUFFER,"OK");
			 if( pstr != NULL)
			 {
				  if(pstr[5]== '1')
					{
						if(Flag_Debug == DEBUG_BLE)
						{
							 BLE_SendString("TCP has connected!\r\n");
						}
						return WIFI_TCP_CONNECT ;
					}
					if(pstr[5]== '0')
					{
						if(Flag_Debug == DEBUG_BLE)
						{
							 BLE_SendString("TCP has not connected!\r\n");
						}
						return WIFI_TCP_NOCONNECT ;
					}
			 }
			 if(strstr(UART_RX_BUFFER,"ERR") != NULL)
			 {
				  WIFI_USART_Send(cmd, strlen(cmd));	
	        ClearUSARTBUF();
			 }
			 if(WiFi_Time>WIFI_5S_TIMEOUT)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Read tcp status failed!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
	  }
}
/****************************************************************************
*	�� �� ��: WIFI_Open_Event
*	����˵��: ���¼�֪ͨ
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_Open_Event(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+EVENT=ON\r");	
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	�� �� ��: WIFI_Send_data
*	����˵��: send data via socket
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
char WIFI_Send_data(uint8_t sock_fd, uint16_t send_len, uint8_t *buf)
{
    char cmd[50]="";

	  ClearUSARTBUF();
//	  sprintf(cmd,"AT+SSEND=%d,%d\r",sock_fd,send_len);
	  sprintf(cmd,"AT+SSEND=%d,%d\r",0,send_len);   //socket = 0�������е�socket��������
	  WIFI_USART_Send(cmd,strlen(cmd));	  
	  WiFi_Time = 0;
	  while(1)
		{
			if(strstr(UART_RX_BUFFER,">")!= NULL)
			{				 
			   WIFI_USART_Send(buf,send_len);
				ClearUSARTBUF();
				if(Flag_Debug == DEBUG_BLE)
				{
					 BLE_SendString("Send data to server over!\r\n");
				}
				return WIFI_SUCCESS ;				
			}
//			if(strstr(UART_RX_BUFFER,"OK")!= NULL)
//			{
//				if(Flag_Debug == DEBUG_BLE)
//				{
//					 BLE_SendString("Send data to server over!\r\n");
//				}
//				return WIFI_SUCCESS ;				
//			}
			if(strstr(UART_RX_BUFFER,"ERR=-1") != NULL)
			 {
				  ClearUSARTBUF();
				  WIFI_USART_Send(cmd,strlen(cmd));
			 }
//			 if(strstr(UART_RX_BUFFER,"ERR=-6") != NULL)
//			 {
//				  ClearUSARTBUF();
//				  WIFI_Query_TCPConnectSocket();
//				  sprintf(cmd,"AT+SSEND=%d,%d\r",sock_fd,send_len);
//				  WIFI_USART_Send(cmd,strlen(cmd));	
//			 }
			if(WiFi_Time> WIFI_3S_TIMEOUT)
			 {
				  if(Flag_Debug == DEBUG_BLE)
				  {
						 BLE_SendString("Send data to server failed!\r\n");
				  }
				 return WIFI_FAIL ;
			 }
		}			
}

/****************************************************************************
*	�� �� ��: WIFI_USART_Send
*	����˵��: WiFi���ڷ�������
*	��    �Σ�tx_buf��Ҫ���͵�����  buflen���͵����ݳ���
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void WIFI_USART_Send(char *tx_buf,uint16_t buflen)
{
    uint16_t i;
    uint32_t err_code;
	 
		for (i=0; i<buflen; i++)
		{
			 err_code=app_uart_put(tx_buf[i]);
			 if(err_code != NRF_SUCCESS)
			 {
				  err_code=app_uart_put(tx_buf[i]);
			 }
		}  
}
