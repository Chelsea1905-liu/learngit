/****************************************************************************
 * @file     BLE.C
 * @brief    �������ݴ���
 * @version  V1.0.0
 * @date     2020.3
 * @note
 * Copyright (C) 2020
 *
 * @par     NB-IOTģ���������ݴ���
 *  
*****************************************************************************/
#include "BLE_NRF.h"
#include "Parameter.h"
//extern char blename[12];

//====================================BLE��ز�������=====================================================
static ble_nus_t     ble_send;                                      /**< Structure to identify the Nordic UART Service. */
static uint16_t      m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current connection. */
static ble_uuid_t    m_adv_uuids[] = {{BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}};  /**< Universally unique service identifier. */
static	  pstorage_handle_t       base_flash_handle[PSTORAGE_PAGES];
pstorage_handle_t BTname_block_handle;
pstorage_handle_t MAC_block_handle;

/****************************************************************************
*	�� �� ��: APP_Receive_Order_Dispose 
*	����˵��: APPָ�����
*	��    �Σ���
*	�� �� ֵ: ����ָ��ִ�гɹ�����ʧ�� 
*****************************************************************************//*
void  APP_Receive_Order_Dispose(uint8_t *dat)
{ 
	 uint32_t      err_code;
	 char send_data[20];
	 uint8_t i = 0;
	 uint8_t len = 0;
	 uint8_t order_len = 0;	
	 order_len = dat[2]+5;
	 char printfstr[50];
	
	 switch( dat[3])
	 {
		 case CMD_MOBSYNC:              //ͬ��ָ��                         		  
		      if(dat[4] == 0x01)
					{
//						  SaveRouterNameToFlash();
//						  send_data[0] = SET_ROUTER_NAME;
//		          send_data[1] = MODULESTATUS_INIT;			 
//              err_code = APP_Send_Order(send_data,2);
//						
//						  WIFI_Set_STASSID("zangwii2008","zw853429");
//             	Flag_init = MAC_INIT_OK;  //���Խ�������ģ���ʼ��
//		          Flag_LED_Status = LED_WIRELEE_INIT;
//		          Flag_WIFIInitStep = WIFI_INIT_STEP1;
//						  WiFi_InitTime = 0;
//						  Flag_Reset_RouterSSID=1;
					}
		 break;

		 case CMD_SETBTNAME :              //�޸��������� 
			BlueTooth_Name_Len = dat[2]-2;	  //�豸��ų���
			//			   len=strlen(MANUFACTURER_NAME);
			//			   for(i=0;i<len;i++)
			//	         BlueTooth_Name[i] = MANUFACTURER_NAME[i];  //ȡ������������iBreezee��ͷ��������趨
			//			   BlueTooth_Name[len] = '_';		  
			//				 for(i=0; i<BlueTooth_Name_Len;i++)
			//				 {
			//					 BlueTooth_Name[len+1+i] = dat[4+i];
			//				 }	      
			//	       BlueTooth_Name_Len += len+1;
			for(i=0; i<BlueTooth_Name_Len;i++)
			{
				BlueTooth_Name[i] = dat[4+i];
			}	      
			SaveBTNameToFlash();
			send_data[0] = CMD_SETBTNAME;
			send_data[1] = RES_COMMANDOK;			 
			err_code = APP_Send_Order(send_data,2);
		 break;
				 
		
						
		case CMD_READVER:               //��ȡ�����汾
				  len = strlen(SoftVer);
			    send_data[0] = CMD_READVER;
			    for(i=0;i<len;i++)
					{
						 send_data[i+1] = SoftVer[i];
					}
				 err_code = APP_Send_Order(send_data,len+1);
		break;	 
		 
		case CMD_DEBUG:     //�������
			if(Flag_Debug == 0)
			{
				Flag_Debug = DEBUG_BLE;
			}
			else
			{
				Flag_Debug = 0;
			}		     
		break;
			
			
		case CMD_DELFLASHDATA:          //ɾ��flash����
			if(dat[4] == CMD_DELSAVENUM )
			{


			}
			if(dat[4] == CMD_DELSERVERIP )
				{
				Flag_PowerOn = 1;
				SPI_FLASH_SectorErase(FLASH_ServerInfoAddress);					 
			}
		break;
			
		
			
		 default:
       break;		 
	 }	

}*/
/****************************************************************************
*	�� �� ��: APP_Send_Order  
*	����˵��: �ظ�APPָ����Ϣ
*	��    �Σ�*pdata���͵�ָ��+�������ݣ�������У��λ
*	�� �� ֵ: ����ָ��ִ�гɹ�����ʧ�� 
*****************************************************************************//*
uint32_t APP_Send_Order(uint8_t *pdata,uint8_t len)
{
 	  uint32_t      err_code;
	  uint8_t i = 0;
	  uint8_t jiaoyan = 0;
	  uint8_t APP_Order_Send_Data[100];     //���ڱ���app���͵�ָ�
		

		APP_Order_Send_Data[0] = CMD_START  ;
		APP_Order_Send_Data[1] = TypeID_WIFI;
		APP_Order_Send_Data[2] = len+1;
		for(i = 0; i < len ;i++ )
		{
				APP_Order_Send_Data[i+3] = pdata[i];
		}
		for(i = 0; i < len ;i++ )
		{
				jiaoyan += pdata[i];
		}
		APP_Order_Send_Data[3+len] = jiaoyan; 
		APP_Order_Send_Data[4+len] = CMD_LF1; 
		APP_Order_Send_Data[5+len] = CMD_LF2; 
		//BLE_SendBufData(APP_Order_Send_Data,len+6);	 
		return err_code;
}*/
/****************************************************************************
*	�� �� ��: app_receivedata_handler   
*	����˵��: ��������������ݴ���
*	��    �Σ�p_nus    Nordic UART Service structure.
*           length   Length of the data.
*           p_data   Data to be send to UART module.
*	�� �� ֵ: ��
* @detailsThis function will process the data received from the Nordic UART BLE Service and send
*          it to the UART module.
*****************************************************************************/
static void app_receivedata_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
	uint8_t i = 0;

	for ( i = 0; i < length; i++)
	{
		APP_Order_Receive_Data[APP_Order_Receive_len+i] = p_data[i];
	}	
	APP_Order_Receive_len += length;
