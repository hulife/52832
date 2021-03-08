/*   J202                   J201


BSI					GND      BS2					GND  电源地
BUSY1				03			 BUSY2				16	忙状态输出引脚（表示忙碌）	
RST1				02       RST2         11	复位，低电平有效
DC1					05       DC2          12	数据/命令，低电平表示命令，高电平表示数据
CS1					07       CS2          15	片选，低电平有效
SCK1				06       SCK2         14	SPI的CLK，时钟信号输入
DIN1				04       DIN2         13	SPI的MOSI，数据输入
nrf_delay_us(200);
*/
/* Includes ------------------------------------------------------------------*/
#ifndef __SPI_BLE_H
#define __SPI_BLE_H
#include <stdint.h>
#include <stdbool.h>

#define BOE0583_DISPLAY_ACTION_INIT       0x49  // 'I'
#define BOE0583_DISPLAY_ACTION_LOAD       0x4C  // 'L'
#define BOE0583_DISPLAY_ACTION_NEXT       0x4E  // 'N'
#define BOE0583_DISPLAY_ACTION_SHOW       0x53  // 'S'
#define BOE0583_DISPLAY_ACTION_INIT_RAW   0x69  // 'i'
#define BOE0583_DISPLAY_ACTION_LOAD_RAW   0x6C  // 'l'
#define BOE0583_DISPLAY_ACTION_NEXT_RAW   0x6E  // 'n'
#define BOE0583_DISPLAY_ACTION_SHOW_RAW   0x73  // 's'
#define BOE0583_DISPLAY_ACTION_REPLY      0x52  // 'R'
#define BOE0583_DISPLAY_ACTION_NONE       0x00

#define BOE0583_DEFAULT_DISPLAY_TIMEOUT   2000000 // ((unsigned long)sec * 1000000L) / Clock_tickPeriod
void BOE0583CC26XX_doDisplay(uint8_t action,  uint8_t* block, uint16_t len, uint32_t timeout);  
// Display resolution




extern const uint8_t gImage_7in5_V2_b[];

#define pingmu_RST_LOW              nrf_gpio_pin_write(pingmu_RST_IO, 0); 
#define pingmu_RST_HIGH             nrf_gpio_pin_write(pingmu_RST_IO, 1); 

#define pingmu_RST_OFF							1
#define pingmu_RST_ON	              0

#define BOARD_SPI0_MISO_IO          17
#define BOARD_SPI0_MOSI_IO          30//4
#define BOARD_SPI0_CLK_IO           31//6          
#define BOARD_SPI0_CSN_IO          25//7
#define pingmu_RST_IO               2
//#define BOARD_BS1_IO                 //未知
#define pingmu_BUSY_IO							3
#define BOARD_DC_IO                 28//5



#define SPI_SDA_SET   nrf_gpio_pin_set(BOARD_SPI0_MOSI_IO)
#define SPI_SDA_CLEAR nrf_gpio_pin_clear(BOARD_SPI0_MOSI_IO)

#define SPI_DC_SET    nrf_gpio_pin_set(BOARD_DC_IO)
#define SPI_DC_CLEAR  nrf_gpio_pin_clear(BOARD_DC_IO)

#define SPI_CLK_SET    nrf_gpio_pin_set(BOARD_SPI0_CLK_IO)
#define SPI_CLK_CLEAR  nrf_gpio_pin_clear(BOARD_SPI0_CLK_IO)

#define SPI_CS_SET    nrf_gpio_pin_set(BOARD_SPI0_CSN_IO)
#define SPI_CS_CLEAR  nrf_gpio_pin_clear(BOARD_SPI0_CSN_IO)

#define SPI_RST_SET    nrf_gpio_pin_set(pingmu_RST_IO)
#define SPI_RST_CLEAR  nrf_gpio_pin_clear(pingmu_RST_IO)

//RST引脚初始化
#define	 MAX_LINE_BYTES   81
#define  MAX_COLUMN_BYTES 480
#define  ALLSCREEN_BYTES  38880

#define  LINE_MNUM1  13
#define  LINE_MNUM2  60


#define YES 1
#define NO  0

#define MONO 1
#define RED  2


#define SPI_OUT 1
#define SPI_IN  0







void EpaperIO_Init(void);
void pingmuRstPinInit(void);
void pingmuRstPinSet(bool pinState);
void BUSY_Init(void);
void DC_Init(void);
void DC_L(void);
void DC_H(void);
void RESET(void);
void READBUSY(void);
void spi_clk(void);
void SPI_DATA_IO_Init(uint8_t mode);
void Epaper_Spi_WriteByte(uint8_t TxData);
uint8_t Epaper_Spi_ReadByte(void);
void Epaper_READBUSY(void);

//void Epaper_Write_Command(uint8_t cmd);
//void Epaper_Write_CommandR(uint8_t cmd);
//void Epaper_Write_Data(uint8_t data);


 void BOE0583CC26XX_writeBlock(const uint8_t* block, uint16_t len);
void BOE0583CC26XX_epdInit(void) ;
 void BOE0583CC26XX_epdUninit(void) ;
void BOE0583CC26XX_display(uint8_t action,  uint8_t* block, uint16_t len);



void Epaper_Update(void);
void Epaper_DeepSleep(void);
void Epaper_otp(void);
void Epaper_Load_Image( const uint8_t *datas,uint32_t num,uint8_t color_mode);
void Display_All_White(void);
void Display_All_Red(void);
void Display_All_Black(void);
void Draw_String(uint32_t  x, uint32_t y, const unsigned char *chs);
void Display(void);
bool pingmu_Init(void);
void black_a(void);
void spi_t(void);
void EPD_7IN5B_V2_TurnOnDisplay(void);
void test_screen(void);
void test_screen1(void);
void Epaper_Write_Data(unsigned char INIT_DATA);
void Epaper_Write_Command(unsigned char INIT_COM);

#endif
