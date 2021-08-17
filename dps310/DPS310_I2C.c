/*
 * DPS310.c
 *
 *  Created on: 2020. 5. 5.
 *      Author: Joonhwa Choi, pass2774@gmail.com
 */


#include <DPS310_I2C.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>


//#include <xdc/runtime/System.h>

int32_t dps310_scaling_factors[8] = {524288, 1572864, 3670016, 7864320, 253952, 516096, 1040384, 2088960};

//DPS310_SensorData_Calib dps310_sensordata_0[10];
//DPS310_SensorData_Calib dps310_sensordata_1[10];

I2C_Transaction i2cTransaction;
I2C_Handle handle_i2c_dps310;
uint8_t i2c_txBuffer_dps310[20];
uint8_t i2c_rxBuffer_dps310[20];

DPS310_SensorData dps310_sensordata_0[10];
DPS310_SensorData dps310_sensordata_1[10];

DPS310_Calib_Factors dps310_calib_factors_0;
DPS310_Calib_Factors dps310_calib_factors_1;




void delay_ms(uint32_t time_ms){ //이건 그냥 딜레이 주는 것
    Task_sleep(time_ms*1000 / Clock_tickPeriod);
}

bool dps310_i2c_synchronization(I2C_Handle i2c_handle, uint8_t Device_Addr_LSB){ // 디바이스 찾기
    switch(Device_Addr_LSB){
        case 0:
            i2cTransaction.slaveAddress = DPS310_DEVICE_ID;
            break;
        case 1:
            i2cTransaction.slaveAddress = DPS310_DEVICE_ID|1;
            break;
        default:
            return false;
            break;

    }
    i2cTransaction.writeBuf = i2c_txBuffer_dps310;
    i2cTransaction.readBuf = i2c_rxBuffer_dps310;
    handle_i2c_dps310 = i2c_handle;
    return true;
}


bool initDPS310(uint8_t sampling_rate, uint8_t sampling_precision){  // 디바이스 시작, rate, precision정의
    i2cTransaction.readCount = 0;
    i2cTransaction.writeCount = 2;

    i2c_txBuffer_dps310[0] = DPS310_PRS_CFG;
    i2c_txBuffer_dps310[1] = sampling_rate|sampling_precision;
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }
    delay_ms(50);

    i2c_txBuffer_dps310[0] = DPS310_MEAS_CFG;
    i2c_txBuffer_dps310[1] = 0b0111;
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }

    delay_ms(50);

    //Below is recommended in the datasheet.
    if(sampling_precision>=PM_PRC_16_TIMES){
        i2c_txBuffer_dps310[0] = DPS310_CFG_REG;
        i2c_txBuffer_dps310[1] = 1<<DPS310_P_SHIFT|1<<DPS310_T_SHIFT;
    }

    delay_ms(50);
    return true;
}


bool ConfigDPS310(uint8_t pressure_sampling_rate, uint8_t pressure_sampling_precision,uint8_t temperature_sampling_rate, uint8_t temperature_sampling_precision){
    i2cTransaction.readCount = 0;
    i2cTransaction.writeCount = 2;

    //Pressure Configuration
    i2c_txBuffer_dps310[0] = DPS310_PRS_CFG;
    i2c_txBuffer_dps310[1] = pressure_sampling_rate|pressure_sampling_precision;
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//        Display_printf(display, 0, 0, "Error 1\n");
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }
    //delay_ms(50);

    //Temperature Configuration
    i2c_txBuffer_dps310[0] = DPS310_TMP_CFG;
    i2c_txBuffer_dps310[1] = temperature_sampling_rate|temperature_sampling_precision;
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//        Display_printf(display, 0, 0, "Error 2\n");
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }
    //delay_ms(50);

    //Measurement Configuration
    i2c_txBuffer_dps310[0] = DPS310_MEAS_CFG;
    i2c_txBuffer_dps310[1] = 0b0111;
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//        Display_printf(display, 0, 0, "Error 3\n");
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }
    //delay_ms(50);

    //Below is recommended in the datasheet.
    i2c_txBuffer_dps310[0] = DPS310_CFG_REG;
    i2c_txBuffer_dps310[1]=0x00;
    if(pressure_sampling_precision>=PM_PRC_16_TIMES){
        i2c_txBuffer_dps310[1] = 1<<DPS310_P_SHIFT;
    }
    if(temperature_sampling_precision>=TMP_PRC_16_TIMES){
        i2c_txBuffer_dps310[1] = i2c_txBuffer_dps310[1]|1<<DPS310_T_SHIFT;
    }
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//        Display_printf(display, 0, 0, "Error 4\n");
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }


    return true;
}


