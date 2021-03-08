////引用的C库头文件
//#include <stdbool.h>
//#include <stdint.h>
//#include <string.h>
////Log需要引用的头文件
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#include "nrf_log_default_backends.h"

//#include "nrf_pwr_mgmt.h"

//#include "nrf_delay.h"
//#include "nrf_gpio.h"
//#include "boards.h"	
//#include "Image_583.h" 
//#include "board_spi.h"
////#include "spi_lib.h"
//#include "spi_ble.h"
//#include "spi_lib_75.h"

//#include "rtc.h"
//#include "ble_led.h"
//#include "ble_tem.h"
////#include "alm_init_profile.h"
////初始化日志打印模块
//static void log_init(void)
//{
//    //初始化log程序模块
//	  ret_code_t err_code = NRF_LOG_INIT(NULL);
//    APP_ERROR_CHECK(err_code);
//    //设置log输出终端（根据sdk_config.h中的配置设置输出终端为UART或者RTT）
//    NRF_LOG_DEFAULT_BACKENDS_INIT();
//}

////空闲状态处理函数。处理挂起的日志
//static void idle_state_handle(void)
//{
////    处理挂起的log
////	  if (NRF_LOG_PROCESS() == false)
////    {

////    }
//		if (NRF_LOG_PROCESS() == false)
//    {
//        nrf_pwr_mgmt_run();
//    }

//}

//static void dcdc_on(void)
//{
//	NRF_POWER->DCDCEN = 1;//dcdc
//}
//static void sys_off(void)
//{
//	NRF_POWER->SYSTEMOFF = 1;//system off
//}



//static void gpio_off(void)
//{
//	nrf_gpio_cfg_default(pingmu_RST_IO);
//	nrf_gpio_cfg_default(pingmu_BUSY_IO);
//	nrf_gpio_cfg_default(BOARD_DC_IO);
//	nrf_gpio_cfg_default(BOARD_SPI0_CSN_IO);

//}





////主函数
//int main(void)
//{

////	TASKS_LOWPWR
//	 
//	
//	//初始化log程序模块
//	//nrf_gpio_cfg_sense_input();

//	//log_init();
//	//NRF_LOG_INFO("BLE ");
//		
////	pingmu_Init();
////	black_a();//显示全
////	nrf_delay_ms(5);
//	
////	rtc_on();//RTC定时
//	 ble_main();
////	ble_led();
//	//LOG打印信息
//	//NRF_LOG_INFO("BLE Template example:Log use uart as output terminal");

//  //主循环
////	while(true)
////	{
////进入低功耗，等待事件唤醒
//	

////处理挂起的LOG和运行电源管理
////	idle_state_handle();
//	//gpio_off();
//	//sys_off();
////	}
//}