//	APP_Receive_Right = 1;	
}

/****************************************************************************
*	�� �� ��: ble_stack_init   
*	����˵��: BLEЭ��ջ��ʼ�� BLE�жϳ�ʼ�� ��ʼ��Э��ջ���ڴ���䣬�趨�ص�����
*	��    �Σ���
*	�� �� ֵ: ��
* @details  This function initializes the SoftDevice and the BLE event interrupt.        
*****************************************************************************/ 
static void ble_stack_init(void)
{
    uint32_t err_code;
    
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;  
    
 // Initialize SoftDevice.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);
    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);

        
//Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    
 // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);  //ϵͳ�¼�����ģ��ע��
}
/****************************************************************************
*	�� �� ��: ble_evt_dispatch   
*	����˵��: BLE�¼��жϵ���
*	��    �Σ�p_ble_evt  SoftDevice event.
*	�� �� ֵ: ��
* @details  This function is called from the SoftDevice event interrupt handler after a 
 *          SoftDevice event has been received.
*****************************************************************************/ 
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_nus_on_ble_evt(&ble_send, p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
    bsp_btn_ble_on_ble_evt(p_ble_evt);
    
}
/****************************************************************************
*	�� �� ��: on_ble_evt   
*	����˵��: BLE�¼��ж��¼�
*	��    �Σ�p_ble_evt SoftDevice event.
*	�� �� ֵ: ��
* @details  Function for the application's SoftDevice event handler.
*****************************************************************************/ 
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:       //��������
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
//				    Flag_BT_Linkstatus = BT_CONNECTED;	 
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:   //�����Ͽ�
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
//				    Flag_BT_Linkstatus = BT_NoCONNECT;
//				    Flag_SendSleepDataToAPP = DISABLESENDDATAAPP;  //�������ӶϿ�����ֹ����˯������
         //   Flag_Debug = 0;

            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);

            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);

            break;

        default:
            // No implementation needed.
            break;
    }
}
/****************************************************************************
*	�� �� ��: gap_params_init   
*	����˵��: ble�ӿڲ�����ʼ��  �趨���Ӳ���
*	��    �Σ���
*	�� �� ֵ: ��
* @details  this function will set up all the necessary GAP (Generic Access Profile) parameters of 
 *          the device. It also sets the permissions and appearance.
*****************************************************************************/ 
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    /*
    if(ReadBTNameFromFlash()==0)	  //��ȡ��������
    {
         if(Flag_Debug == DEBUG_BLE)
        {
            BLE_SendString(BlueTooth_Name);	 
        }	
        err_code = sd_ble_gap_device_name_set(&sec_mode,
                                      (const uint8_t *) DEVICE_NAME,
                                      strlen(DEVICE_NAME));
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        err_code = sd_ble_gap_device_name_set(&sec_mode,
                                      (const uint8_t *) BlueTooth_Name,
                                      BlueTooth_Name_Len);
         APP_ERROR_CHECK(err_code);
     }
    */
    err_code = sd_ble_gap_device_name_set(&sec_mode,\
                      (const uint8_t *) DEVICE_NAME,\
                      strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
//    APP_ERROR_CHECK(err_code);
}
/****************************************************************************
*	�� �� ��: services_init   
*	����˵��: Ӧ�÷����ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
static void services_init(void)
{
    uint32_t       err_code;
    ble_nus_init_t nus_init;
    ble_dis_init_t dis_init;

	
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = app_receivedata_handler;
    
    err_code = ble_nus_init(&ble_send, &nus_init);
//    APP_ERROR_CHECK(err_code);
	
	  // Initialize Device Information Service.
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)MANUFACTURER_NAME);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err_code = ble_dis_init(&dis_init);
//    APP_ERROR_CHECK(err_code);
	
}

/****************************************************************************
*	�� �� ��: advertising_init   
*	����˵��: �㲥��ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
static void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;

//    // 0x0059��Nordic��������ID
//    manuf_specific_data.company_identifier = 0x24;
//    // ָ���Զ�������
//    manuf_specific_data.data.p_data = count_datai;
//    manuf_specific_data.data.size   = len;
    
    //Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;//һ������ģʽ
//    advdata.p_manuf_specific_data = &manuf_specific_data;


    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);    
    APP_ERROR_CHECK(err_code);
}
/****************************************************************************
*	�� �� ��: on_adv_evt  
*	����˵��: �㲥�ж��¼�
*	��    �Σ�ble_adv_evt  Advertising event.
*	�� �� ֵ: ��
* @details This function will be called for advertising events which are passed to the application.
*****************************************************************************/ 
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
//        case BLE_ADV_EVT_IDLE:    //�������ӳ�ʱ�����³�ʼ��                                 
//            //err_code = ble_advertising_start(BLE_ADV_MODE_FAST);  //BLE��������
//            //sleep_mode_enter();
//            break;
        default:
            break;
    }
}
/****************************************************************************
*	�� �� ��: sleep_mode_enter   
*	����˵��: ����ģʽ
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void sleep_mode_enter(void)
{   
    //����5��Ϊ�ߵ�ƽ���Ѵ���
    nrf_gpio_cfg_sense_input(SLEEP_MODE_PIN,NRF_GPIO_PIN_NOPULL,GPIO_PIN_CNF_SENSE_High);
    
    
    __set_FPSCR(__get_FPSCR() & ~(FPU_EXCEPTION_MASK));
    (void) __get_FPSCR();
    NVIC_ClearPendingIRQ(FPU_IRQn);

    IO3_OFF;
    IO4_OFF;
//    nrf_gpio_cfg_default(IO4_PIN);
//	  nrf_gpio_cfg_default(IO3_PIN);
    nrf_drv_saadc_channel_uninit(0);
    nrf_drv_saadc_channel_uninit(1);
    nrf_drv_saadc_channel_uninit(2);
    nrf_drv_saadc_channel_uninit(3);
    nrf_drv_saadc_uninit();
//    app_uart_close();
//    nrf_gpio_cfg_default(FLASH_CS_PIN);
//	  nrf_gpio_cfg_default(FLASH_SPI_SCK_PIN);   
//    nrf_gpio_cfg_default(FLASH_SPI_MOSI_PIN);
//	  nrf_gpio_cfg_default(FLASH_SPI_MISO_PIN);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    uint32_t err_code = sd_power_system_off();
    //APP_ERROR_CHECK(err_code);
}
/****************************************************************************
*	�� �� ��: conn_params_init   
*	����˵��: BLE���Ӳ�����ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    
    memset(&cp_init, 0, sizeof(cp_init));
    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = true;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;
    
    err_code = ble_conn_params_init(&cp_init);
//    APP_ERROR_CHECK(err_code);
}
/****************************************************************************
*	�� �� ��: on_conn_params_evt  
*	����˵��: BLE�����ж��¼�
*	��    �Σ� p_evt  Event received from the Connection Parameters Module.
*	�� �� ֵ: ��
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
*****************************************************************************/
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;
    
    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
