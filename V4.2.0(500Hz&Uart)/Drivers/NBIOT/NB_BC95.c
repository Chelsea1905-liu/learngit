/****************************************************************************
 * @file     NB_BC95.C
 * @brief    NB_IOTģ��BC95��������
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 ���
 *
 * @par     NB-IOTģ����������
 * 
*****************************************************************************/
#include "NB_BC95.h"



/****************************************************************************
*	�� �� ��: BC95_GPIO_CONFIG
*	����˵��: ����BC95��IO����
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void BC95_GPIO_CONFIG(void)
{	
	 nrf_gpio_cfg_output_pullup(NB_BC95_RESET_PIN);
   nrf_gpio_cfg_output_pullup(NB_BC95_POWER_PIN); 
//   BC95_POWER_OFF;	
//	 nrf_delay_ms(2000);
	
	 BC95_POWER_ON;	
	 BC95_RESET_OFF;   
}
/****************************************************************************
*	�� �� ��: BC95_SEND_STRING
*	����˵��: ͨ��NBģ�鷢���ַ���
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void BC95_SEND_STRING(uint8_t *str)
{
  uint8_t len ,i;

	while(*str != '\0')
	{
	  app_uart_put(*str++);		
	}
  app_uart_put(0x0D);
	app_uart_put(0x0A);
		
}
/****************************************************************************
*	�� �� ��: BC95_Reset
*	����˵��: ģ�����
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����ָ��ͳɹ���ģ����ӦREBOOTING��������ģ�����REBOOT_CAUSE_APPLICATION_AT
*                                                        Neul 
*                                                        OK
*****************************************************************************/
char  BC95_Reset(void)
{

	  BC95_SEND_STRING("AT+NRB");
	  delaytime = 0;
	  ClearNBUSARTBUF();
	  nrf_delay_ms(50);
//	  while(delaytime<5)
//		{
//			  if(strstr(NBUART_RX_BUFFER,"REBOOTING")!=NULL)
//				{
//				   ClearNBUSARTBUF();
//					 return 1;
//					
//				}			
//		}
//	 	ClearNBUSARTBUF();
		return 0;
}

