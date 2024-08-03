#ifndef __AK60_H__
#define __AK60_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "can.h"
#include "gpio.h"
	
enum AK60_Control_Mode
{
	DutyMode = 0,     	//ռ�ձ�ģʽ
	CurrentLoopMode,		//������ģʽ
	BreakMode,					//ɲ��ģʽ
	SpeedLoopMode,			//�ٶȻ�ģʽ
	PosationMode,				//λ�û�ģʽ
	SetOriginMode,			//����ԭ��ģʽ
	PositionSpeedMode,	//λ���ٶȻ�˫��ģʽ
};


//Warning!!!
//When U Use The PositionMode(Or PositionSpeedMode),U Should Set Origion.
//Don't run so long times exceed Rated speed.


struct AK60_Motor
{
	float Posation;
	int32_t Speed;
	float Current;
	int8_t Temperature;
	uint8_t error;
};

void AK60_CAN_Transmit(uint16_t ID, enum AK60_Control_Mode Mode, const uint8_t *Data, uint8_t len);
void AK60_DutyMode(uint16_t ID, float Duty);//Don't use.
void Ak60_CurrentLoopMode(uint8_t ID, float Current);
void AK60_BreakMode(uint8_t ID, float BreakCurrent);
void AK60_SpeedLoopMode(uint8_t ID, float Speed);
//The range of speed is -1190 to 1190 
//Rated speed is ��420
//Unit: rpm

void AK60_PosationMode(uint8_t ID, float Posation);
//The range of posation is -36000 to 36000
//Uint: degree

void AK60_PositionSpeedMode(uint8_t ID, float Posation, int32_t Speed, int32_t Acceleration);
//This mode can control posation, speed and Acceleration at the same time
//The range of speed is -3900 to 3900
//Rated speed is ��420
//Uint: rpm
//The range of acceleration is 0 to 3900
//Unit:rpm/s

void AK60_SetOriginMode(uint8_t ID);


void AK_AnalysisData(uint8_t* Data, struct AK60_Motor* AK60_Motor);



#ifdef __cplusplus
}
#endif

#endif 

