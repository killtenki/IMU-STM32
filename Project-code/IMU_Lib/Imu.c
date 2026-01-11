#include "imu.h"
extern UART_HandleTypeDef huart1;

uint8_t  imu_data[11];
volatile uint8_t enable_read_angle_data = 0;
volatile uint8_t set_offset = 0;

uint8_t protocal_header = 0x55;
uint8_t adr_angle = 0x53;
uint8_t unlock_code[5] = {0xFF, 0XAA, 0X69, 0X88, 0XB5}; 
uint8_t save_code[5] = {0xFF, 0XAA, 0X00, 0X00, 0X00};
uint8_t set_angle_reference[5] = {0xFF, 0XAA, 0X01, 0X08, 0X00};


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if ( huart->Instance == USART1){
		static uint8_t index = 0;
	
		
		if(index == 0 && imu_data[0] != 0x55){
			HAL_UART_Receive_IT(&huart1, (uint8_t*)&imu_data[0], 1);
			return;
		}
		
		index ++;
		
		if (index >= 11){
			index = 0;
			enable_read_angle_data = 1;
		}
		
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&imu_data[index], 1);
	}
}

void Angle_data(){
	static float Roll, Pitch, Yaw;
	static float const_angle_val = 180.0f/32768.0f;		
	uint8_t sum_data = 0;
	if (imu_data[0] != 0x55 || imu_data[1] != 0x53) return;
	
	for(int i = 0; i < 10; i++){
		sum_data += imu_data[i];
	}
	
	if (sum_data == (uint8_t)imu_data[10] ){

		Roll  = ((int16_t)(imu_data[3] << 8 |imu_data[2]))*const_angle_val;
		Pitch = ((int16_t)(imu_data[5] << 8 |imu_data[4]))*const_angle_val;
		Yaw   = ((int16_t)(imu_data[7] << 8 |imu_data[6]))*const_angle_val;
	}

}

void Demand_angle_data(){
	HAL_UART_Transmit(&huart1, &protocal_header, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, &adr_angle, 1, HAL_MAX_DELAY);
}


void Setting_angle_data(){
	HAL_UART_Transmit(&huart1, &protocal_header, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, &adr_angle, 1, HAL_MAX_DELAY);
}

void set_zero(){
		HAL_UART_Transmit(&huart1, unlock_code, 5, HAL_MAX_DELAY);
		HAL_Delay(36);
		HAL_UART_Transmit(&huart1, set_angle_reference, 5, HAL_MAX_DELAY);
		HAL_Delay(36);
		HAL_UART_Transmit(&huart1, save_code, 5, HAL_MAX_DELAY);
		HAL_Delay(36);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_15){
		set_offset = 1;
	}
}

void Imu_init(){
	HAL_UART_Receive_IT(&huart1, (uint8_t*)&imu_data[0], 1);
	set_zero();
	HAL_Delay(100);
	Demand_angle_data();
}
