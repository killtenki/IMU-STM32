#ifndef Imu_H
#define Imu_H

#include "main.h"
//====================Variables==========================//
static float const_angle_val;

extern uint8_t sum_data;
extern uint32_t Current_tick;

//==================Protocal_passcode========================//
typedef struct{
	const uint8_t PROTOCAL_HEADER;
	const uint8_t UNLOCK_CODE[5]; 
	const uint8_t SAVE_CODE[5];
}Imu_protocal_passcode;

//===================Imu_function_setting_passcode=============//
typedef struct{
	const uint8_t SET_ANGLE_REFERENCE[5];
}Imu_function_setting_passcode ;

//====================Imu_demand_data_type==========================//
typedef struct{
	const uint8_t ANGLE;
}Imu_demand_data_type;

//====================Sate_of_transmition==========================//
typedef enum{
	STOP_STATE,
	UNLOCK_STATE,
	SET_STATE,
	SAVE_STATE
}Imu_setting_state;
	

//====================Imu_data==========================//

typedef struct{
	float Roll;
	float Pitch;
	float Yaw;
	uint8_t  data[11];
	Imu_setting_state state;
	uint32_t last_tick;
}Imu_data;

//====================Functions==========================//
void Setting_IMU(const uint8_t Code[5]);
void Angle_data(void);
void Get_data(uint8_t requested_id);

#endif 