bool dps310_readPressure(DPS310_SensorData* ptr_dps310_sensordata){
    /* Point to the target register and read its 3 bytes */
    i2c_txBuffer_dps310[0] = DPS310_PSR_B2;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readCount = 3;

    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)) {
        //The sensor data is printed in 2's complement
        (*ptr_dps310_sensordata).pressure_raw = (((uint32_t)i2c_rxBuffer_dps310[0])<<16)|(((uint32_t)i2c_rxBuffer_dps310[1])<<8)|(((uint32_t)i2c_rxBuffer_dps310[2])<<0);
//        (*ptr_dps310_sensordata).pressure_raw = ((~(*ptr_dps310_sensordata).pressure_raw)&0xFFFFFF)+1;
        getTwosComplement(&(*ptr_dps310_sensordata).pressure_raw, 24);

    }
    else {
//        System_printf("I2C Bus fault\n");
//        System_flush();
        return false;
    }
    return true;
}


bool dps310_readTemperature(DPS310_SensorData* ptr_dps310_sensordata){
    /* Point to the target register and read its 3 bytes */
    i2c_txBuffer_dps310[0] = DPS310_TMP_B2;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readCount = 3;

    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)) {
        //The sensor data is printed in 2's complement
        (*ptr_dps310_sensordata).temperature_raw = (((uint32_t)i2c_rxBuffer_dps310[0])<<16)|(((uint32_t)i2c_rxBuffer_dps310[1])<<8)|(((uint32_t)i2c_rxBuffer_dps310[2])<<0);
        getTwosComplement(&(*ptr_dps310_sensordata).temperature_raw, 24);
    }
    else {
//        System_printf("I2C Bus fault\n");
//        System_flush();
        return false;
    }
    return true;
}


