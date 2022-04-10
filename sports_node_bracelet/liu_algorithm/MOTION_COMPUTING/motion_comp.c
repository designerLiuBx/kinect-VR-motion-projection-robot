#include "motion_comp.h"
extern char UART4_Buffer[2];
#define N  50
void main_controller(void)
{
	mode_check();
	if( 2 == flag.display)
	{
		switch(flag.index)
		{			
			case 1:
				break;
			case 2:
				OLED_Temperature(measure_T());
				break;
			case 3:
				motion_comp();
				break;
			case 4:
				break;
		}
	}
}
void mode_check(void)
{ //index indexDst display
	Key_Run();
	oled_mode_display();
}
void get_acc(void)
{
	acc.x = stcAcc.a[0];
	acc.y = stcAcc.a[1];
	acc.z = stcAcc.a[2];
}

void motion_comp(void)
{
	const int ZHONG = -18000;
	static double accDst = 0;
	static int 	angleYaw = 0;
	static int  last_angleYaw = 0;
	get_acc();
	accDst = getCubeRoot ((double)(acc.x*acc.x + acc.y*acc.y + acc.z*acc.z ));
//	accDst = lowfilter3(accDst);
	accDst = (int)(accDst);
	angleYaw = motion.angle;
//	JY61_Signal_inputF32= accDst;	
//	arm_fir_f32(&JY61_S,&JY61_Signal_inputF32, &JY61_Signal_outputF32,1);
//	accDst =JY61_Signal_outputF32;	
	if( ((last_angleYaw > ZHONG) && (angleYaw < ZHONG)) || ((last_angleYaw < ZHONG) && (angleYaw > ZHONG)) )
	{
		//if( (accDst >157) || (accDst <120))
	//	{
			motion.steps ++;
			motion.dis += UART4_Buffer[0];
//		}
	}
	//============
	
	//============
	//	acc.dst = (int)(accDst);	

	//acc.dst = acc.dst + (int)(accDst-170);	
	last_angleYaw = angleYaw;
}
double getCubeRoot(double Y)
{
	double an1 = 0;
	double an = 1;
	while (1)
	{

		an1 = (2*an + Y / (an*an)) / 3; 

		if (fabs(an1 - an) < 0.0001)
		{
			break;
		}
		an = an1; 
	}
	return an1;

}
double lowfilter(double x)
{
	double a, baro,baroOffset,baroAlt;
	a = 0.05;
  baroOffset  = x;
  baro = a * baroOffset  + (1.0f - a) * baroAlt;
  baroAlt = baro;
  return baro;  
}
char lowfilter2(double x)  
{  

	 
   char count,i,j;  

   char value_buf[N];  

   int  sum=0,temp=0;  
	
		x = (char)x;
   for  (count=0;count<N;count++)  

   {  

      value_buf[count] = x;  

      delay_ms(1);  

   }  

   for (j=0;j<N-1;j++)  

   {  

      for (i=0;i<N-j;i++)  

      {  

         if ( value_buf[i]>value_buf[i+1] )  

         {  

            temp = value_buf[i];  

            value_buf[i] = value_buf[i+1];   

             value_buf[i+1] = temp;  
         }  
      }  
   }  

   for(count=1;count<N-1;count++)  

      sum += value_buf[count];  

   return (char)(sum/(N-2));  

}  

double lowfilter3(double x)  
{  
	 double dx , y, last_x;
	 dx = x - last_x;
	 y = x + 0.5*dx;
	return y;
}  

