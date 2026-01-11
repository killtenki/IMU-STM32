#ifndef IMU_H
#define IMU_H

#include "main.h" 

extern uint8_t protocal_header;
extern uint8_t adr_angle ;
 
extern uint8_t  imu_data[11];
extern volatile uint8_t enable_read_angle_data;
extern volatile uint8_t set_offset;

void Demand_angle_data(void);
void Angle_data(void);
void set_zero(void);
void Imu_init(void);
#endif 