//        APP_ERROR_CHECK(err_code);
    }
}
/****************************************************************************
*	�� �� ��: conn_params_error_handler
*	����˵��: BLE���Ӵ�����
*	��    �Σ�nrf_error  Error code containing information about what went wrong.
*	�� �� ֵ: ��
*****************************************************************************/ 
static void conn_params_error_handler(uint32_t nrf_error)
{
//    APP_ERROR_HANDLER(nrf_error);
}
/****************************************************************************
*	�� �� ��: bsp_event_handler
*	����˵��: �豸�ж��¼�����
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;
    switch (event)
    {
//        case BSP_EVENT_SLEEP:
////            sleep_mode_enter();
//            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
//                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            err_code = ble_advertising_restart_without_whitelist();
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
//                APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            break;
    }
}


/****************************************************************************
*	�� �� ��: sys_evt_dispatch
*	����˵��:  Function for dispatching a system event to interested modules 
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
static void sys_evt_dispatch(uint32_t sys_evt)
{
	  pstorage_sys_event_handler(sys_evt);   //pstorageϵͳ�¼�����
}
/****************************************************************************
*	�� �� ��: flash_pstorage_cb_handler
*	����˵��: Event Notification Handler.  
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
static void flash_pstorage_cb_handler(pstorage_handle_t  * handle,
                               uint8_t              op_code,
                               uint32_t             result,
                               uint8_t            * p_data,
                               uint32_t             data_len)
{
    switch(op_code)
    {
		case PSTORAGE_LOAD_OP_CODE:
           if (result == NRF_SUCCESS)
           {
               // Store operation successful.
           }
           else
           {
               // Store operation failed.
           }
           // Source memory can now be reused or freed.
           break;
		case PSTORAGE_CLEAR_OP_CODE:
           if (result == NRF_SUCCESS)
           {
               // Clear operation successful.
           }
           else
           {
               // Clear operation failed.
           }
           break;
    }
}
/****************************************************************************
*	�� �� ��: register_flash
*	����˵��: ע��洢module.  
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void register_flash(void)
{
	 uint32_t err_code;
	 uint32_t i = 0;
	 pstorage_module_param_t param;
	 
	 param.block_size  = M_BLOCK_SIZE;
	 param.block_count = M_BLOCK_COUNT;
	 param.cb          = flash_pstorage_cb_handler;
 
	 err_code= pstorage_init();

	
	 	for(i = 0; i < PSTORAGE_PAGES; i++)
	{
			err_code = pstorage_register(&param,base_flash_handle);

	}	
}

/****************************************************************************
*	�� �� ��: BLE_Start
*	����˵��: ��������  
*	��    �Σ���
*	�� �� ֵ: ����0 NRF_SUCCESS��ʾ�����ɹ�����������ֵ��ʾʧ��
*****************************************************************************/
uint8_t BLE_Start(void)	
{
	uint32_t err_code;
//---------------------------------BLE��ʼ��------------------------------------------	
	ble_stack_init();  				//Э��ջ��ʼ��
	register_flash();	
    gap_params_init(); 				//�������Ӳ�����ʼ��(�������Ƶȣ�
    services_init();  				//
    advertising_init();				//�㲥��ʼ��
    conn_params_init();				//
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);  //�㲥����
    Flag_PowerOn = 0;
    return(err_code&0xFF);
}
/****************************************************************************
*	�� �� ��: BLE_SendString
*	����˵��: ʹ�����������ַ���  
*	��    �Σ�*str ���͵��ַ���
*	�� �� ֵ: 
* ˵    ����BLE һ��ֻ�ܷ���20�ֽڵ��ַ���,����ַ������ȴ���20�ֽڣ�����Ҫ���зְ�����
*****************************************************************************//*
void  BLE_SendString(uint8_t *str)
{
	uint8_t len,i;
	uint8_t len_int = 0;
	uint8_t len_rem = 0;
	
	len = strlen(str);
	if(len == 0)
	{
		return ;
	}
	else if(len < 21)     
	{
		 ble_nus_string_send(&ble_send,str, len);
	}
	else
	{
		 len_int=len/20;
		 len_rem=len%20;
		 for(i=0;i<len_int;i++)
		 { 
			  ble_nus_string_send(&ble_send,&str[20*i], 20);
			  nrf_delay_ms(20);
		 }
		 ble_nus_string_send(&ble_send,&str[20*len_int], len_rem);
	}	
	nrf_delay_ms(20);
}*/
/****************************************************************************
*	�� �� ��: BLE_SendBufData
*	����˵��: ʹ���������������е�����  
*	��    �Σ�*buf����  len���͵����鳤��
*	�� �� ֵ: 
* ˵    ����BLE һ��ֻ�ܷ���20�ֽڵ��ַ���,������ȴ���20�ֽڣ�����Ҫ���зְ�����
*****************************************************************************//*
void  BLE_SendBufData(char *buf,uint8_t len)
{
	 uint8_t i;
	uint8_t len_int = 0;
	uint8_t len_rem = 0;
	
  if(len == 0)
	{
    return;
	}		
	else if(len < 21)     
	{
		 ble_nus_string_send(&ble_send,buf, len);
	}
	else
	{
		 len_int=len/20;
		 len_rem=len%20;
		 for(i=0;i<len_int;i++)
		 { 
			  ble_nus_string_send(&ble_send,&buf[20*i], 20);
			  nrf_delay_ms(20);
		 }
		 ble_nus_string_send(&ble_send,&buf[20*len_int], len_rem);
	}
	nrf_delay_ms(20);
}*/

