#include "PID.h"


//Struct define






//Function define


//struct PID_Current_HandleTypeDef* pid_Current(uint8_t ID,float Current_expect,
//																int16_t Current_now,
//																float kpC, float kiC, float kdC)
//{
//#define Current_integral_limit 5
//	

//	PID_Current[ID].err = Current_expect-Current_now;
//	PID_Current[ID].integral += PID_Current[ID].err;
//	
//  if(PID_Current[ID].integral>Current_integral_limit) PID_Current[ID].integral = Current_integral_limit;
//	else if (PID_Current[ID].integral < -Current_integral_limit) PID_Current[ID].integral = -Current_integral_limit;	

//	
//	
//	PID_Current[ID].pout = kpC * PID_Current[ID].err;
//	PID_Current[ID].iout = kiC * PID_Current[ID].integral;
//	PID_Current[ID].dout = kdC *(PID_Current[ID].err-PID_Current[ID].err_last);
//	
//	PID_Current[ID].Current_set = PID_Current[ID].pout + PID_Current[ID].iout + PID_Current[ID].dout;
//	
//	PID_Current[ID].err_last = PID_Current[ID].err;
//	
//	if(PID_Current[ID].Current_set > 16384) PID_Current[ID].Current_set = 16384;
//	if(PID_Current[ID].Current_set < -16384) PID_Current[ID].Current_set = -16384;
//	
//	return &PID_Current[ID];
//}




void pid_Speed(uint8_t ID,struct PID_Speed_HandleTypeDef* PID_Speed,
							float speed_expect,
							int16_t speed_now,
							float kpV, float kiV, float kdV)
{
	if(speed_expect > 500) speed_expect = 500;
	else if(speed_expect < -500) speed_expect = -500;
#define Speed_integral_limit 5
	
	speed_expect *= 36;
	
//	PID_Speed[ID].FilterSpeed = 0.3 * speed_now + 0.7 * PID_Speed[ID].FilterSpeed_last;
	
//	PID_Speed[ID].err = speed_expect-PID_Speed[ID].FilterSpeed;
	PID_Speed[ID].err = speed_expect-speed_now;
	PID_Speed[ID].integral += PID_Speed[ID].err;
	
//	if(((val_expect - val_now)<val_expect/20)&&((val_now - val_expect)<val_expect/10)) 
//	{
////		PID_Val.integral = 0;
//		kpV /= 10;
//	}
	
	if(PID_Speed[ID].integral > Speed_integral_limit) PID_Speed[ID].integral = Speed_integral_limit;
	else if (PID_Speed[ID].integral < -Speed_integral_limit) PID_Speed[ID].integral = -Speed_integral_limit;
	
	PID_Speed[ID].pout = kpV * PID_Speed[ID].err;
	PID_Speed[ID].iout = kiV * PID_Speed[ID].integral;
	PID_Speed[ID].dout = kdV *(PID_Speed[ID].err-PID_Speed[ID].err_last);
	PID_Speed[ID].Current_set = PID_Speed[ID].pout + PID_Speed[ID].iout + PID_Speed[ID].dout;
	//PID_Speed[ID].Current_set = pid_Current(ID,PID_Speed[ID].Current_set,Current_now,5,3,3)->Current_set;
	PID_Speed[ID].err_last = PID_Speed[ID].err;
//	PID_Speed[ID].FilterSpeed_last = PID_Speed[ID].FilterSpeed;
		
   
	if(PID_Speed[ID].Current_set>9000) PID_Speed[ID].Current_set=9000;
	else if(PID_Speed[ID].Current_set<-9000) PID_Speed[ID].Current_set=-9000;
}





