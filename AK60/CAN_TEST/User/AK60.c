#include "AK60.h"


struct AK60_Motor
{
	float Posation;
	int32_t Speed;
	float Current;
	int8_t Temperature;
	uint8_t error;
};


void AK60_CAN_Transmit(uint16_t ID, enum AK60_Control_Mode Mode, const uint8_t *Data, uint8_t len)
{
	if(len > 8) len = 8;
	CAN_TxHeaderTypeDef Can_Tx;
	uint32_t pTxMailbox = 0;
	Can_Tx.StdId = 0;
	Can_Tx.ExtId = ID | (Mode << 8);
	Can_Tx.IDE = CAN_ID_EXT;
	Can_Tx.RTR = 0;
	Can_Tx.DLC = len;
	
	HAL_CAN_AddTxMessage(&hcan1,&Can_Tx,Data,&pTxMailbox);
}

void AK60_dutyMode(uint16_t ID, float Duty)
{

}


void Ak60_currentLoopMode(uint8_t ID, float Current)
{
	if(Current < -60) Current = -60;
	else if(Current > 60) Current = 60;
	
	int32_t Current_buff1 = Current * 1000;
	uint8_t Current_buff2[4];
	
	Current_buff2[3] = Current_buff1;
	Current_buff2[2] = Current_buff1 >> 8;
	Current_buff2[1] = Current_buff1 >> 16;
	Current_buff2[0] = Current_buff1 >> 24;
	
	AK60_CAN_Transmit(ID, CurrentLoopMode, Current_buff2, 4);
}


void AK60_breakMode(uint8_t ID, float BreakCurrent)
{
	if(BreakCurrent > 60) BreakCurrent = 60;
	else if(BreakCurrent < 0) BreakCurrent = 0;
	
	int32_t BreakCurrent_buff1 = BreakCurrent * 1000;
	uint8_t BreakCurrent_buff2[4];
	
	BreakCurrent_buff2[3] = BreakCurrent_buff1;
	BreakCurrent_buff2[2] = BreakCurrent_buff1 >> 8;
	BreakCurrent_buff2[1] = BreakCurrent_buff1 >> 16;
	BreakCurrent_buff2[0] = BreakCurrent_buff1 >> 24;
	
	AK60_CAN_Transmit(ID, BreakMode, BreakCurrent_buff2, 4);
}


void AK60_speedLoopMode(uint8_t ID, float Speed)
{
	int32_t Speed_buff = Speed * 6 * 14;
	if(Speed_buff > 100000) Speed_buff = 100000;
	else if(Speed_buff < -100000) Speed_buff = -100000;
	
	uint8_t Speed_buff1[4];
	Speed_buff1[3] = Speed_buff;
	Speed_buff1[2] = Speed_buff >> 8;
	Speed_buff1[1] = Speed_buff >> 16;
	Speed_buff1[0] = Speed_buff >> 24;
	
	AK60_CAN_Transmit(ID, SpeedLoopMode, Speed_buff1, 4);
}


void AK60_posationMode(uint8_t ID, float posation)
{
	if(posation > 36000) posation = 36000;
	else if(posation < -36000) posation = -36000;
	
	int32_t posation_buff1 = posation * 10000;
	uint8_t posation_buff2[4] = {posation_buff1 >> 24, posation_buff1 >> 16, posation_buff1 >> 8, posation_buff1};
	
	AK60_CAN_Transmit(ID, PosationMode, posation_buff2, 4);
}


void AK60_setOriginMode(uint8_t ID)
{
	uint8_t buff = 0;
	AK60_CAN_Transmit(ID, SetOriginMode, &buff, 1);
}


void AK60_positionSpeedMode(uint8_t ID, float posation, int32_t Speed, int32_t Acceleration)
{
	if(posation > 36000) posation = 36000;
	else if(posation < -36000) posation = -36000;

	if(Speed > 3900) Speed = 3900;
	else if(Speed < -3900) Speed = -3900;
	
	if(Acceleration > 3900) Acceleration = 3900;
	else if(Acceleration < 0) Acceleration = 0;
	
	int32_t posation_buff = posation * 10000;
	int16_t Speed_buff = Speed / 10 * 6 * 14;
	int16_t Acceleration_buff = Acceleration / 10 * 6 * 14;
	
	uint8_t buff[8] = {posation_buff >> 24, posation_buff >> 16, posation_buff >> 8, posation_buff,
										Speed_buff >> 8, Speed_buff, Acceleration_buff, Acceleration_buff >> 8};
	
	AK60_CAN_Transmit(ID, PositionSpeedMode, buff, 8);
}


void AK60_analysisData(uint8_t* Data, struct AK60_Motor* AK60_Motor)
{
	AK60_Motor->Posation = ((Data[0] << 8) | Data[1]) / 10;
	AK60_Motor->Speed = ((Data[2] << 8) | Data[3]) * 10;
	AK60_Motor->Current = ((Data[4] << 8) | Data[5]) / 100;
	AK60_Motor->Temperature = Data[6];
	AK60_Motor->error = Data[7];
	
}

