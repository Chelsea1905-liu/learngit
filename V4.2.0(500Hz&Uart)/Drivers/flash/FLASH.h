/**************************************************************************//**
 * @file     FLASH.c
 * @brief    FALSH ´æ´¢
 * @version  V1.0
 * @date     2017.09
 * @note
 * Copyright (C) 2017 ÕÅì¿
 *
 * @par      NRF52822 flashÇý¶¯³ÌÐò
 *   
 ******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "boards.h"
#include "nrf_drv_gpiote.h"

#define  sFLASH_ID_Q16             0xEF3015   //W25X16
#define  sFLASH_ID_Q32             0xEF4015	 //W25Q32
#define  sFLASH_ID_Q64             0xEF4017	 //W25Q64
#define  sFLASH_ID_Q128            0xEF4018	 //W25Q128

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

#define SPI_FLASH_CS_LOW()       nrf_gpio_pin_clear(FLASH_CS_PIN)
#define SPI_FLASH_CS_HIGH()      nrf_gpio_pin_set(FLASH_CS_PIN)
#define SPI_FLASH_SCK_HIGH()      nrf_gpio_pin_set(FLASH_SPI_SCK_PIN)
#define SPI_FLASH_SCK_LOW()      nrf_gpio_pin_clear(FLASH_SPI_SCK_PIN)
#define SPI_FLASH_MOSI_HIGH()      nrf_gpio_pin_set(FLASH_SPI_MOSI_PIN)
#define SPI_FLASH_MOSI_LOW()      nrf_gpio_pin_clear(FLASH_SPI_MOSI_PIN)
#define SPI_FLASH_MISO_HIGH()      nrf_gpio_pin_clear(FLASH_SPI_MISO_PIN)
#define SPI_FLASH_MISO_LOW()      nrf_gpio_pin_clear(FLASH_SPI_MISO_PIN)

void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_I2S_SendData(uint8_t data);
uint8_t SPI_I2S_ReceiveData(void);



#endif /* __SPI_FLASH_H */

