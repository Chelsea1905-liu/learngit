
蓝牙调试指令
------------------------------------------------------
24 02 02 01 ff 69 42    //同步指令
24 02 03 25 01 FF 69 42 //读取软件版本
24 02 03 23 01 FF 69 42 //读取MAC
24 02 03 20 01 ff 69 42  //调试指令
24 02 03 10 01 FF 69 42 //实时数据
24 02 03 D1 01 FF 69 42 //删除保存序号
24 02 03 D1 02 FF 69 42 //删除IP
24 02 03 30 01 FF 69 42 //查询设备联网状态
24 02 03 27 01 FF 69 42 //查询设备状态-运行时间
24 05 0F 22 4E 42 31 37 30 39 30 34 30 30 30 30 31 ff 69 42    //设置MAC为NB1709040001

         上海庆科wifi指令
=======================================
AT+FMVER                  //查询固件版本
ST+SAVE                   //配置参数保存
AT+FACTORY                //恢复用户出厂设置
AT+FCLR                   //清除用户参数，恢复出厂设置
AT+REBOOT                 //重启
AT+EVENT=ON               //查询  设置开启事件信息
AT+ECHO=OFF               //查询  设置发送命令是否回显
AT+PMSLP=ON                //查询  设置是否开启MCU低功耗模式,MCU进入低功耗之后，指令无响应
AT+PRSLP=ON                  //查询  设置是否开启RF低功耗模式

AT+WFVER                  //查询射频驱动版本
AT+WMAC                   //查询模块MAC地址
AT+WSCAN                  //扫描模块周围AP
AT+WLANF=STA,ON           //开启sta
AT+WMODE=STA               //查询 设置wifi工作模式 输入AT+SAVE保存配置
AT+WSTA=iBreezee-DEV,dev201705  //设置STA模式ap名称和密码   输入AT+SAVE保存配置
AT+WPRESTA=1                //查询设置STA模式预设置的AP名称和密码 
AT+WSTATUS                //查询STA连接状态
AT+WLINK                  //查询STA连接信号强度，连接状态
AT+DHCP=ON                 //开启DHCP  输入ST+SAVE保存配置
AT+CON1=CLIENT,2000,25000,192.168.1.109            //设置TCP 连接1参数   输入AT+SAVE保存配置
AT+CONF=1,ON               //打开或者关闭TCP功能，在不断电情况下实现TCP开关功能
AT+CONS                   //查询tcp连接状态
AT+CONSOCKET=1              //查询socket fd
AT+SSEND=1,8                  //命令模式下发送数据 字符串+16进制数据
2402031001ff6942
AT+SSSEND=2,16,2402031001ff6942    //命令模式下发送数据 字符串

AT+FMODE                  //设置进入命令模式后是否开启wlan tcp
AT+FEVENT                 //设置开启事件回调功能
AT+FAT                    //查询是否进入AT功能



//----------------流程----------------------------
+++
a
AT+WMODE=STA
AT+EVENT=ON
AT+PMSLP=ON  //

AT+WLANF     //查询STA是否开启，如果回应+OK=AP,OFF,STA,OFF则发送  AT+WLANF=STA,ON
AT+WSTATUS      //查询连接状态  +OK=UAP_DOWN,STATION_UP

AT+WSTA=iBreezee-DEV,dev201705
AT+WSTA=zangwii2008,zw853429
AT+DHCP=ON
AT+SAVE
AT+REBOOT

wifi已连接之后，使用AT+CONF=1查询TCP是否打开，如果为OFF，则使用AT+CONF=1,ON打开
AT+CON1=CLIENT,2000,6000,192.168.1.109
AT+CON1=CLIENT,2000,6008,117.78.48.143
AT+SAVE
AT+REBOOT

AT+CONS   //查询TCP是否连接，已连接则查询socket，然后按照socket发送数据
AT+CONSOCKET=1
AT+SSSEND=2,32,2402031001ff69422402031001ff6942

