#include "Robstride01.h"

enum RS01_Cmd
{
	getMotorID = 0,        			//Get the id of the motor. Generally speaking, u wouldn't use it
	posationSpeedControl = 1, 		 	//U can ctrl posation and speed at the same time
	enableMotor = 3,							//Enable motor
	disableMotor = 4,						//Disable motor
	setOrigin = 6,    		  //Set Origin, power-off loss
	changeID = 7,        		  	//Change the id of motor
	singleParameterRead = 17,		//Only read one parameter
	singleParameterWrite = 18		//Only write one parameter
};


void RS01_CAN_Transmit(uint8_t ID, enum RS01_Cmd Cmd, uint16_t Data1, const uint8_t *Data2)
{
	CAN_TxHeaderTypeDef Can_Tx;
	uint32_t pTxMailbox = 0;
	Can_Tx.StdId = 0;
	Can_Tx.ExtId = ID | (Data1 << 8) | (Cmd << 24);
	Can_Tx.IDE = CAN_ID_EXT;
	Can_Tx.RTR = 0;
	Can_Tx.DLC = 8;
	
	HAL_CAN_AddTxMessage(&hcan1,&Can_Tx,Data2,&pTxMailbox);
}


void RS01_getMotorID(uint8_t ID)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0}; 
	RS01_CAN_Transmit(ID, getMotorID, Data1, Data2);
}


void RS01_posationSpeedControl(uint8_t ID, float Posation, float Speed, float Torget, float Kp, float Kd)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0};
	
	if(Torget > 17) Torget = 17;
	else if(Torget < - 17) Torget = -17;
	
	if(Posation > 720) Posation = 720;
	else if(Posation < -720) Posation = -720;
	
	if(Speed > 420) Speed = 420;
	else if(Speed < -420) Speed = -420;
	
	Torget += 17;
	Torget = Torget / 34 * 65535;
	Data1 = Torget;
	
	Speed += 420;
	Speed = Speed / 840 * 65535;
	uint16_t Speed_buff = Speed;
	Data2[2] = Speed_buff >> 8;
	Data2[3] = Speed_buff;
	
	Posation += 720;
	Posation = Posation / 1440 * 65535;
	uint16_t Posation_buff = Posation;
	Data2[0] = Posation_buff >> 8;
	Data2[1] = Posation_buff;
	
	Kp = Kp / 500 * 65535;
	Kd = Kd / 5 * 65535;
	uint16_t Kp_buff = Kp;
	uint16_t Kd_buff = Kd;
	Data2[4] = Kp_buff >> 8;
	Data2[5] = Kp_buff;
	Data2[6] = Kd_buff >> 8;
	Data2[7] = Kd_buff;
	
	RS01_CAN_Transmit(ID, posationSpeedControl, Data1, Data2);
}


void RS01_enableMotor(uint8_t ID)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0}; 
	RS01_CAN_Transmit(ID, enableMotor, Data1, Data2);
}


void RS01_disableMotor(uint8_t ID)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0}; 
	RS01_CAN_Transmit(ID, disableMotor, Data1, Data2);
}


void RS01_setOrigin(uint8_t ID)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0}; 
	RS01_CAN_Transmit(ID, setOrigin, Data1, Data2);
}


void RS01_changeID(uint8_t ID, uint8_t New_ID)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0}; 
	Data1 = New_ID << 8;
	RS01_CAN_Transmit(ID, changeID, Data1, Data2);
}




void RS01_singleParameterWrite(uint8_t ID, uint16_t index, uint8_t *Parameter_Data)
{
	uint16_t Data1 = 0;
	uint8_t Data2[8] = {0}; 
	Data2[0] = index;
	Data2[1] = index >> 8;
	Data2[2] = 0;
	Data2[3] = 0;
	Data2[4] = Parameter_Data[0];
	Data2[5] = Parameter_Data[1];
	Data2[6] = Parameter_Data[2];
	Data2[7] = Parameter_Data[3];
	
	RS01_CAN_Transmit(ID, singleParameterWrite, Data1, Data2);
}


void RS01_Mode_Change(uint8_t ID, enum RS01_Mode Mode)
{
	uint8_t Data[4] = {Mode,0x00,0x00,0x00};
	RS01_singleParameterWrite(ID, 0x7005, Data);
}


void RS01_speedControl(uint8_t ID, float Speed) //unit: rpm
{
	if(Speed > 420) Speed = 420;
	else if(Speed < -420) Speed = -420;
	Speed = Speed / 60 * 2 * 3.1415926535;
	
	RS01_singleParameterWrite(ID, 0x700A, (uint8_t*)&Speed);
}

void RS01_CurrentControl(uint8_t ID, float Current)//unit: A
{
	if(Current > 23) Current = 23;
	else if(Current < -23) Current = -23;
	RS01_singleParameterWrite(ID, 0x7006, (uint8_t *)&Current);
}

