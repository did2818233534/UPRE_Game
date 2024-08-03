#ifndef __ROBSTRIDE01_H__
#define __ROBSTRIDE01_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "can.h"
#include "gpio.h"
	
	
enum RS01_Mode
{
	posationSpeedMode = 0,
	posationMode,
	speedMode,
	currentMode
};

//Use process: Mode selection -> Enable Motor -> Start the mode


	
void RS01_posationSpeedMode(uint8_t ID, float Posation, float Speed, float Torget, float Kp, float Kd);
//Posation							//Speed									//Torget								//Kp								//Kd
//	Range -720 to 720		//	Range -420 to 420		//	Range -17 to 17			//	Range 0 to 500	//	Range 0 to 5 
//	Units Angle					//	Units Rpm						//	Units Nm


void RS01_enableMotor(uint8_t ID);

void RS01_disableMotor(uint8_t ID);

void RS01_setOrigin(uint8_t ID);

void RS01_changeID(uint8_t ID, uint8_t New_ID);

void RS01_Mode_Change(uint8_t ID, enum RS01_Mode Mode);

void RS01_speedControl(uint8_t ID, float Speed);
void RS01_CurrentControl(uint8_t ID, float Current);


#ifdef __cplusplus
}
#endif

#endif 



