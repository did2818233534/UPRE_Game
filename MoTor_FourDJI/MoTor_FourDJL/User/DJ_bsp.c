#include "DJ_bsp.h"


void Motor_Set_Current(uint16_t *DATA)
{
	CAN_TxHeaderTypeDef Can_Tx;
	uint8_t DATA_T[8];
	uint32_t pTxMailbox=0;
	
	Can_Tx.IDE=CAN_ID_STD;
	Can_Tx.RTR=CAN_RTR_DATA;
	Can_Tx.DLC=0x08;
	Can_Tx.StdId=0x200;	
	
	DATA_T[0] = DATA[0] >> 8;
	DATA_T[1] = DATA[0];
	DATA_T[2] = DATA[1] >> 8;
	DATA_T[3] = DATA[1];
	DATA_T[4] = DATA[2] >> 8;
	DATA_T[5] = DATA[2];
	DATA_T[6] = DATA[3] >> 8;
	DATA_T[7] = DATA[3];
	
	HAL_CAN_AddTxMessage(&hcan1,&Can_Tx,DATA_T,&pTxMailbox);
}

