////���õ�C��ͷ�ļ�
//#include <stdbool.h>
//#include <stdint.h>
//#include <string.h>
////Log��Ҫ���õ�ͷ�ļ�
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
////��ʼ����־��ӡģ��
//static void log_init(void)
//{
//    //��ʼ��log����ģ��
//	  ret_code_t err_code = NRF_LOG_INIT(NULL);
//    APP_ERROR_CHECK(err_code);
//    //����log����նˣ�����sdk_config.h�е�������������ն�ΪUART����RTT��
//    NRF_LOG_DEFAULT_BACKENDS_INIT();
//}

////����״̬������������������־
//static void idle_state_handle(void)
//{
////    ��������log
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





////������
//int main(void)
//{

////	TASKS_LOWPWR
//	 
//	
//	//��ʼ��log����ģ��
//	//nrf_gpio_cfg_sense_input();

//	//log_init();
//	//NRF_LOG_INFO("BLE ");
//		
////	pingmu_Init();
////	black_a();//��ʾȫ
////	nrf_delay_ms(5);
//	
////	rtc_on();//RTC��ʱ
//	 ble_main();
////	ble_led();
//	//LOG��ӡ��Ϣ
//	//NRF_LOG_INFO("BLE Template example:Log use uart as output terminal");

//  //��ѭ��
////	while(true)
////	{
////����͹��ģ��ȴ��¼�����
//	

////��������LOG�����е�Դ����
////	idle_state_handle();
//	//gpio_off();
//	//sys_off();
////	}
//}

