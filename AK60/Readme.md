AK60驱动使用指南

该电机有6种运动模式,其中占空比模式用的较少故没写驱动

速度环模式刹车模式,和电流环模式可直接使用

void Ak60_currentLoopMode(uint8_t ID, float Current);

void Ak60_currentLoopMode(uint8_t ID, float Current);

void AK60_breakMode(uint8_t ID, float BreakCurrent);

位置环模式或者位置速度加速度三环模式使用前需要先设置原点

void AK60_setOriginMode(uint8_t ID);

void AK60_positionSpeedMode(uint8_t ID, float Posation, int32_t Speed, int32_t Acceleration);

void AK60_posationMode(uint8_t ID, float Posation);

速度单位为rmp,加速度单位为rmp/s,角度单位为弧度,电流环不常用,单位暂无
