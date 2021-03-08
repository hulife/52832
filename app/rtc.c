#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#include <nrfx_rtc.h>
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"

#include "rtc.h"
/* 试验需要用到IK-52832DK开发板中的指示灯D1，占用的nRF52832引脚资源如下
P0.17：输出：驱动指示灯D1


需要用跳线帽短接P0.17
*/
//定义RTC0的驱动程序实例，0对应RTC0
const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2);

//RTC事件回调函数
 void rtc_handler(nrfx_rtc_int_type_t int_type)
{
    //判断产生的事件是否是TICK事件，本例中设置的TICK事件每125ms产生一次
	  if (int_type == NRFX_RTC_INT_TICK)
    {
        //翻转指示灯D1的状态 
			  nrf_gpio_pin_toggle(LED_1);
			
			
    }
}
//配置低频时钟，时钟源设置的是外部32.768KHz晶体
 void lfclk_config(void)
{
    //初始化时钟模块，设置低频时钟源
	  ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    //请求低频时钟，输入参数为NULL表示低频时钟启动后不产生事件
    nrf_drv_clock_lfclk_request(NULL);
}
//配置RTC
 void rtc_config(void)
{
    uint32_t err_code;
    
	  //定义RTC初始化配置结构体，并使用默认参数初始化
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
	  //重写分频系数，分频系数设置为4095时的递增频率 = 32768/(4095+1) = 8Hz，即每125ms COUNTER计数器递增一次。
	  //TICK事件是在COUNTER计数器递增时发生，所以TICK事件每125ms产生一次
    config.prescaler =4095;
	  //初始化RTC驱动程序实例的驱动，注册事件句柄
    err_code = nrfx_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //使能TICK事件
    nrfx_rtc_tick_enable(&rtc,true);
	  //启动RTC

    nrfx_rtc_enable(&rtc);
}

/***************************************************************************
* 描  述 : main函数 
* 入  参 : 无 
* 返回值 : int 类型
**************************************************************************/
void rtc_on(void)
{
	  //初始化开发板上的4个LED，即将驱动LED的GPIO配置为输出，
	  bsp_board_init(BSP_INIT_LEDS);

    //初始化低频时钟
    lfclk_config();
    //RTC配置
    rtc_config();

//    while (true)
//    {
//        //进入低功耗，等待事件唤醒
//			  __SEV();
//        __WFE();
//        __WFE();

 //   }
}
/********************************************END FILE**************************************/