/****************************************************************************
*	�� �� ��: advertising_status_data   
*	����˵��: �㲥״ֵ̬
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
void advertising_status_data(uint8_t *count_datai,uint8_t len)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    // ����һ���������Զ������ݵĽṹ����������ù㲥����ʱ���ñ����ĵ�ַ��ֵ���㲥���ݰ���
    ble_advdata_manuf_data_t manuf_specific_data;
    

    
    // 0x0059��Nordic��������ID
    manuf_specific_data.company_identifier = 0x24;
    // ָ���Զ�������
    manuf_specific_data.data.p_data = count_datai;
    manuf_specific_data.data.size   = len;
    
    
    //Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;//һ������ģʽ
    advdata.p_manuf_specific_data = &manuf_specific_data;

#if 0
    uint8_t kk_buf[]="hello";
    
    ble_advdata_manuf_data_t manuf_data_response;
    ble_advdata_t advdata_response;
    
    manuf_data_response.company_identifier = 0x0024;
    manuf_data_response.data.p_data = kk_buf;
    manuf_data_response.data.size   = sizeof(kk_buf);
    
    
    memset(&advdata_response, 0, sizeof(advdata_response));
    advdata_response.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata_response.p_manuf_specific_data = &manuf_data_response;
    
   
    
    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;
     
    
    err_code = ble_advertising_init(&advdata, &advdata_response, &options, on_adv_evt, NULL);
#else
    err_code = ble_advertising_specific_data(&advdata, NULL,on_adv_evt, NULL);
#endif
    APP_ERROR_CHECK(err_code);
}	
