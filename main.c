#include "ble1.h"
/**@brief Application main function.
 */
 
custome_event print_event;
 
 
 void print_world(void *p_event_data, uint16_t event_size)    
{
  custome_event *evt = (custome_event*)p_event_data;
 if(print_event.flag==1)
 {
//pingmu_Init();	
//black_a();	
	
//registHeightCallback(EpdDisplay_taskFxn,NULL,NULL);
	 
	
 }
}
 void print_timer_handler(void *p_contex){

       print_event.flag = 1;    

       app_sched_event_put(&print_event, sizeof(print_event), print_world);//数据和处理函数放入scheduler队列

}
 
int main(void)
{
	
	ret_code_t err_code;
	
	  //初始化开发板上的4个LED，即将驱动LED的GPIO配置为输出，
	  bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS);
	  
		APP_SCHED_INIT(sizeof(custome_event),1);
   
		bool erase_bonds;
    log_init(); 
	
		//gpio_init();
	
		lfclk_config();
    // Initialize.
    uart_init();
/*	
		spi_t();	
		EPD_7IN5B_V2_Init();
  	black_a();
*/
/*
	  spi_t();	

		test_screen();
*/
	
		spi_t();	
		EPD_7IN5B_V2_Init();
		Epaper_Write_Command(0x13);

    timers_init();
    buttons_leds_init(&erase_bonds);
    power_management_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
		
	  //Alm_init1();//值操作
		
    services_init();
		
		advertising_init();
		
		mac_set();
    conn_params_init();
		
    // Start execution.
    printf("\r\nUART started.\r\n");
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
    advertising_start();
		
		throughput_test();
    // Enter main loop.
    for (;;)
    {
			if(flag==1)
	{
//	   black_a();
//		 printf("%d",flag);
//		 flag=0;
		
		 print_event.flag = 1;    
     app_sched_event_put(&print_event, sizeof(print_event), print_world);//数据和处理函数放入scheduler队列

	//	flag=0;
	}
	
			app_sched_execute();		
      idle_state_handle();
						
    }
}


/**
 * @}
 */
