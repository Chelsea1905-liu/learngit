



#ifndef __FILTERPEAK_H__
#define __FILTERPEAK_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ROWS(s) sizeof(s)/sizeof(s[0])
#define FS 	        500  //信号采样率
#define HEART_FS    500  //心率采样率
#define RESP_FS     25   //呼吸采样率
#define STATUS_FS 25     //有无人、体动、呼吸暂停等其他状态检测采样率

#define SIG_MAX_VAL 4300  //ADC采样信号最大值
#define VOL_VAL 3300      //设备参考电压3.3V
#define SIG_SCALE VOL_VAL/SIG_MAX_VAL   //ADC信号转换参考电压比值
#define BASE_LINE 1425    //设备参考电压基线指

#define RESAMPLE_RESP   (FS / RESP_FS)  // 呼吸降采样计数器
#define SM_INIT_LEN1 RESP_FS * 4
#define SM_INIT_LEN2 HEART_FS * 2

#define MS 	        1000
#define MIN 	    60
#define PEAK_D_LEN_INIT     50
#define MAX_HEART           300 // 最大心率阈值
#define PF_HEART_INIT_LEN   50
#define PF_RESP_INIT_LEN    10
#define RR_VALI_INIT        2

#define SKIN_NEEDLING_RANGE 10
/*** 主峰间小峰个数差值判断，差值越大，信号约不平稳，设置该变量，来改变平稳信号查找约束条件 */
#define STABLE_LEN          10

#define S_LEN 3
#define MULTIPLE1   		0.91
#define MULTIPLE2   		0.71
#define MULTIPLE3   		0.61
#define RR_MAX_PARAM 	1.2 //心率推演基准点最大系数
#define RR_MIN_PARAM 	0.8 //心率推演基准点最小系数
#define STD_INIT_LEN   5
#define STEADY_RADIO 0.1 //平稳度判断参数

extern int turn_m_range;  //体动突变幅度（默认：3000）
extern int turn_s_ma_range;  //体动饱和信号最大幅度（默认：3700）
extern int turn_s_mi_range;  //体动饱和信号最小幅度（默认：20）
extern int turn_l_range;  //1S最大最小信号低于该幅度时，不判断体动（默认：300）
extern char move_body;//体动标记
extern float respReduce;//呼吸波形縮小倍數（默认：0.3）

#define HPLEN               300 //波形特征值数组初始长度
extern float HwavPiont[2][HPLEN];//存储波形特征值
extern int HwavLen;   //存储波形特征值长度;
extern float RwavPiont[HPLEN] ;  //呼吸波形
extern int RwavLen;

extern float heartFS[HEART_FS];
extern int heartFSLen;


extern float respFS[RESP_FS];
extern int respFSLen;

#define HEARTRATE_INIT_LEN  60
extern int heartRates[4][HEARTRATE_INIT_LEN];
extern int hr_len;
extern int hearts[4][HEARTRATE_INIT_LEN];  //存储心率值
extern unsigned int hearts_len ;


#define RESPS_INIT_LEN   20
extern int RespRate[2][RESPS_INIT_LEN];  //存储呼吸值
extern int RespRate_len ;

extern int resps[2][RESPS_INIT_LEN];
extern unsigned short resps_len;

extern char pieValue; //压阻判断人是否在床
extern char resp_status; //1:中枢型呼吸暂停，2：阻塞型呼吸暂停
extern char ahi_type_zsu;//呼吸暂停中枢性标记
extern char ahi_type_zse;//呼吸暂停阻塞性标记
extern char gPeopleFlag;//0：脱离监护;1：监护中
extern int cc;
//filter
//高通滤波器
float highPassFilterBcg(float signal);
//低通滤波器
float lowPassFilterBcg(float signal);

//心率平滑滤波器
float smoothFiltHeart(float gDatas);
//呼吸平滑处理器
float smoothRespSignal(float signal);
void arithmeticHearts(float signal);
void arithmeticResps(float signal);
void arithmeticAhi(float respSignal);

void leftShiftShort1DArray(short *d, int len, int num);
void leftShiftUnsigedShort1DArray(unsigned short *d, int len, int num);
void leftShiftInt1DArray(int *d, int len, int num);
void leftShiftInt2DArray(int *d, int row,int len, int num) ;
void leftShift1DArray(float *d, int len, int num);
void leftShift2DArray(float *d, int row,int len, int num);
void leftShiftShort2DArray(short *d, int row,int len, int num);

unsigned short getHeartRR(int pointRR) ;
unsigned short getHeartRate(float rr);
unsigned short getRespRate(int point) ;
unsigned short getRespRR(int point);
void OSAHS(float signalr);
void CSAS(float resp_data,float bcg_data);
void turnOverTimes(float hSig,float rSig);
void judgePeople(float hrSignal,float respSignal);
//void judgePeople(float hrSignal);
void arithmeticHeartInit();
int getMedValue(int s);
float smoothAHISignal(float signal);
float getRadioPara(int rr);
float getSteadyRadioPara(int rr);
unsigned int avgRespFilter(float x);
float avgRespSignalB(float signal);
float avgRespSignalC(float signal);
float getRespBaseLine(float signal);
float getHeatBaseLine(float signal);

#ifdef __cplusplus
}
#endif

#endif
