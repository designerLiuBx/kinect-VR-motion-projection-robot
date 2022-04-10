#include "Gen_include.h"
//Lab  : 601
//autor£ºliuBaixu gaoHaozhen

int main(void)
{
	flag.start_ok = All_init();
	//OLED_Clear2();
	//OLED_DrawPoint(25,25,1);
	//OLED_Refresh_Gram();
	//while(1);
	while(1){
		if(flag.start_ok==1)
		{
			Scheduler_Run();
		}
	}
}
