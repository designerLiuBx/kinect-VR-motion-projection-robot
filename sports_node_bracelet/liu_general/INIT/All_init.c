#include "All_init.h"


u8 All_init(void)
{ 
	
	uart_init(115200);
	usart3_init(115200);
	usart2_init(115200);
	uart4_init(38400);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	ANO_V7_SendOutPut_uart = USART1;
	
	delay_init(180);
	LED_Init();		    //初始化LED端口	
	KEY_Init();       //初始化与按键连接的硬件接口
	OLED_Init();
	OLED_Roll(1,1,50);
	dev_SysTick_init();
	//SPI4_Init();	
	//ADS1292_Init();
	OLED_Clear();
	ADC1_Init();
	fir_init();
	TIM3_Int_Init(3000,45);
	LED1=1;	LED2=1; LED0=1;
	flag.display = 1;
	flag.index = 1;
	delay_ms(200);

	return 1;
}
