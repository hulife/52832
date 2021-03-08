#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#include <nrfx_rtc.h>
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"

#include "rtc.h"
/* ������Ҫ�õ�IK-52832DK�������е�ָʾ��D1��ռ�õ�nRF52832������Դ����
P0.17�����������ָʾ��D1


��Ҫ������ñ�̽�P0.17
*/
//����RTC0����������ʵ����0��ӦRTC0
const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2);

//RTC�¼��ص�����
 void rtc_handler(nrfx_rtc_int_type_t int_type)
{
    //�жϲ������¼��Ƿ���TICK�¼������������õ�TICK�¼�ÿ125ms����һ��
	  if (int_type == NRFX_RTC_INT_TICK)
    {
        //��תָʾ��D1��״̬ 
			  nrf_gpio_pin_toggle(LED_1);
			
			
    }
}
//���õ�Ƶʱ�ӣ�ʱ��Դ���õ����ⲿ32.768KHz����
 void lfclk_config(void)
{
    //��ʼ��ʱ��ģ�飬���õ�Ƶʱ��Դ
	  ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    //�����Ƶʱ�ӣ��������ΪNULL��ʾ��Ƶʱ�������󲻲����¼�
    nrf_drv_clock_lfclk_request(NULL);
}
//����RTC
 void rtc_config(void)
{
    uint32_t err_code;
    
	  //����RTC��ʼ�����ýṹ�壬��ʹ��Ĭ�ϲ�����ʼ��
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
	  //��д��Ƶϵ������Ƶϵ������Ϊ4095ʱ�ĵ���Ƶ�� = 32768/(4095+1) = 8Hz����ÿ125ms COUNTER����������һ�Ρ�
	  //TICK�¼�����COUNTER����������ʱ����������TICK�¼�ÿ125ms����һ��
    config.prescaler =4095;
	  //��ʼ��RTC��������ʵ����������ע���¼����
    err_code = nrfx_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //ʹ��TICK�¼�
    nrfx_rtc_tick_enable(&rtc,true);
	  //����RTC

    nrfx_rtc_enable(&rtc);
}

/***************************************************************************
* ��  �� : main���� 
* ��  �� : �� 
* ����ֵ : int ����
**************************************************************************/
void rtc_on(void)
{
	  //��ʼ���������ϵ�4��LED����������LED��GPIO����Ϊ�����
	  bsp_board_init(BSP_INIT_LEDS);

    //��ʼ����Ƶʱ��
    lfclk_config();
    //RTC����
    rtc_config();

//    while (true)
//    {
//        //����͹��ģ��ȴ��¼�����
//			  __SEV();
//        __WFE();
//        __WFE();

 //   }
}
/********************************************END FILE**************************************/
