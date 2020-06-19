

#ifndef  __RING_BUFFER_H 
#define __RING_BUFFER_H 
#include "stdint.h"
#define     RINGBUFF_LEN    (1000*4)  //lqs change 20200519 原本没有括号

typedef enum buffer_type
{
    type_float,
    type_int,
    type_char
}RingBuff_type_t;

typedef struct
{
    uint16_t    Head; 
    uint16_t    Tail;
    uint16_t    Lenght;
    uint8_t     Ring_Buff[2][RINGBUFF_LEN];
    RingBuff_type_t type;
    
}RingBuff_t;
extern  RingBuff_t ringBuff_ori;//创建一个ringBuff的缓冲区
//extern  RingBuff_t ringBuff_heart_point;//创建一个ringBuff的缓冲区
//extern  RingBuff_t ringBuff_resp_point;//创建一个ringBuff的缓冲区

int RingBuff_Init(RingBuff_t* st_ringbuf ,  RingBuff_type_t btype );
uint8_t Write_RingBuff(RingBuff_t* st_ringbuf ,void* data1, void* data2);
uint8_t Read_RingBuff(RingBuff_t* st_ringbuf ,void* data1, void* data2);
uint16_t RingBuff_len(RingBuff_t* st_ringbuf);
#endif

