////////////////////////////////////////////////////////////////////////////
// This sketch is for test the sensor GY91                                //
//                                                                        //
// Author: Ignacio Gonzalez                                                 // 
// Created on: 17/02/2017                                                 //
// Version: 1.2                                                           //
// Notes: -Includes the default config for the GY-91 sensor.              // 
//        -Sends all data via serial                                      //
////////////////////////////////////////////////////////////////////////////


//BMP280
#include <Wire.h>
#include <BMP280_I2C.h>

//MPU9250
#define FSR 2000
#include <MPU9250_I2C.h>
#include <I2Cdev_GY91.h> //library from Jeff Rowberg
static int ret;


BMP280_I2C bme;
int calibrationp = 73;   //Calibration for the pressure
int calibrationa = 97;   //Calibration for the altitude


//Variables de la IMU:

int16_t accel[3];
int16_t gyro[3];
int16_t magnetometer[3];

//presion con barometro
int16_t pressure;


void setup() {
  Serial.begin(9600);
  
  //MPU9250
  Fastwire::setup(400,0);

  mpu_select_device(0);
  mpu_init_structures();
  ret = mpu_init(NULL);
  Serial.print("MPU init: "); Serial.println(ret);
  ret = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS); 
  Serial.print("Sensor init: "); Serial.println(ret);
  ret = mpu_set_gyro_fsr(FSR);
  ret = mpu_set_accel_fsr(2);

  mpu_get_power_state((unsigned char *)&ret);
  ret = mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  
  //  ret = mympu_open(200);
  
  delay(100);  

  //BMP280
  if (!bme.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  
  delay(100);
}


void loop() {

    //BMP280
  
    Serial.print(F("T= "));
    Serial.print(bme.readTemperature()); //Do the conversion to F if you need to
    Serial.print(" C ");
    
    Serial.print(F("\tP= "));
    Serial.print(bme.readPressure()/100+calibrationp); //Should be divided by 100 for showing hPa and not Pa
    Serial.print(" hPa ");
    
    Serial.print(F("\tA= "));
    Serial.print(bme.readAltitude(1013.25)+calibrationa); // this should be adjusted to your local forcase
    Serial.println(" m ");

    mpu_get_accel_raw(accel);
    mpu_get_gyro_raw(gyro);
    mpu_get_compass_raw(magnetometer);
    
    Serial.print("Acx: "); Serial.print(accel[0]); //Compass
      Serial.print("\tAcy: "); Serial.print(accel[1]); //X
      Serial.print("\tAcz: "); Serial.print(accel[2]); //Y
      Serial.print("\tGx: "); Serial.print(gyro[0]); //Acelerometer compass
      Serial.print("\tGy: "); Serial.print(gyro[1]); //Acelerometer X
      Serial.print("\tGz: "); Serial.print(gyro[2]); //Acelerometer Y
      Serial.print("\tMx: "); Serial.print(magnetometer[0]); //Acelerometer compass
      Serial.print("\tMy: "); Serial.print(magnetometer[1]); //Acelerometer X
      Serial.print("\tMz: "); Serial.println(magnetometer[2]); //Acelerometer Y
      delay(100); 

}