/****************************************************************************
*	�� �� ��: BC95_Set_CEREGReturnFormat
*	����˵��: ����CEREG���ظ�ʽ
*	��    �Σ�format  ��ʽ
*	�� �� ֵ: 
* ˵    ����0��ʾ���ظ�ʽΪ+CEREG:0,1,
            1��ʾ���ظ�ʽΪ+CEREG:1,1,
            2��ʾ���ظ�ʽΪ+CEREG:2,1,77C3,D2A3152,7
*****************************************************************************/
char  BC95_Set_CEREGReturnFormat(uint8_t format)
{

	  char printfstr[30];
	
	  sprintf(printfstr,"AT+CEREG=%d",format);
//	  printf("%s",printfstr);
	   delaytime = 0;
	   ClearNBUSARTBUF();
		 BC95_SEND_STRING(printfstr);
		 Command_SendCount++;	 
		 nrf_delay_ms(50);
	  while(delaytime<3)
		{
			  if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
				{
					if(Flag_COMDebug == 1)
				 {	
					 sprintf(printfstr,"Set CEREG=%d OK!\r\n",format);									 
					 BLE_SendString(printfstr);
				 }
				   ClearNBUSARTBUF();
					 return 1;
				}
			  if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
				{
//					 printf("%s",printfstr);
					 BC95_SEND_STRING(printfstr);
					 Command_SendCount++;
					 nrf_delay_ms(50);
				}	
		}	
		if(Flag_COMDebug == 1)
		{	
			BLE_SendBufData(NBUART_RX_BUFFER,NBUART_RX_LEN);
			sprintf(printfstr,"Set CEREG=%d Time out:%s\r\n",format,NBUART_RX_BUFFER);									 
			BLE_SendString(printfstr);
		}
		 ClearNBUSARTBUF();
		 return 0;
}
/****************************************************************************
*	�� �� ��: BC95_Query_CEREG
*	����˵��: ��ѯ����ע��״̬
*	��    �Σ���
*	�� �� ֵ: ��������ע��״̬
* ˵    ����0��ʾ���ظ�ʽΪ+CEREG:0,1,
            1��ʾ���ظ�ʽΪ+CEREG:1,1,
            2��ʾ���ظ�ʽΪ+CEREG:2,1,77C3,D2A3152,7
*****************************************************************************/
char  BC95_Inquire_CEREG(void)
{
	  uint8_t i,len;
	  uint8_t redata[30];
	  unsigned char tacstring[5]={0};
		unsigned char cistring[10]={0};
	  char printfstr[50];
	  char *pstr = NULL;
		
		ClearNBUSARTBUF();
	  delaytime = 0; 
		Command_SendCount=0;
    BC95_SEND_STRING("AT+CEREG?");
		nrf_delay_ms(50);  
	  while(delaytime<3)
		{
			 if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
			 {
				 pstr = strstr(NBUART_RX_BUFFER,"+CEREG");			 
				 if( pstr != NULL)   //������״̬
					 {
						  redata[0] = pstr[7];						  
						  if((redata[0] == 0x30)||(redata[0] == 0x31))
							{
								  redata[1] =  pstr[9]-0x30;
								if(Flag_COMDebug == 1)
								 {	
                   sprintf(printfstr,"CEREG:%d\r\n",redata[1]);									 
									 BLE_SendString(printfstr);
								 }
                 ClearNBUSARTBUF();
								 return redata[1];
							}
						  if(redata[0] == 0x32)
							{
								redata[1] =  pstr[9]-0x30;
								for(i=0;i<12;i++)
								{
									redata[2+i] = pstr[11+i];
								}
								memcpy(tacstring,pstr+11,4);
								StringToHex(tacstring,4,Position_TAC);
				        memcpy(cistring,pstr+16,8);	
								if( cistring[7] == 0x2C)
								{
								  StringToHex(cistring,7,Position_CI);
								}
								else
								{
									StringToHex(cistring,8,Position_CI);
								}
								if(Flag_COMDebug == 1)
								 {	
                   sprintf(printfstr,"CEREG:%d\r\n",redata[1]);									 
									 BLE_SendString(printfstr);
									 sprintf(printfstr,"tac:%02x%02x\r\n",Position_TAC[0],Position_TAC[1]);									 
									 BLE_SendString(printfstr);
									 sprintf(printfstr,"ci:%02x%02x%02x%02x\r\n",Position_CI[0],Position_CI[1],Position_CI[2],Position_CI[3]);									 
									 BLE_SendString(printfstr);
								 }
								 ClearNBUSARTBUF();
								 return redata[1];
							}
					  
					 }
          else
					 {            
							 BC95_SEND_STRING("AT+CEREG?");
						   Command_SendCount++;
							 ClearNBUSARTBUF();
							 nrf_delay_ms(50);
					 }						 
			 }
			  if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
				{
					 BC95_SEND_STRING("AT+CEREG?");
					 Command_SendCount++;
	         ClearNBUSARTBUF();
	         nrf_delay_ms(50);
				}
		}
		if(Flag_COMDebug == 1)
		 {						
			 BLE_SendString("Read CEREG Error!\r\n");
		 }
		 ClearNBUSARTBUF();
		 return -1;
}
/****************************************************************************
*	�� �� ��: StringToHex
*	����˵��: ���ַ�����ʾ��16��������ת��Ϊ16��������
*	��    �Σ�*strת��֮ǰ���ַ�����lenת�����ַ������ȣ�*hexdata����ת�����16��������
*	�� �� ֵ: ��
* ˵    ��������37 37 43 33ת��Ϊ77C3
*****************************************************************************/
void StringToHex(uint8_t *str,uint8_t len,uint8_t *hexdata)
{
	 uint8_t i = 0;
	 uint8_t dat[len];
	
	 if(len%2 == 0)  //ż����
	 {
			 for(i=0;i<len;i++)
			 {
					if((*(str+i)>=0x30)&&(*(str+i)<=0x39))  //����
					{
						 dat[i] = *(str+i)-0x30;
					}
					else if((*(str+i)>=0x41)&&(*(str+i)<=0x46))  //��д�ַ�A~F
					{
						 dat[i] = *(str+i)-0x41+0x0A;
					}
					else if((*(str+i)>=0x61)&&(*(str+i)<=0x66))  //Сд�ַ�a~f
					{
						 dat[i] = *(str+i)-0x61+0x0A;
					}
					else   //������������󣬸�ֵ0
					{
						 dat[i] = 0;
					}
			 }
			 for(i=0;i<len/2;i++)
			 {
					hexdata[i] = (dat[i*2]<<4)|dat[i*2+1];
			 }
   }
	 else   //����Ϊ����
	 {
		 dat[0] = 0;
		 for(i=0;i<len;i++)
			 {
					if((*(str+i)>=0x30)&&(*(str+i)<=0x39))  //����
					{
						 dat[i+1] = *(str+i)-0x30;
					}
					else if((*(str+i)>=0x41)&&(*(str+i)<=0x46))  //��д�ַ�A~F
					{
						 dat[i+1] = *(str+i)-0x41+0x0A;
					}
					else if((*(str+i)>=0x61)&&(*(str+i)<=0x66))  //Сд�ַ�a~f
					{
						 dat[i+1] = *(str+i)-0x61+0x0A;
					}
					else   //������������󣬸�ֵ0
					{
						 dat[i+1] = 0;
					}
			 }
			 for(i=0;i<(len+1)/2;i++)
			 {
					hexdata[i] = (dat[i*2]<<4)|dat[i*2+1];
			 }
		 
	 }
}
/****************************************************************************
*	�� �� ��: BC95_InquireCSQ
*	����˵��: ��ѯ�ź�����ֵ
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
char BC95_InquireCSQ(void)
{

	uint8_t i,len;
  char csqstring[5]={0};
	char *pcsq;
  char printfstr[50];

	
	Command_SendCount = 0;
	ClearNBUSARTBUF();
  BC95_SEND_STRING("AT+CSQ");
	nrf_delay_ms(50);
  delaytime = 0;
	while(1)                    
	{		
			if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
			{								
				if(strstr(NBUART_RX_BUFFER,"+CSQ") != NULL) 
				{ 
					pcsq = strstr(NBUART_RX_BUFFER,"+CSQ:");	
					memcpy(csqstring,pcsq+5,3);
					CSQNual = atoi(csqstring);
	//				if((CSQNual < 99)&&(CSQNual > 0))
					{
						if(Flag_COMDebug == 1)
						{						
							 sprintf(printfstr,"CSQ:%d\r\n",CSQNual);
							 BLE_SendString(printfstr);
						}
						ClearNBUSARTBUF(); 
						return 1;
					}
	//				else
	//				{
	//					printf("AT+CSQ\r\n");
	//          ClearNBUSARTBUF();
	//					nrf_delay_ms(50);
	//				}
				}			
			}
			if(strstr(NBUART_RX_BUFFER,"ERROR") != NULL)
			{	
				BC95_SEND_STRING("AT+CSQ");
				Command_SendCount++;
				ClearNBUSARTBUF();
				nrf_delay_ms(50);
			}	
			if(delaytime>3)
			{
					if(Flag_COMDebug == 1)
						{						
							 BLE_SendString("CSQ Timer Out\r\n");
						}	
					ClearNBUSARTBUF(); 				
					return 0;  
			}			
	}  
}
/****************************************************************************
*	�� �� ��: SetAutoConnect
*	����˵��: �����Զ�����
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����ʹ��AT+CFUN��ѯ���������+CFUN:0����Ҫʹ��AT+CFUN=1����
*****************************************************************************/
char SetAutoConnect(void)
{
	  uint8_t i;

	  ClearNBUSARTBUF();
	  BC95_SEND_STRING("AT+CFUN?");
	  delaytime = 0;
	  nrf_delay_ms(50);
	  while(1)
		{
			 if(strstr(NBUART_RX_BUFFER,"+CFUN:1")!=NULL)
			 {	
				 if(Flag_COMDebug == 1)
					{						
						 BLE_SendString("CFUN:1\r\n");
					}
         ClearNBUSARTBUF();					
				 return 1;
			 }
			 if(strstr(NBUART_RX_BUFFER,"+CFUN:0")!=NULL)
			 {
				   AutoConnectConfig();
				   BC95_SEND_STRING("AT+CFUN==1\r\n");
				   ClearNBUSARTBUF();
	         nrf_delay_ms(50);
//				   while(delaytime<5)
//					 {
//							 if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
//							 {				 
//								 return 2;
//							 }
//							 if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
//							 {				 
//								  BC95_SEND_STRING("AT+CFUN==1\r\n");
//				          ClearNBUSARTBUF();
//	                nrf_delay_ms(50);
//							 }						 
//					 }
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
			 {
				   BC95_SEND_STRING("AT+CFUN==1\r\n");
				   ClearNBUSARTBUF();
	         nrf_delay_ms(50);
			 }
			 if(delaytime>3)
			 {
				  if(Flag_COMDebug == 1)
					{				
						BLE_SendBufData(NBUART_RX_BUFFER,NBUART_RX_LEN);
						BLE_SendString("Read CFUN time out!\r\n");
					}
				 ClearNBUSARTBUF();
				 return 0;
			 }
		}		
}
/****************************************************************************
*	�� �� ��: AutoConnectConfig
*	����˵��: �Զ���������
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
char  AutoConnectConfig(void)
{

//	  printf("AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
	  delaytime = 0;
	  Command_SendCount=0;
	  ClearNBUSARTBUF();
		BC95_SEND_STRING("AT+NCONFIG=AUTOCONNECT,TRUE");				
		nrf_delay_ms(100);
	  while(delaytime<3)
		{
			 if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
			 {				 
				 return 1;
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
			 {
				  BC95_SEND_STRING("AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
				  Command_SendCount++;
					ClearNBUSARTBUF();
					nrf_delay_ms(100);
			 }
		}
	  ClearNBUSARTBUF();
		return 0;
}
/****************************************************************************
*	�� �� ��: BC95_InquireCGATT
*	����˵��: ��ѯ���總��״̬
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ������Ӧ+CGATT:1��ʾ���������ӣ�+CGATT:0��ʾ����δ����
*****************************************************************************/
char BC95_InquireCGATT(void)
{

//	  printf("AT+CGATT?\r\n");
	  delaytime = 0;
	  Command_SendCount=0;
	  ClearNBUSARTBUF();
    BC95_SEND_STRING("AT+CGATT?");
	  nrf_delay_ms(50);
	  while(delaytime<3)
		{
			 if(strstr(NBUART_RX_BUFFER,"+CGATT:1")!=NULL)
			 {	
          if(Flag_COMDebug == 1)
					{						
						 BLE_SendString("CGATT:1\r\n");
					}	
          ClearNBUSARTBUF();					
				  return 1;
			 }
			 if(strstr(NBUART_RX_BUFFER,"+CGATT:0")!=NULL)
			 {	
          if(Flag_COMDebug == 1)
					{						
						 BLE_SendString("CGATT:0\r\n");
					}	
          ClearNBUSARTBUF();					
				  return 0;
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
			 {
				  BC95_SEND_STRING("AT+CGATT?");
				  Command_SendCount++;
					ClearNBUSARTBUF();
					nrf_delay_ms(50);
			 }
		}
		if(Flag_COMDebug == 1)
       BLE_SendBufData(NBUART_RX_BUFFER,NBUART_RX_LEN);
    ClearNBUSARTBUF();		
		return -1;
}
/****************************************************************************
*	�� �� ��: BC95_InquireIMSI
*	����˵��: ��ѯsim����IMSI��
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ������Ӧ+CGATT:1��ʾ���������ӣ�+CGATT:0��ʾ����δ����
*****************************************************************************/
char  BC95_InquireIMSI(void)
{
    char *pstr =NULL;
	  char printfstr[50];


	  delaytime = 0;
	  Command_SendCount=0;
	  ClearNBUSARTBUF();
	  BC95_SEND_STRING("AT+CIMI");
	  nrf_delay_ms(50);
	  while(delaytime<3)
		{		 
			 if(strstr(NBUART_RX_BUFFER,"OK")!= NULL)
			 {
				  pstr = strstr(NBUART_RX_BUFFER,"460");
				  SIMCard_IMSI_Len = 0;
				  while(*pstr != 0x0D)
				  {
            SIMCard_IMSI[SIMCard_IMSI_Len++] = *pstr++;
				  }
          SIMCard_IMSI[SIMCard_IMSI_Len]= '\0';					
				  if(Flag_COMDebug == 1)
					{						
						sprintf(printfstr,"IMSI:%s\r\n",SIMCard_IMSI);
						BLE_SendString(printfstr);
					}
					ClearNBUSARTBUF();
					return 1;
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR") != NULL)
			 {			 
				  BC95_SEND_STRING("AT+CIMI");
				  Command_SendCount++;
	        ClearNBUSARTBUF();
	        nrf_delay_ms(50);
			 }
		}
	if(Flag_COMDebug == 1)
	{						
		sprintf(printfstr,"IMSI time out:%s\r\n",NBUART_RX_BUFFER);
		BLE_SendString(printfstr);
	}
	ClearNBUSARTBUF();
	return 1;
}
/****************************************************************************
*	�� �� ��: BC95_InquireNetworkTime
*	����˵��: ��ѯ����ʱ��
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
char BC95_InquireNetworkTime(void)
{
	 char *pstr = NULL;
	 
	 uint8_t timebuf[17];
	
		  delaytime = 0;
	  Command_SendCount=0;
	  ClearNBUSARTBUF();
	  BC95_SEND_STRING("AT+CCLK?");
	  nrf_delay_ms(50);
	  while(delaytime<3)
		{		 
			 if(strstr(NBUART_RX_BUFFER,"OK")!= NULL)
			 {
				  pstr = strstr(NBUART_RX_BUFFER,"+CCLK");
				  if(pstr != NULL)
					{
						memcpy(timebuf,pstr+6,17);
						RealTime[0]=(timebuf[0]-0x30)*10+timebuf[1]-0x30; 
						RealTime[1] =(timebuf[3]-0x30)*10+timebuf[4]-0x30;
						RealTime[2] =(timebuf[6]-0x30)*10+timebuf[7]-0x30;
						RealTime[3] =(timebuf[9]-0x30)*10+timebuf[10]-0x30;
						RealTime[4] =(timebuf[12]-0x30)*10+timebuf[13]-0x30;
						RealTime[5]=(timebuf[15]-0x30)*10+timebuf[16]-0x30;
						RealTimeZoneConvert();
						ClearNBUSARTBUF();
						return 1;
					}
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR") != NULL)
			 {			 
	        BC95_SEND_STRING("AT+CCLK?");
				  Command_SendCount++;
					ClearNBUSARTBUF();
				  ClearNBUSARTBUF();
					nrf_delay_ms(50);
			 }
		}
	return 0;
}
/****************************************************************************
*	�� �� ��: BC95_Init
*	����˵��: ģ���ʼ��
*	��    �Σ���
*	�� �� ֵ: ��ʼ���Ƿ����
* ˵    ����
*****************************************************************************/
char  BC95_Init(void)
{
	 char err;
 	 
		err=SetAutoConnect();   //�����Զ�����
	  nrf_delay_ms(200);
		if( err== 0)
		{
			return 0;			
		}		
		err=BC95_Set_CEREGReturnFormat(2);
		nrf_delay_ms(200);
		if( err== 0)
		{
			return 0;			
		}
		err = BC95_InquireCGATT();
		nrf_delay_ms(200);
		if( err== 0)
		{
			return 0;			
		}
		err = BC95_Inquire_CEREG();
		nrf_delay_ms(200);
		if(err== 0)
		{
			return 0;			
		}   	 
	 return 1;
}

/****************************************************************************
*	�� �� ��: BC95_CreateSocket
*	����˵��: ����socket
*	��    �Σ���
*	�� �� ֵ: ģ�鷵�ص�socket,ΪASCII�ַ�
* ˵    ����AT+NSOCR=<type>,<protocol>,<listen port>[,<receive control>]
<type>         Socket Type. Supported value is DGRAM 
<protocol>       Standard internet protocol definition. For example, UDP is 17 
<listen port>      A number in the range 0-65535. This is the local port that will be included in sent 
          messages and on which messages will be received. 
<socket>        This is a reference to the created socket. It is an integer greater than or equal to 0. 
<receive control>    Set to 1 if incoming messages should be received, 0 if incoming messages should 
          be ignored. Default is 1 (messages will be received). 
*****************************************************************************/
uint8_t BC95_CreateSocket(void)
{
	  char *pstr =NULL;
	  char printfstr[50];
	  uint16_t port = 1000;
	  uint8_t soc;
	  uint16_t  i = 0;
	

	   sprintf(printfstr,"AT+NSOCR=DGRAM,17,%d,1",port);
	   delaytime = 0;
	   Command_SendCount=0;
	   ClearNBUSARTBUF();
	   BC95_SEND_STRING(printfstr);
	   nrf_delay_ms(50);
		while(delaytime<10)                    
		{		
			if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
			{								
					for(i=0;i<NBUART_RX_LEN;i++)    //��ȡsocket
				  {
					   if((NBUART_RX_BUFFER[i]>=0x30)&&((NBUART_RX_BUFFER[i]<0x38)))
						 {
							  soc = NBUART_RX_BUFFER[i]-0x30;
							  break;
						 }
				  }
				 if(Flag_COMDebug == 1)
					{						
						sprintf(printfstr,"Socket is:%d\r\n",soc);
						BLE_SendString(printfstr);
					}
					ClearNBUSARTBUF();
					return soc;
				
			}
			if(strstr(NBUART_RX_BUFFER,"ERROR") != NULL)
			{	
	 				  BC95_CloseSocket(Socket);
					  Command_SendCount++;
					  BC95_SEND_STRING("AT+NSOCR=DGRAM,17,1000,1");
		        ClearNBUSARTBUF();
		        nrf_delay_ms(50);
			}				
		}
		if(Flag_COMDebug == 1)
		{		
      sprintf(printfstr,"Create socket time out:%s\r\n",NBUART_RX_BUFFER);			
			BLE_SendString(printfstr);
		}
	 ClearNBUSARTBUF();
	 return 0;
}
/****************************************************************************
*	�� �� ��:  BC95_CloseSocket
*	����˵��: �ر�socket
*	��    �Σ�soc ��Ҫ�رյ�socket���
*	�� �� ֵ: ��
* ˵    ����
*****************************************************************************/
uint8_t BC95_CloseSocket(uint8_t soc)
{
	  char printfstr[50];
    uint8_t count = 0;
	
	  sprintf(printfstr,"AT+NSOCL=%d",soc);
//		BC95_SEND_STRING(printfstr);
		delaytime1 = 0;
	  Command_SendCount=0;
//		ClearNBUSARTBUF();
//		nrf_delay_ms(50);
	  while(delaytime1<4)
		{		
			 if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
			 {
				    if(Flag_COMDebug == 1)
						{		
							sprintf(printfstr,"Close socket %d is OK!\r\n",soc);			
							BLE_SendString(printfstr);
						}
				  	 ClearNBUSARTBUF();
	           return 1;
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
			 {
				    BC95_SEND_STRING(printfstr);
				    Command_SendCount++;
		        ClearNBUSARTBUF();
	          
			 }
			 if((delaytime1%2 == 0)&&(Command_SendCount==0))
			 {
					 BC95_SEND_STRING(printfstr);
					 Command_SendCount++;
					 ClearNBUSARTBUF();
					 nrf_delay_ms(50);
			 }
	  }
	 if(Flag_COMDebug == 1)
		{		
				sprintf(printfstr,"Close socket %d is Error!\r\n",soc);			
				BLE_SendString(printfstr);
		}
		 ClearNBUSARTBUF();
		 return 0;
}
/****************************************************************************
*	�� �� ��:  BC95_SendDataByUDP
*	����˵��: ͨ��UDP��������
*	��    �Σ�soc�������ݵ�socket *ip������IP�������˿ںţ�IP�Ͷ˿ں�֮���ж��ŷָ�
*           len���͵����ݳ���  data��Ҫ���͵�����
*	�� �� ֵ: 1��ʾ���سɹ���0��ʾ����ʧ��
* ˵    �������ݷ��ͳɹ���ģ�鷵��һ��socket,���͵����ݳ��Ⱥ�OK�ַ�
*****************************************************************************/
char BC95_SendDataByUDP(uint8_t soc,char *ip,uint8_t len,uint8_t *data)
{
	   char printfstr[50];
	  char senddatastr[200];
	  unsigned char datastring[150];
	
	  HexToBCDstring(datastring,data,len);
	  sprintf(senddatastr,"AT+NSOST=%d,%s,%d,%s",soc,ip,len,datastring);
	  BC95_SEND_STRING(senddatastr);
		delaytime = 0;
		ClearNBUSARTBUF();
		nrf_delay_ms(50);
	  while(delaytime<20)
		{
			 if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
			 {
				   if(Flag_COMDebug == 1)
						{		
//						  BLE_SendString(senddatastr);
							 sprintf(printfstr,"UDP data send OK:%s\r\n",ip);							
							 BLE_SendString(printfstr);
						}
				  	 ClearNBUSARTBUF();
	           return 1;
			 }
			 if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
			 {
				   ClearNBUSARTBUF();
				   BC95_SEND_STRING(senddatastr);
				   nrf_delay_ms(50);
			 }
		}
	  ClearNBUSARTBUF();
	  return 0;
}
/****************************************************************************
*	�� �� ��:  BC95_ReadUDPData
*	����˵��:  ��ȡģ���յ�������
*	��    �Σ�soc�������ݵ�socket *ip������IP�������˿ںţ�IP�Ͷ˿ں�֮���ж��ŷָ�
*	�� �� ֵ: 1��ʾ���سɹ���0��ʾ����ʧ��
* ˵    ����ģ����յ����������ݺ󷵻�+NSONMI������ʹ��AT+NSORF��ȡ�յ������ݣ����
*           ģ���յ�������Ϊ��ȡ�����´η��������͵�����ģ�鲻����ʾ
*****************************************************************************/
char BC95_ReadUDPData(uint8_t soc,uint8_t len)
{
	  char printfstr[50];
	  char readorder[20];
	
	  sprintf(readorder,"AT+NSORF=%d,%d",soc,len);
	  BC95_SEND_STRING(readorder);
		delaytime = 0;
		ClearNBUSARTBUF();
		nrf_delay_ms(50);
	  while(delaytime<5)
		{
			  if(strstr(NBUART_RX_BUFFER,"OK")!=NULL)
				{
					 ExtractServerSenddata(len);
					 return 1;
				}	
        if(strstr(NBUART_RX_BUFFER,"ERROR")!=NULL)
				{

           return 0;
				}					
		}
	return 0;
}
/****************************************************************************
*	�� �� ��:  ExtractServerSenddata
*	����˵��:  ��ȡ���������͵�����
*	��    �Σ� len���������͵����ݳ���
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
char  ExtractServerSenddata(uint8_t len)
{
	 char *pstr=NULL;
	 char *pstr1=NULL;
	 uint8_t severdatastring[130];
	
	 pstr = strstr(NBUART_RX_BUFFER,"2405");
	 pstr1 = strstr(NBUART_RX_BUFFER,"6942");
	 if((pstr !=NULL)&&(pstr1 != NULL))
	 {
	    memcpy(severdatastring,pstr,2*len);	
      StringToHex(severdatastring,2*len,NB_ReceiveServerData);
      Flag_ReceivedServerOrder = 1;
      ClearNBUSARTBUF();		 
	 }
   
 return 1;	
}
/****************************************************************************
*	�� �� ��: HexToBCDstring
*	����˵��: ��16��������ת��ΪBCD���ʽ���ַ���
*	��    �Σ�*bcdstrת�����ַ���λ�ã�*hexdata ת��ǰ16�������飬len��Ҫת�������ݳ���
*	�� �� ֵ: 1��ʾ���سɹ���0��ʾ����ʧ��
* ˵    �������ݷ��ͳɹ���ģ�鷵��һ�����ݳ��Ⱥ�OK�ַ�
*****************************************************************************/
void  HexToBCDstring(unsigned char *bcdstr,unsigned char *hexdata,uint8_t len)
{
	 uint8_t i = 0;
	 uint8_t dat = 0;
	
	 for(i=0;i<len;i++)
	 {
		  dat = (hexdata[i]&0xF0)>>4;
		  if((dat>=0)&&(dat<=9))
			{
		     bcdstr[2*i]= dat+0x30;
			}
			if((dat>=0x0A)&&(dat<=0x0F))
			{
		     bcdstr[2*i]= dat-0x0A+0x41;
			}
			dat = (hexdata[i]&0x0F);
		  if((dat>=0)&&(dat<=9))
			{
		     bcdstr[2*i+1]= dat+0x30;
			}
			if((dat>=0x0A)&&(dat<=0x0F))
			{
		     bcdstr[2*i+1]= dat-0x0A+0x41;
			}
	 }
	 bcdstr[2*len] = '\0';	
}