//int16_t dps310_readcoeffs(void){
//    // TODO: remove magic number
//    i2c_txBuffer_dps310[0] = COEF_FIRST_ADDR;
//    i2cTransaction.writeCount = 1;
//    i2cTransaction.readCount = 18;
//    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//       System_printf("I2C Bus fault\n");
//       System_flush();
//       return false;
//    }
//
//
//    //compose coefficients from buffer content
//    m_c0Half = ((uint32_t)i2c_rxBuffer_dps310[0] << 4) | (((uint32_t)i2c_rxBuffer_dps310[1] >> 4) & 0x0F);
//    getTwosComplement(&m_c0Half, 12);
//    //c0 is only used as c0*0.5, so c0_half is calculated immediately
//    m_c0Half = m_c0Half / 2U;
//
//    //now do the same thing for all other coefficients
//    m_c1 = (((uint32_t)i2c_rxBuffer_dps310[1] & 0x0F) << 8) | (uint32_t)i2c_rxBuffer_dps310[2];
//    getTwosComplement(&m_c1, 12);
//
//    m_c00 = ((uint32_t)i2c_rxBuffer_dps310[3] << 12) | ((uint32_t)i2c_rxBuffer_dps310[4] << 4) | (((uint32_t)i2c_rxBuffer_dps310[5] >> 4) & 0x0F);
//    getTwosComplement(&m_c00, 20);
//    m_c10 = (((uint32_t)i2c_rxBuffer_dps310[5] & 0x0F) << 16) | ((uint32_t)i2c_rxBuffer_dps310[6] << 8) | (uint32_t)i2c_rxBuffer_dps310[7];
//    getTwosComplement(&m_c10, 20);
//
//    m_c01 = ((uint32_t)i2c_rxBuffer_dps310[8] << 8) | (uint32_t)i2c_rxBuffer_dps310[9];
//    getTwosComplement(&m_c01, 16);
//
//    m_c11 = ((uint32_t)i2c_rxBuffer_dps310[10] << 8) | (uint32_t)i2c_rxBuffer_dps310[11];
//    getTwosComplement(&m_c11, 16);
//    m_c20 = ((uint32_t)i2c_rxBuffer_dps310[12] << 8) | (uint32_t)i2c_rxBuffer_dps310[13];
//    getTwosComplement(&m_c20, 16);
//    m_c21 = ((uint32_t)i2c_rxBuffer_dps310[14] << 8) | (uint32_t)i2c_rxBuffer_dps310[15];
//    getTwosComplement(&m_c21, 16);
//    m_c30 = ((uint32_t)i2c_rxBuffer_dps310[16] << 8) | (uint32_t)i2c_rxBuffer_dps310[17];
//    getTwosComplement(&m_c30, 16);
//    return true;
//}



int16_t dps310_readcoeffs(DPS310_Calib_Factors* calib_fact){
    // TODO: remove magic number
    i2c_txBuffer_dps310[0] = COEF_FIRST_ADDR;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readCount = 18;
    if (I2C_transfer(handle_i2c_dps310, &i2cTransaction)==false){
//       System_printf("I2C Bus fault\n");
//       System_flush();
       return false;
    }


    //compose coefficients from buffer content
    (*calib_fact).c0Half = ((uint32_t)i2c_rxBuffer_dps310[0] << 4) | (((uint32_t)i2c_rxBuffer_dps310[1] >> 4) & 0x0F);
    getTwosComplement(&(*calib_fact).c0Half, 12);
    //c0 is only used as c0*0.5, so c0_half is calculated immediately
    (*calib_fact).c0Half = (*calib_fact).c0Half / 2U;

    //now do the same thing for all other coefficients
    (*calib_fact).c1 = (((uint32_t)i2c_rxBuffer_dps310[1] & 0x0F) << 8) | (uint32_t)i2c_rxBuffer_dps310[2];
    getTwosComplement(&(*calib_fact).c1, 12);

    (*calib_fact).c00 = ((uint32_t)i2c_rxBuffer_dps310[3] << 12) | ((uint32_t)i2c_rxBuffer_dps310[4] << 4) | (((uint32_t)i2c_rxBuffer_dps310[5] >> 4) & 0x0F);
    getTwosComplement(&(*calib_fact).c00, 20);
    (*calib_fact).c10 = (((uint32_t)i2c_rxBuffer_dps310[5] & 0x0F) << 16) | ((uint32_t)i2c_rxBuffer_dps310[6] << 8) | (uint32_t)i2c_rxBuffer_dps310[7];
    getTwosComplement(&(*calib_fact).c10, 20);

    (*calib_fact).c01 = ((uint32_t)i2c_rxBuffer_dps310[8] << 8) | (uint32_t)i2c_rxBuffer_dps310[9];
    getTwosComplement(&(*calib_fact).c01, 16);

    (*calib_fact).c11 = ((uint32_t)i2c_rxBuffer_dps310[10] << 8) | (uint32_t)i2c_rxBuffer_dps310[11];
    getTwosComplement(&(*calib_fact).c11, 16);
    (*calib_fact).c20 = ((uint32_t)i2c_rxBuffer_dps310[12] << 8) | (uint32_t)i2c_rxBuffer_dps310[13];
    getTwosComplement(&(*calib_fact).c20, 16);
    (*calib_fact).c21 = ((uint32_t)i2c_rxBuffer_dps310[14] << 8) | (uint32_t)i2c_rxBuffer_dps310[15];
    getTwosComplement(&(*calib_fact).c21, 16);
    (*calib_fact).c30 = ((uint32_t)i2c_rxBuffer_dps310[16] << 8) | (uint32_t)i2c_rxBuffer_dps310[17];
    getTwosComplement(&(*calib_fact).c30, 16);
    return true;
}





