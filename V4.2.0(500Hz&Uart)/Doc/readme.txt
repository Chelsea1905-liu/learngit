
��������ָ��
------------------------------------------------------
24 02 02 01 ff 69 42    //ͬ��ָ��
24 02 03 25 01 FF 69 42 //��ȡ����汾
24 02 03 23 01 FF 69 42 //��ȡMAC
24 02 03 20 01 ff 69 42  //����ָ��
24 02 03 10 01 FF 69 42 //ʵʱ����
24 02 03 D1 01 FF 69 42 //ɾ���������
24 02 03 D1 02 FF 69 42 //ɾ��IP
24 02 03 30 01 FF 69 42 //��ѯ�豸����״̬
24 02 03 27 01 FF 69 42 //��ѯ�豸״̬-����ʱ��
24 05 0F 22 4E 42 31 37 30 39 30 34 30 30 30 30 31 ff 69 42    //����MACΪNB1709040001

         �Ϻ����wifiָ��
=======================================
AT+FMVER                  //��ѯ�̼��汾
ST+SAVE                   //���ò�������
AT+FACTORY                //�ָ��û���������
AT+FCLR                   //����û��������ָ���������
AT+REBOOT                 //����
AT+EVENT=ON               //��ѯ  ���ÿ����¼���Ϣ
AT+ECHO=OFF               //��ѯ  ���÷��������Ƿ����
AT+PMSLP=ON                //��ѯ  �����Ƿ���MCU�͹���ģʽ,MCU����͹���֮��ָ������Ӧ
AT+PRSLP=ON                  //��ѯ  �����Ƿ���RF�͹���ģʽ

AT+WFVER                  //��ѯ��Ƶ�����汾
AT+WMAC                   //��ѯģ��MAC��ַ
AT+WSCAN                  //ɨ��ģ����ΧAP
AT+WLANF=STA,ON           //����sta
AT+WMODE=STA               //��ѯ ����wifi����ģʽ ����AT+SAVE��������
AT+WSTA=iBreezee-DEV,dev201705  //����STAģʽap���ƺ�����   ����AT+SAVE��������
AT+WPRESTA=1                //��ѯ����STAģʽԤ���õ�AP���ƺ����� 
AT+WSTATUS                //��ѯSTA����״̬
AT+WLINK                  //��ѯSTA�����ź�ǿ�ȣ�����״̬
AT+DHCP=ON                 //����DHCP  ����ST+SAVE��������
AT+CON1=CLIENT,2000,25000,192.168.1.109            //����TCP ����1����   ����AT+SAVE��������
AT+CONF=1,ON               //�򿪻��߹ر�TCP���ܣ��ڲ��ϵ������ʵ��TCP���ع���
AT+CONS                   //��ѯtcp����״̬
AT+CONSOCKET=1              //��ѯsocket fd
AT+SSEND=1,8                  //����ģʽ�·������� �ַ���+16��������
2402031001ff6942
AT+SSSEND=2,16,2402031001ff6942    //����ģʽ�·������� �ַ���

AT+FMODE                  //���ý�������ģʽ���Ƿ���wlan tcp
AT+FEVENT                 //���ÿ����¼��ص�����
AT+FAT                    //��ѯ�Ƿ����AT����



//----------------����----------------------------
+++
a
AT+WMODE=STA
AT+EVENT=ON
AT+PMSLP=ON  //

AT+WLANF     //��ѯSTA�Ƿ����������Ӧ+OK=AP,OFF,STA,OFF����  AT+WLANF=STA,ON
AT+WSTATUS      //��ѯ����״̬  +OK=UAP_DOWN,STATION_UP

AT+WSTA=iBreezee-DEV,dev201705
AT+WSTA=zangwii2008,zw853429
AT+DHCP=ON
AT+SAVE
AT+REBOOT

wifi������֮��ʹ��AT+CONF=1��ѯTCP�Ƿ�򿪣����ΪOFF����ʹ��AT+CONF=1,ON��
AT+CON1=CLIENT,2000,6000,192.168.1.109
AT+CON1=CLIENT,2000,6008,117.78.48.143
AT+SAVE
AT+REBOOT

AT+CONS   //��ѯTCP�Ƿ����ӣ����������ѯsocket��Ȼ����socket��������
AT+CONSOCKET=1
AT+SSSEND=2,32,2402031001ff69422402031001ff6942

