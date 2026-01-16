#include "Imu.h"

extern UART_HandleTypeDef huart1;


const Imu_protocal_passcode protocal_passcode = { 
	.PROTOCAL_HEADER = 0x55,
	.UNLOCK_CODE = {0xFF, 0XAA, 0X69, 0X88, 0XB5},
	.SAVE_CODE = {0xFF, 0XAA, 0X00, 0X00, 0X00}
};

const Imu_function_setting_passcode function_passcode = {
	.SET_ANGLE_REFERENCE =  {0xFF, 0XAA, 0X01, 0X08, 0X00}
};

const Imu_demand_data_type data_type = {
	.ANGLE = 0x53
};

Imu_data Imu = {
	.Roll = 0.0f,
	.Pitch = 0.0f,
	.Yaw = 0.0f
};
//==================Decode_angle_data--------------------------//
void Angle_data(){

	static float const_angle_val = 180.0f/32768.0f;		
	uint8_t sum_data = 0;
	if (Imu.data[0] != protocal_passcode.PROTOCAL_HEADER || Imu.data[1] != 0x53) return;
	
	for(int i = 0; i < 10; i++){
		sum_data += Imu.data[i];
	}
	
	if (sum_data == (uint8_t)Imu.data[10] ){
		Imu.Roll  = ((int16_t)(Imu.data[3] << 8 |Imu.data[2]))*const_angle_val;
		Imu.Pitch = ((int16_t)(Imu.data[5] << 8 |Imu.data[4]))*const_angle_val;
		Imu.Yaw   = ((int16_t)(Imu.data[7] << 8 |Imu.data[6]))*const_angle_val;
	}

}


//==================Requested_data--------------------------//
void Get_data(uint8_t requested_id){ 
		static uint8_t packet[2];
    packet[0] = protocal_passcode.PROTOCAL_HEADER;
    packet[1] = requested_id;
    HAL_UART_Transmit_IT(&huart1, packet, 2);
}
//======================Set_offset(zero)===============================//	

void Setting_IMU(const uint8_t Code[5]){ //=================== function_passcode.<loai function muon dung> ==================///
	uint32_t Current_tick = HAL_GetTick();

	switch(Imu.state){
		case UNLOCK_STATE:
			HAL_UART_Transmit_IT(&huart1, protocal_passcode.UNLOCK_CODE, 5);
			Imu.last_tick = Current_tick;
			Imu.state = SET_STATE;

			break;
		case SET_STATE:
			if(Current_tick- Imu.last_tick >= 10){
				HAL_UART_Transmit_IT(&huart1, Code, 5);
				Imu.last_tick = Current_tick;
				Imu.state = SAVE_STATE;

			}
			break;
		case SAVE_STATE:
			if(Current_tick- Imu.last_tick >= 10){
				HAL_UART_Transmit_IT(&huart1, protocal_passcode.SAVE_CODE, 5);
				Imu.state = STOP_STATE;

			}			
			break;
		default: break;
	}
}

	
	



