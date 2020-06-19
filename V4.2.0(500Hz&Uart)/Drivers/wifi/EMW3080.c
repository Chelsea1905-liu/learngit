/******************************************************************************
 * @file     EMW3080.c
 * @brief   WiFi通讯函数
 * @version  
 * @date     2017.11
 * @note
 * Copyright (C)  
 *
 * @par      张炜201711
 *    V1.0    EMW3080 WiFi模块通讯程序
*******************************************************************************/


#include "EMW3080.h"



#define htons(n) 				(((n & 0xff) << 8) | ((n & 0xff00) >> 8))
#define htonl(n) 				(((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000UL) >> 8) | ((n & 0xff000000UL) >> 24))
#define ntohs(n) 				htons(n)
#define ntohl(n) 				htonl(n)


uint8_t AP_Channel=0;          //AP连接的信道

/****************************************************************************
*	函 数 名: WIFI_INIT
*	功能说明: 无线模块初始化
*	形    参：
*	返 回 值:返回初始化完成的步骤
* 说    明：
*****************************************************************************/
char WIFI_INIT(void)
{
	 uint8_t buf[5];
	
//-------进入AT指令模式---------------------------------
	 if(Flag_WIFIInitStep == WIFI_INIT_STEP1)
	 {	 
		 if(WIFI_Open_cmdMode()==WIFI_SUCCESS)
		 {
			 if(Flag_PowerOn == 1)
			   Flag_WIFIInitStep = WIFI_INIT_STEP2;
			 else
				 Flag_WIFIInitStep = WIFI_INIT_STEP3;
		 }
		 else     //AT指令模式无法进入，则复位模块
		 {
				if(WiFi_InitTime>=NETWORKLINK_TIMEOUT_30S)
				 {
					  app_uart_close();
						WIFI_POWER(0);
						nrf_delay_ms(5000);
						WIFI_POWER(1);
					  nrf_delay_ms(500);	
	          UART_CONFIG(UART_WIFI);
						Flag_DataSendMode = WIFI_DATASEND_TRANSMODE;   //透传模式
						WiFi_InitTime = 0;
				 }
        nrf_delay_ms(2000);				 
			  return WIFI_INIT_STEP1;
		 } 
   }	 
//-------wifi模块设置---------------------------------
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
//-------查询路由器连接状态---------------------------------	 
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
							WIFI_Set_STASSID(Router_Name,Router_PWD);  //使用保存的wifi名称重新配置
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
							WIFI_Set_STASSID(Router_Name,Router_PWD);  //使用保存的wifi名称重新配置
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
*	函 数 名: WIFI_Open_cmdMode
*	功能说明: enter AT command mode from easytxrx mode
*	形    参：
*	返 回 值: 0表示成功
* 说    明：
*****************************************************************************/
char WIFI_Open_cmdMode(void)
{
    int8_t	     retval=0;
    uint8_t	     retry_times=0;	   //重试次数设置
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
    WIFI_USART_Send("+++",3);//发送打开模块命令接口请求
		WiFi_Time = 0;
		nrf_delay_ms(300);   //等待300ms查看模块有没有返回A;	
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
					if(strstr(UART_RX_BUFFER,"OK")!=NULL)  //成功进入AT指令模式
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
          WIFI_USART_Send("+++",3);//发送打开模块命令接口请求
				  retry_times++;
					WiFi_Time=0;				 
		      nrf_delay_ms(300);   //等待300ms查看模块有没有返回A;	
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
*	函 数 名: WIFI_STA_Status
*	功能说明: 查询STA连接状态
*	形    参：
*	返 回 值: 0表示成功
* 说    明：
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
*	函 数 名: WIFI_Query_STA_RSSI
*	功能说明: 查询STA模式下rssi无线网络信号强度
*	形    参：
*	返 回 值: 无线信号强度数组
* 说    明：查询STA连接状态，AP信号强度，AP通道 
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
				  if(pstr[3] == '1')   //STA已连接 ，读取到+OK=1,-20,1
					{
						 len1 = strcspn(UART_RX_BUFFER,",")+1;		 //第一个“，”号位置			
						 memcpy(rebuf,pstr+len1,UART_RX_LEN-len1); //获取到字符串20,1
						len2 = strcspn(rebuf,",");                 //字符串20,1中“，”号位置
						memcpy(csqstring,rebuf,len2);              //获取到20
						memcpy(apchstring,&rebuf[len2+1],UART_RX_LEN-len1-len2-1);//获取到1
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
*	函 数 名: WIFI_Open_TCPConnect
*	功能说明: 打开TCP连接
*	形    参：
*	返 回 值:0表示未连接，1表示连接，-1表示错误
* 说    明：
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
*	函 数 名: WIFI_Open_STA
*	功能说明: 启动sta
*	形    参：
*	返 回 值: 
* 说    明：进入STA模式
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
			 if(strstr(UART_RX_BUFFER,"STA,OFF") != NULL)  //配置错误重新发送
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
*	函 数 名: WIFI_Set_STAMode
*	功能说明: 设置STA模式
*	形    参：
*	返 回 值: 
* 说    明：开机时将设备配置为STA模式
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
*	函 数 名: WIFI_Set_STASSID
*	功能说明: 设置STA模式连接的路由器ssid和密码
*	形    参：
*	返 回 值: 
* 说    明：
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
*	函 数 名: WIFI_Req_STASSID
*	功能说明: 查询wifi模块连接的路由器名称和密码
*	形    参：
*	返 回 值: 
* 说    明：
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
*	函 数 名: WIFI_Set_TCPConnectIP
*	功能说明: 设置ip和端口号
*	形    参：
*	返 回 值: 
* 说    明：
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
*	函 数 名: WIFI_Query_TCPConnectSocket
*	功能说明: 查询Socket
*	形    参：
*	返 回 值: 
* 说    明：
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
*	函 数 名: WIFI_ConnectIPCompare
*	功能说明: 设备保存的IP和wifi链接的ip进行比较
*	形    参：
*	返 回 值:1表示成功,0表示失败，-1表示一键配置失败
* 说    明：
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
*	函 数 名: WIFI_Query_MACaddr
*	功能说明: 查询wifi模块MAC地址
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_Query_macaddr(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WMAC\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_Query_TCPConnectIP
*	功能说明: 查询wifi模块连接的服务器IP
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_Query_TCPConnectIP(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+CON1\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}

/****************************************************************************
*	函 数 名: WIFI_Query_fwversion
*	功能说明: 查询固件版本
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_Query_fwversion(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WFVER\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_Reset
*	功能说明: reset module
*	形    参：
*	返 回 值: 
* 说    明：
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
*	函 数 名: WIFI_Restore
*	功能说明: restore to factory mode
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_Restore(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+FCLR\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_SaveConfig
*	功能说明: 保存配置信息
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_SaveConfig(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+SAVE\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_SetRFLowPower
*	功能说明: 设置RF低功耗
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_SetRFLowPower(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+PRSLP=ON\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_OpenDHCP
*	功能说明: 开启DHCP
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_OpenDHCP(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+DHCP=ON\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_Query_constatus
*	功能说明: 查询STA连接状态
*	形    参：
*	返 回 值: 
* 说    明：模块返回,+OK=UAP_DOWN,STATION_UP
*****************************************************************************/
void WIFI_Query_STAStatus(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WSTATUS\r");
    WIFI_USART_Send(cmd, strlen(cmd));
	 
}

/****************************************************************************
*	函 数 名: WIFI_Query_rssi
*	功能说明: 查询当前连接的信号强度 -- sta mode only
*	形    参：
*	返 回 值: 
* 说    明：+OK=1,-45,11
*****************************************************************************/
void WIFI_Query_rssi(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+WLINK\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}

/****************************************************************************
*	函 数 名: WIFI_Query_IPconifg
*	功能说明: query module's ip info
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_Query_IPconifg(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+IPCONFIG\r");
    WIFI_USART_Send(cmd, strlen(cmd));
}

/****************************************************************************
*	函 数 名: WIFI_Query_tcpstatus
*	功能说明: query tcp status
*	形    参：
*	返 回 值: 0表示未连接，1表示连接，-1表示错误
* 说    明：+OK=0，1，0，0
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
*	函 数 名: WIFI_Open_Event
*	功能说明: 打开事件通知
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void WIFI_Open_Event(void)
{
    char cmd[30]="";
    sprintf(cmd, "AT+EVENT=ON\r");	
    WIFI_USART_Send(cmd, strlen(cmd));
}
/****************************************************************************
*	函 数 名: WIFI_Send_data
*	功能说明: send data via socket
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
char WIFI_Send_data(uint8_t sock_fd, uint16_t send_len, uint8_t *buf)
{
    char cmd[50]="";

	  ClearUSARTBUF();
//	  sprintf(cmd,"AT+SSEND=%d,%d\r",sock_fd,send_len);
	  sprintf(cmd,"AT+SSEND=%d,%d\r",0,send_len);   //socket = 0，向所有的socket发生数据
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
*	函 数 名: WIFI_USART_Send
*	功能说明: WiFi串口发送数据
*	形    参：tx_buf需要发送的数组  buflen发送的数据长度
*	返 回 值: 
* 说    明：
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
