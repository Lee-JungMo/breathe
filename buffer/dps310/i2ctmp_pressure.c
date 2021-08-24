/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== i2ctmp116.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "Board.h"


/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>



//from DPS310_I2C.c
#include <DPS310_I2C.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

/* Example/Board Header files */



#define TASKSTACKSIZE       1024

static Display_Handle display;

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    uint16_t        sample;
    uint8_t i2c_txBuffer_dps310[20];
    uint8_t i2c_rxBuffer_dps310[20];
    uint8_t Device_Addr_LSB;

    float prs;

    I2C_Handle handle_i2c_dps310;
    I2C_Transaction i2cTransaction;
    I2C_Params      i2cParams;

    /* Call driver init functions */
    Display_init();
    GPIO_init();
    I2C_init();


    //from DPS310_I2C.c
    int32_t dps310_scaling_factors[8] = {524288, 1572864, 3670016, 7864320, 253952, 516096, 1040384, 2088960};

    DPS310_SensorData dps310_sensordata_0[10];
    DPS310_SensorData dps310_sensordata_1[10];

    DPS310_Calib_Factors dps310_calib_factors_0;
    DPS310_Calib_Factors dps310_calib_factors_1;






////////////////////////////////////////////////////////////////////////////

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    Display_printf(display, 0, 0, "Starting the i2ctmp example.");
///////////////////////////////////////////////////////////////////////////////

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams); // param init !!
    i2cParams.bitRate = I2C_400kHz; // 보드레이트 설정
    handle_i2c_dps310 = I2C_open(Board_I2C0, &i2cParams);  // 핸들 오픈

    if (handle_i2c_dps310 == NULL) {     // 핸들 못찾으면 에러
        Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1);
    }
    else {
        Display_printf(display, 0, 0, "I2C Initialized!\n");
    }



    //Atmospheric Pressure sensor initialization
    dps310_i2c_synchronization(handle_i2c_dps310, LSB_DPS310_1); // You can use 'bool' like this. You can do device select(LSB select), writebuf, readbuf, handle set from here.

    //initDPS310(PM_RATE_32_PER_SEC, PM_PRC_16_TIMES); //??

    //bool b_DPS310_ready = ConfigDPS310(PM_RATE_32_PER_SEC, PM_PRC_16_TIMES, TMP_RATE_32_PER_SEC, TMP_PRC_SINGLE); // Pressure, Temperature, Measurement configuration + recommended step
    ConfigDPS310(PM_RATE_32_PER_SEC, PM_PRC_16_TIMES, TMP_RATE_32_PER_SEC, TMP_PRC_SINGLE);
    Task_sleep(50 * 1000 / Clock_tickPeriod);

    if (ConfigDPS310(PM_RATE_32_PER_SEC, PM_PRC_16_TIMES, TMP_RATE_32_PER_SEC, TMP_PRC_SINGLE)) { // If Config ready
        dps310_readcoeffs(&dps310_calib_factors_0);
        Display_printf(display, 0, 0, "1");
    }
    else {
        Display_printf(display, 0, 0, "2");
    }
    //    Task_sleep(500*1000/Clock_tickPeriod);

    int i;

    while (true) {

        if (ConfigDPS310(PM_RATE_32_PER_SEC, PM_PRC_16_TIMES, TMP_RATE_32_PER_SEC, TMP_PRC_SINGLE)) {
            dps310_i2c_synchronization(handle_i2c_dps310, LSB_DPS310_1);
            dps310_readTemperature(&dps310_sensordata_0[0]);
            dps310_readPressure(&dps310_sensordata_0[0]);
            dps310_calcTemp(&dps310_sensordata_0[0], &dps310_calib_factors_0, TMP_PRC_SINGLE);
            dps310_calcPressure(&dps310_sensordata_0[0], &dps310_calib_factors_0, PM_PRC_16_TIMES);

            //Display_printf(display, 0, 0, "TMP : %f, P : %f", dps310_calcTemp(&dps310_sensordata_0[0], &dps310_calib_factors_0, TMP_PRC_SINGLE), dps310_calcPressure(&dps310_sensordata_0[0], &dps310_calib_factors_0, PM_PRC_16_TIMES));
            Display_printf(display, 0, 0, "%f", dps310_calcPressure(&dps310_sensordata_0[0], &dps310_calib_factors_0, PM_PRC_16_TIMES));
            //idx = 8 * i + 8;
            //SensorMsg.state[idx] = ((dps310_sensordata_0[0].temperature) >> 24) & 0xFF;
        }
    }




    I2C_close(handle_i2c_dps310);
    Display_printf(display, 0, 0, "I2C closed!");

    return (NULL);
}
