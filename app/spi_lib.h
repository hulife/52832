#ifndef _EPD_5IN83B_V2_h_
#define _EPD_5IN83B_V2_h_
#include <stdint.h>


#define  SPI_DC     5   //OLED命令/数据选择 H=命令 L=数据
#define  SPI_RST    2    //OLED复位

//#define pingmu_RST_LOW              nrf_gpio_pin_write(SPI_RST, 0); 
//#define pingmu_RST_HIGH             nrf_gpio_pin_write(SPI_RST, 1); 

//#define SPI_DC_SET    nrf_gpio_pin_set(SPI_DC)
//#define SPI_DC_CLEAR  nrf_gpio_pin_clear(SPI_DC)

//#define SPI_RST_SET    nrf_gpio_pin_set(SPI_RST)
//#define SPI_RST_CLEAR  nrf_gpio_pin_clear(SPI_RST)



#define EPD_5IN83B_V2_WIDTH       648
#define EPD_5IN83B_V2_HEIGHT      480

uint8_t EPD_5IN83B_V2_Init(void);
void EPD_5IN83B_V2_Clear(void);
void EPD_5IN83B_V2_Display(const uint8_t *blackimage, const uint8_t *ryimage);
void EPD_5IN83B_V2_Sleep(void);
void EPD_5IN83B_V2_DisplayPicture(const uint8_t *blackimage,uint8_t Block);
void EPD_5IN83B_V2_Clear_C(uint8_t color);	
#endif
