/*
 * Sensors.c
 *
 * Created: 2017-07-22
 * Author : Craig Hollinger
 *
 * This is an example program that uses the I2C bus to read data from sensors
 * and display it on an OLED display.  The sensors are an accelerometer,
 * a gyroscope and a magnetometer.  Each sensor has three axes of data and 
 * communicates on the I2C bus.  The OLED display also communicates on the I2C
 * bus.
 *
 * The I2C SCL line is on Arduino pin A5 and the SDA line is A4.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "i2c/i2c.h"
#include "adxl345/adxl345.h"
#include "hmc5883/hmc5883.h"
#include "itg3205/itg3205.h"
#include "graphics/graphics.h"
#include "ssd1306/ssd1306_i2c.h"

/* These define the string formatting for the itoa() function. */
#define HEX_FORMAT 16
#define DEC_FORMAT 10
#define OCT_FORMAT 8

int main(void)
{
/* temporary storage for raw data read from a sensor, two bytes per axis  */
  uint8_t sensorBuf[6];
  
/* temporary storage for a formatted string from itoa() */
  char tempStr[8];

/* temporary storage for 16-bit raw sensor data */
  int16_t sensorXData, sensorYData, sensorZData;

/* initialize and start up the I2C system */
  i2c_init(400000UL);

/* Initialize and power up the accelerometer.
   
    power it up
    set data format to full resolution and +/-16g
    set data rate to 12.5Hz
 */
  adxl345_setDataFormat(ADXL_DATA_FORMAT_FULL_RES | ADXL_DATA_FORMAT_RANGE_02);
  adxl345_setBWRate(ADXL_BW_RATE_0012);
  adxl345_setPowerControl(ADXL_POWER_CTL_MEASURE);

/* Initialize the gyroscope.
 */
  itg3205_setPowerMgmt(ITG3205_PWR_MGMT_RESET|ITG3205_PWR_MGMT_PLLZ);
  itg3205_setSampleRate(ITG3205_FS_SEL|ITG3205_DLPF_20HZ);

/* Initialize and power up the magnetometer.
   
    average = 1, data rate = 15Hz
    gain = 1.3 Ga
    speed = normal, mode = continuous
 */
  hmc5883_init(HMC5883_AVRG_1|HMC5883_DORT_1500|HMC5883_MESC_NORM,
               HMC5883_GAIN_092,
               HMC5883_MODE_NS|HMC5883_MODE_CONT);

/* Initialize the display.
   Setup the display in graphics mode.
 */
  ssd1306_i2c_init();
  ssd1306_i2c_flip_vertical();
  graphics_init(SSD1306_GRAPHICS_MAX_X, SSD1306_GRAPHICS_MAX_Y);
  graphics_set_text_size(2);
  graphics_set_rotation(GRAPHICS_ROTATION_0);

/* Draw the labels on the screen. */
  graphics_draw_filled_rectangle(0, 0, 127, 20);
  graphics_set_cursor(21, 2);
  graphics_putStr("SENSORS");
  graphics_set_text_size(1);
  graphics_set_cursor(0, 27);
  graphics_putStr("Acc:");
  graphics_set_cursor(0, 37);
  graphics_putStr("Gyr:");
  graphics_set_cursor(0, 47);
  graphics_putStr("Mag:");
  ssd1306_i2c_graphics_update();

/* Repeatedly read the sensors and send the data to the display. */
  while(1) 
  {
  /* read the accelerometer, format and display the data */
    adxl345_getAccelData(sensorBuf);
    sensorXData = (int16_t)sensorBuf[1];
    sensorXData += (int16_t)sensorBuf[0] << 8;
    sensorYData = (int16_t)sensorBuf[3];
    sensorYData += (int16_t)sensorBuf[2] << 8;
    sensorZData = (int16_t)sensorBuf[5];
    sensorZData += (int16_t)sensorBuf[4] << 8;

    graphics_set_cursor(28, 27);
    itoa(sensorXData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

    graphics_set_cursor(64, 27);
    itoa(sensorYData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

    graphics_set_cursor(100, 27);
    itoa(sensorZData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

  /* read the gyroscope, format and display the data */
    itg3205_getGyroData(sensorBuf);
    sensorXData = (int16_t)sensorBuf[1];
    sensorXData += (int16_t)sensorBuf[0] << 8;
    sensorYData = (int16_t)sensorBuf[3];
    sensorYData += (int16_t)sensorBuf[2] << 8;
    sensorZData = (int16_t)sensorBuf[5];
    sensorZData += (int16_t)sensorBuf[4] << 8;

    graphics_set_cursor(28, 37);
    itoa(sensorXData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

    graphics_set_cursor(64, 37);
    itoa(sensorYData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

    graphics_set_cursor(100, 37);
    itoa(sensorZData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

  /* read the magnetometer, format and display the data */
    hmc5883_getMagData(sensorBuf);
    sensorXData = (int16_t)sensorBuf[1];
    sensorXData += (int16_t)sensorBuf[0] << 8;
    sensorYData = (int16_t)sensorBuf[3];
    sensorYData += (int16_t)sensorBuf[2] << 8;
    sensorZData = (int16_t)sensorBuf[5];
    sensorZData += (int16_t)sensorBuf[4] << 8;

    graphics_set_cursor(28, 47);
    itoa(sensorXData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

    graphics_set_cursor(64, 47);
    itoa(sensorYData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

    graphics_set_cursor(100, 47);
    itoa(sensorZData, tempStr, HEX_FORMAT);
    graphics_putStr(tempStr);

  /* everything is written to the graphics RAM, now send it to the display */
    ssd1306_i2c_graphics_update();

  }/* end while(1) */

}/* end main() */

