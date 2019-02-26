/***************************************************************************
  This is a library for the BMP280 pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "Arduino.h"
//#include <Wire.h>
#include "I2Cdev_GY91.h"
#include "BMP280_I2C_minimal.h"



/* The following functions must be defined for this platform:
 * i2c_write(unsigned char slave_addr, unsigned char reg_addr,
 *      unsigned char length, unsigned char const *data)
 * i2c_read(unsigned char slave_addr, unsigned char reg_addr,
 *      unsigned char length, unsigned char *data)
 * delay_ms(unsigned long num_ms)
 * get_ms(unsigned long *count)
 * reg_int_cb(void (*cb)(void), unsigned char port, unsigned char pin)
 * labs(long x)
 * fabsf(float x)
 * min(int a, int b)
 */

#define i2c_write   !I2Cdev_GY91::writeBytes
#define i2c_read    !I2Cdev_GY91::readBytes
#define delay_ms    delay

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/


BMP280_I2C_minimal::BMP280_I2C_minimal()
{ }


bool BMP280_I2C_minimal::begin(void) {

  unsigned char data;
  
  if (i2c_read(BMP280_ADDRESS, BMP280_REGISTER_CHIPID, 1, &data))
            return false;
  
  if (data != BMP280_CHIPID)
    return false;

  readCoefficients();
  data = 0x3F;
  if (i2c_write(BMP280_ADDRESS, BMP280_REGISTER_CONTROL, 1, &data))
      return false;
  
  
  return true;
}



/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/

uint16_t BMP280_I2C_minimal::read16_LE(byte reg) {
  unsigned char data[2];
  uint16_t value;
  
  i2c_write(BMP280_ADDRESS, reg, 2, data);
    
  value = (data[0]<< 8) | data[1];

  return (value >> 8) | (value << 8);

}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/

int16_t BMP280_I2C_minimal::readS16_LE(byte reg)
{
  unsigned char data[2];
  uint16_t value;
  
  i2c_write(BMP280_ADDRESS, reg, 2, data);
    
  value = (data[0]<< 8) | data[1];
  
  return value;

}


/**************************************************************************/
/*!
    @brief  Reads a 24 bit value over I2C
*/
/**************************************************************************/
uint32_t BMP280_I2C_minimal::read24(byte reg)
{
    unsigned char data[3];
    uint32_t value;
  
  i2c_write(BMP280_ADDRESS, reg, 3, data);
    
  value = (data[0]<< 16) |(data[1]<< 8) | data[2];
  
  
  return value;
}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void BMP280_I2C_minimal::readCoefficients(void)
{
    _bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
    _bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
    _bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

    _bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
    _bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
    _bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
    _bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
    _bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
    _bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
    _bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
    _bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
    _bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
    
    #ifdef ENABLE_GPS
         _bmp280_calib.calibrationp = BMP280_CALIBRATRION_NP;   //Calibration for the pressure
      #else
         _bmp280_calib.calibrationp =73; //default calibrate pressure output -> milibar
      #endif 
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float BMP280_I2C_minimal::readTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = read24(BMP280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)_bmp280_calib.dig_T1 <<1))) *
	   ((int32_t)_bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1)) *
	     ((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
	   ((int32_t)_bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
int16_t BMP280_I2C_minimal::readPressure(void) {
  int64_t var1, var2, p;

  // Must be done first to get the t_fine variable set up
  readTemperature();

  int32_t adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)_bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)_bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)_bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bmp280_calib.dig_P1)>>33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)_bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7)<<4);
  float T = (float)p/256;
  
  T= T/100.0F; //pasar a milibar
  return (int16_t)T; //presion en RAW
}