void pid_Angle(uint8_t ID,
							struct PID_Angle_HandleTypeDef* PID_Angle,struct PID_Speed_HandleTypeDef* PID_Speed,
							float angle_expect,
							float angle_now, int16_t speed_now,
							float kpA, float kiA, float kdA,
							float kpV, float kiV, float kdV)
{
#define Angle_integral_limit 50
	
	
	angle_expect *= 36;
	angle_now *= 36;
	
	PID_Angle[ID].err = angle_expect-angle_now;
	PID_Angle[ID].integral += PID_Angle[ID].err;
	
	
	if(PID_Angle[ID].integral > Angle_integral_limit) PID_Angle[ID].integral = Angle_integral_limit;
	else if (PID_Angle[ID].integral < -Angle_integral_limit) PID_Angle[ID].integral = -Angle_integral_limit;
	
	
	
	
	
	PID_Angle[ID].pout = kpA * PID_Angle[ID].err;
	PID_Angle[ID].iout = kiA * PID_Angle[ID].integral;
	PID_Angle[ID].dout = kdA *(PID_Angle[ID].err-PID_Angle[ID].err_last);
	
	PID_Angle[ID].speed_set = PID_Angle[ID].pout + PID_Angle[ID].iout + PID_Angle[ID].dout;
	
	
	pid_Speed(ID, PID_Speed, PID_Angle[ID].speed_set, speed_now, kpV, kiV, kdV);
	PID_Angle[ID].Current_set = PID_Speed[ID].Current_set;
	PID_Angle[ID].err_last = PID_Angle[ID].err;
	
//	if(((angle_expect - angle_now)<200)&&((angle_now - angle_expect)<200)) PID_Angle[ID].Current_set=0;
//	if(((angle_expect - angle_now)<angle_expect/10)&&((angle_now - angle_expect)<angle_expect/10)) PID_Angle[ID].integral = 0;
	
		if(PID_Angle[ID].Current_set>9000) PID_Angle[ID].Current_set=9000;
	else if(PID_Angle[ID].Current_set<-9000) PID_Angle[ID].Current_set=-9000;
}


void pid_Angle_and_Speed(uint8_t ID,
												struct PID_Angle_and_Speed_HandleTypeDef* PID_Angle_and_Speed,struct PID_Speed_HandleTypeDef* PID_Speed,
												float angle_expect,float speed_expect,
												int16_t angle_now, int16_t speed_now,int16_t Current_now,
												float kpA, float kiA, float kdA,
												float kpV, float kiV, float kdV)
{
#define Angle_integral_limit 50
	
	angle_expect *= 36;
	speed_expect *= 36;
	
	PID_Angle_and_Speed[ID].err = angle_expect-angle_now;
	PID_Angle_and_Speed[ID].integral += PID_Angle_and_Speed[ID].err;
	
	
	if(PID_Angle_and_Speed[ID].integral > Angle_integral_limit) PID_Angle_and_Speed[ID].integral = Angle_integral_limit;
	else if (PID_Angle_and_Speed[ID].integral < -Angle_integral_limit) PID_Angle_and_Speed[ID].integral = -Angle_integral_limit;
	
	
	PID_Angle_and_Speed[ID].pout = kpA * PID_Angle_and_Speed[ID].err;
	PID_Angle_and_Speed[ID].iout = kiA * PID_Angle_and_Speed[ID].integral;
	PID_Angle_and_Speed[ID].dout = kdA *(PID_Angle_and_Speed[ID].err-PID_Angle_and_Speed[ID].err_last);
	
	PID_Angle_and_Speed[ID].speed_set = PID_Angle_and_Speed[ID].pout + PID_Angle_and_Speed[ID].iout + PID_Angle_and_Speed[ID].dout;
	if(PID_Angle_and_Speed[ID].speed_set > speed_expect * 0.6) PID_Angle_and_Speed[ID].speed_set = speed_expect;
	pid_Speed(ID, PID_Speed, PID_Angle_and_Speed[ID].speed_set, speed_now,  kpV, kiV, kdV);
	PID_Angle_and_Speed[ID].Current_set = PID_Speed[ID].Current_set;
	PID_Angle_and_Speed[ID].err_last = PID_Angle_and_Speed[ID].err;
	
	
	if(((angle_expect - angle_now)<angle_expect/10)&&((angle_now - angle_expect)<angle_expect/10)) PID_Angle_and_Speed[ID].integral = 0;
	
	//	if(PID_Val.Current_set>16300) PID_Val.Current_set=16300;
//	else if(PID_Val.Current_set<16300) PID_Val.Current_set=-16300;
}





