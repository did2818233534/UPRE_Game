#include "Vofa.h"


void Vofa_Transmit_8(float Data1,float Data2,float Data3,float Data4,
										float Data5,float Data6,float Data7,float Data8)
{
	float Data[8];
	uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};
	Data[0] = Data1;
	Data[1] = Data2;
	Data[2] = Data3;
	Data[3] = Data4;
	Data[4] = Data5;
	Data[5] = Data6;
	Data[6] = Data7;
	Data[7] = Data8;

	
	HAL_UART_Transmit(&huart1,(uint8_t*)Data,sizeof(float)*8,0xff);
	HAL_UART_Transmit(&huart1,tail,4,0xff);
}