void getTwosComplement(int32_t *raw, uint8_t length){
    if (*raw & ((uint32_t)1 << (length - 1))){
        *raw -= ((uint32_t)1 << length);
    }
}


float dps310_calcTemp(DPS310_SensorData* ptr_dps310_sensordata, DPS310_Calib_Factors* calib_fact, uint8_t temperature_oversampling_rate){
    float temp = (*ptr_dps310_sensordata).temperature_raw;

    //scale temperature according to scaling table and oversampling
    temp /= dps310_scaling_factors[temperature_oversampling_rate];

    //update last measured temperature
    //it will be used for pressure compensation
    (*ptr_dps310_sensordata).temperature_scaled=temp;

    //Calculate compensated temperature
    temp = (*calib_fact).c0Half + (*calib_fact).c1 * temp;
//    temp = m_c1* temp;

    (*ptr_dps310_sensordata).temperature=(int32_t)(temp*1000);
    return temp;
}

float dps310_calcPressure(DPS310_SensorData* ptr_dps310_sensordata, DPS310_Calib_Factors* calib_fact, uint8_t pressure_oversampling_rate){
    float prs = (float)((*ptr_dps310_sensordata).pressure_raw);

    //scale pressure according to scaling table and oversampling
    prs /= dps310_scaling_factors[pressure_oversampling_rate];

    //Calculate compensated pressure
    prs = (*calib_fact).c00 + prs * ((*calib_fact).c10 + prs * ((*calib_fact).c20 + prs * (*calib_fact).c30)) + (*ptr_dps310_sensordata).temperature_scaled * ((*calib_fact).c01 + prs * ((*calib_fact).c11 + prs * (*calib_fact).c21));

    (*ptr_dps310_sensordata).pressure=(int32_t)(prs*1000);
    //return pressure
    return prs;
}


//float dps310_calcTemp(DPS310_SensorData* ptr_dps310_sensordata,uint8_t temperature_oversampling_rate){
////    float temp = (float)((*ptr_dps310_sensordata).temperature_raw);
//    float temp = (*ptr_dps310_sensordata).temperature_raw;
//
//    //scale temperature according to scaling table and oversampling
//    temp /= dps310_scaling_factors[temperature_oversampling_rate];
//
//    //update last measured temperature
//    //it will be used for pressure compensation
//    (*ptr_dps310_sensordata).temperature_scaled=temp;
//
//    //Calculate compensated temperature
//    temp = m_c0Half + m_c1 * temp;
////    temp = m_c1* temp;
//
//    (*ptr_dps310_sensordata).temperature=(int32_t)(temp*1000);
//    return temp;
//}
//
//float dps310_calcPressure(DPS310_SensorData* ptr_dps310_sensordata,uint8_t pressure_oversampling_rate){
//    float prs = (float)((*ptr_dps310_sensordata).pressure_raw);
//
//    //scale pressure according to scaling table and oversampling
//    prs /= dps310_scaling_factors[pressure_oversampling_rate];
//
//    //Calculate compensated pressure
//    prs = m_c00 + prs * (m_c10 + prs * (m_c20 + prs * m_c30)) + (*ptr_dps310_sensordata).temperature_scaled * (m_c01 + prs * (m_c11 + prs * m_c21));
//
//    (*ptr_dps310_sensordata).pressure=(int32_t)(prs*1000);
//    //return pressure
//    return prs;
//}
//
//
