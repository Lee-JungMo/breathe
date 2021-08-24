/*
 * DPS310.h
 *
 *  Created on: 2020. 5. 5.
 *      Author: Joonhwa Choi, pass2774@gmail.com
 *      Based on the DPS310 datasheet below:
 *      https://www.infineon.com/dgdl/Infineon-DPS310-DataSheet-v01_01-EN.pdf?fileId=5546d462576f34750157750826c42242
 */

#ifndef DPS310_I2C_H_
#define DPS310_I2C_H_

#include <ti/drivers/I2C.h>
#include <stdint.h>


#define DPS310_DEVICE_ID 0x76   //Change it according to your pin configuration. 0x76 if SDO is pulled down to GND, else 0x77
#define DPS310_PSR_B2 0x00
#define DPS310_PSR_B1 0x01
#define DPS310_PSR_B0 0x02
#define DPS310_TMP_B2 0x03
#define DPS310_TMP_B1 0x04
#define DPS310_TMP_B0 0x05
#define DPS310_PRS_CFG 0x06
#define DPS310_TMP_CFG 0x07
#define DPS310_MEAS_CFG 0x08
#define DPS310_CFG_REG 0x09
#define DPS310_Product_ID 0x0D // response=0x10 // this is the register that contains the part ID


//Pressure measurement rate. Applicable for measurements in Background mode only.
#define PM_RATE_1_PER_SEC 0b000<<4
#define PM_RATE_2_PER_SEC 0b001<<4
#define PM_RATE_4_PER_SEC 0b010<<4
#define PM_RATE_8_PER_SEC 0b011<<4
#define PM_RATE_16_PER_SEC 0b100<<4
#define PM_RATE_32_PER_SEC 0b101<<4
#define PM_RATE_64_PER_SEC 0b110<<4
#define PM_RATE_128_PER_SEC 0b111<<4

//Pressure measurement precision
#define PM_PRC_SINGLE    0b0000 //Low precision
#define PM_PRC_2_TIMES   0b0001 //Low Power
#define PM_PRC_4_TIMES   0b0010
#define PM_PRC_8_TIMES   0b0011
#define PM_PRC_16_TIMES  0b0100 //*Standard
#define PM_PRC_32_TIMES  0b0101 //*
#define PM_PRC_64_TIMES  0b0110 //*High precision
#define PM_PRC_128_TIMES 0b0111 //*
//Note - *: use in combination with a bit shift. See Interrupt and FIFO configuration(CFG_REG) register chapter in the datasheet.



//Temperature measurement rate. Applicable for measurements in Background mode only.
#define TMP_RATE_1_PER_SEC 0b000<<4
#define TMP_RATE_2_PER_SEC 0b001<<4
#define TMP_RATE_4_PER_SEC 0b010<<4
#define TMP_RATE_8_PER_SEC 0b011<<4
#define TMP_RATE_16_PER_SEC 0b100<<4
#define TMP_RATE_32_PER_SEC 0b101<<4
#define TMP_RATE_64_PER_SEC 0b110<<4
#define TMP_RATE_128_PER_SEC 0b111<<4

//Temperature measurement precision
#define TMP_PRC_SINGLE    0b0000 //Low precision(default)
#define TMP_PRC_2_TIMES   0b0001 //Low Power
#define TMP_PRC_4_TIMES   0b0010
#define TMP_PRC_8_TIMES   0b0011
#define TMP_PRC_16_TIMES  0b0100 //*Standard
#define TMP_PRC_32_TIMES  0b0101 //*
#define TMP_PRC_64_TIMES  0b0110 //*High precision
#define TMP_PRC_128_TIMES 0b0111 //*

#define COEF_FIRST_ADDR 0x10
#define COEF_LAST_ADDR 0x21

#define DPS310_P_SHIFT 2
#define DPS310_T_SHIFT 3

#define LSB_DPS310_0    0
#define LSB_DPS310_1    1
//typedef struct _DPS310_SensorData{
//    uint8_t pressure[3];
//    uint8_t temperature[3];
//} DPS310_SensorData;


typedef struct _DPS310_SensorData{
    int32_t pressure_raw;
    int32_t temperature_raw;
    float temperature_scaled; //divided by kT
    int32_t pressure;  //[Pa]
    int32_t temperature; //[oC]
} DPS310_SensorData;


typedef struct _DPS310_Calib_Factors{
    int32_t c0Half;
    int32_t c1;
    int32_t c00;
    int32_t c10;
    int32_t c01;
    int32_t c11;
    int32_t c20;
    int32_t c21;
    int32_t c30;
} DPS310_Calib_Factors;


//DPS310_SensorData dps310_sensordata;
extern DPS310_SensorData dps310_sensordata_0[10];
extern DPS310_SensorData dps310_sensordata_1[10];

extern DPS310_Calib_Factors dps310_calib_factors_0;
extern DPS310_Calib_Factors dps310_calib_factors_1;




//int16_t dps310_readcoeffs(void);
int16_t dps310_readcoeffs(DPS310_Calib_Factors* calib_fact);
bool ConfigDPS310(uint8_t pressure_sampling_rate, uint8_t pressure_sampling_precision,uint8_t temperature_sampling_rate, uint8_t temperature_sampling_precision);
bool dps310_i2c_synchronization(I2C_Handle i2c_handle, uint8_t Device_Addr_LSB);
//void dps310_i2c_synchronization(I2C_Handle i2c_handle);
bool initDPS310(uint8_t sampling_rate, uint8_t sampling_precision);
bool dps310_readPressure();
bool dps310_readTemperature();
void getTwosComplement(int32_t *raw, uint8_t length); //https://github.com/Infineon/DPS310-Pressure-Sensor/blob/dps310/src/DpsClass.cpp
void delay_ms(uint32_t time_ms);
//float dps310_calcTemp(DPS310_SensorData* ptr_dps310_sensordata,uint8_t temperature_oversampling_rate);
//float dps310_calcPressure(DPS310_SensorData* ptr_dps310_sensordata,uint8_t pressure_oversampling_rate);
float dps310_calcTemp(DPS310_SensorData* ptr_dps310_sensordata, DPS310_Calib_Factors* calib_fact, uint8_t temperature_oversampling_rate);
float dps310_calcPressure(DPS310_SensorData* ptr_dps310_sensordata, DPS310_Calib_Factors* calib_fact, uint8_t pressure_oversampling_rate);


//float dps310_calcTemp(int32_t raw);
//float dps310_calcPressure(int32_t raw);
#endif /* DPS310_I2C_H_ */
