 /**
 * @brief  RingBuff_Init
 * @param  void
 * @return void
 * @author 
 * @date   2018
 * @version v1.0
 * @note   初始化环形缓冲区
 */
 #include "ring_buffer.h"
RingBuff_t ringBuff_ori;//创建一个ringBuff的缓冲区
//RingBuff_t ringBuff_heart_point;//创建一个ringBuff的缓冲区  lqs test
//RingBuff_t ringBuff_resp_point;//创建一个ringBuff的缓冲区

int  RingBuff_Init(RingBuff_t* st_ringbuf,  RingBuff_type_t btype)
{
    //初始化相关信息
    st_ringbuf->Head = 0;
    st_ringbuf->Tail = 0;
    st_ringbuf->Lenght = 0;
    st_ringbuf->type = btype;
    memset(st_ringbuf->Ring_Buff, 0, sizeof(st_ringbuf->Ring_Buff));
    return 0;
}
 /**
 * @brief  Write_RingBuff
 * @param  u8 data
 * @return FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
 * @author 杰杰
 * @date   2018
 * @version v1.0
 * @note   往环形缓冲区写入u8类型的数据
 */
uint8_t Write_RingBuff(RingBuff_t* st_ringbuf , void* data1, void* data2)
{
    uint32_t var_len = 0;
    
    if(st_ringbuf->Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
    {
        return 0;
    }
    switch(st_ringbuf->type)
    {
        //float
        case 0:
            var_len = 4;
            break;
        //int
        case 1:
            var_len = 4;
            break;
        //char
        case 2:
            var_len = 1;
            break;
        default:
            var_len = 4;
        break;
    }
    
    memcpy(&(st_ringbuf->Ring_Buff[0][st_ringbuf->Tail]), data1, var_len);
    memcpy(&(st_ringbuf->Ring_Buff[1][st_ringbuf->Tail]), data2, var_len);
    
    st_ringbuf->Tail = (st_ringbuf->Tail+var_len)%RINGBUFF_LEN;//防止越界非法访问
    st_ringbuf->Lenght += var_len;
    return 1;
}
 /**
 * @brief  Read_RingBuff
 * @param  u8 *rData，用于保存读取的数据
 * @return FLASE:环形缓冲区没有数据，读取失败;TRUE:读取成功
 * @author 杰杰
 * @date   2018
 * @version v1.0
 * @note   从环形缓冲区读取一个u8类型的数据
 */
uint8_t Read_RingBuff(RingBuff_t* st_ringbuf ,void* data1,void* data2)
{
    uint8_t var_len = 0;
    uint8_t tail_count = 0;
    if(st_ringbuf->Lenght == 0)//判断非空
    {
        return 0;
    }
    switch(st_ringbuf->type)
    {
        //float
        case 0:
            var_len = 4;
            break;
        //int
        case 1:
            var_len = 4;
            break;
        //char
        case 2:
            var_len = 1;
            break;
        default:
            var_len = 4;
        break;
    }
    
    memcpy(data1,&(st_ringbuf->Ring_Buff[0][st_ringbuf->Head]), var_len);
    memcpy(data2,&(st_ringbuf->Ring_Buff[1][st_ringbuf->Head]), var_len);
    st_ringbuf->Head = (st_ringbuf->Head+var_len)%RINGBUFF_LEN;//防止越界非法访问
    st_ringbuf->Lenght -= var_len;
    return 1;

}

uint16_t RingBuff_len(RingBuff_t* st_ringbuf)
{
    return  st_ringbuf->Lenght;
}
