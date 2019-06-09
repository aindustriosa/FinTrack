// -*- mode: C++ -*-
/*
 * FinTrackLegacyNanoCalibration for FinTrack system
 * 
 * Description:
 * Firmware para calibracion de la Baliza de telemetria para vehículos radiocontrol
 * Sobre dispositivo ArduinoMini Pro (5v16Mhz) y Hardware de comunicacion RF430 SX1278 LoRa
 * Output por puerto serie de los datos de:
 * IMU MPU6050
 * Luz: 1xLDR
 * Voltage: ADC interno
 * Amperios: ADC interno
 * Presion BMP280
 */

//configuration
#include <Arduino.h>
#include <SPI.h>
#include "Statistic.h"

#include "Hardware_cfg.h"

//el sistema de IMU :  libreria GY91_I2C
#define FSR 2000
#include <MPU9250_I2C.h>
#include <I2Cdev_GY91.h> //library from Jeff Rowberg

//BMP280
#include <Wire.h>
#include <BMP280_I2C.h>
//-----------------------------------------------------------

//Variables de la IMU:
static int status_ret;

BMP280_I2C bme;
uint8_t use_pressure;
int calibrationp = 73;   //Calibration for the pressure

//memoria de datos adquiridos:
int16_t ukn_vector[3]; //variable temporal para compartir
Statistic_int16t voltage;
Statistic_int16t current;
Statistic_int16t ldr;
Statistic_int16t pressure;

Statistic_int16t accx;
Statistic_int16t accy;
Statistic_int16t accz;

Statistic_int16t gyrx;
Statistic_int16t gyry;
Statistic_int16t gyrz;

Statistic_int16t magx;
Statistic_int16t magy;
Statistic_int16t magz;

//time and status control
uint32_t timeNow=0; // Hold the milliseond value for now
uint32_t timerStore=0;   //El anterior valor del timer 
int8_t cycleTimerCount=0;
uint8_t counter_log = 8; //cuando se activa el log (8=1 segundo)



////////////////////////////////////////////////////////////////////////////////
//MAIN SETUP
////////////////////////////////////////////////////////////////////////////////
void setup() {
    delay(200);
    status_ret = 0; //empiexzo como fail

    delay(500);
    // Start the UART for the GPS device
    Serial.begin( GPS_BAUDRATE );
    delay(50);
    Serial.flush();
  
    //MPU9250
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Fastwire::setup(400,0);
    mpu_select_device(0);
    status_ret= mpu_init(NULL);
    status_ret = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS); 
    status_ret = mpu_set_gyro_fsr(FSR);
    status_ret = mpu_set_accel_fsr(8);
    status_ret = mpu_set_sample_rate(160);
    mpu_get_power_state((unsigned char *)&status_ret);
    
    //Serial.print(F("#DEBUG,status IMU: "));
    //Serial.println(status_ret, DEC);

    use_pressure=1;
    if (!bme.begin()) {  
        use_pressure=0;
        //Serial.print(F("#DEBUG, FAIL BMP280"));
    }

    delay(100);
  
    clear_values();
    //Confgure pinout
    pinMode(LDRPIN, INPUT);
    //pinMode(A7, INPUT); //Este pin solo funciona como analoginput, si lo configuro expresamente, falla el programa :-P
    //pinMode(A6, INPUT); //Este pin solo funciona como analoginput, si lo configuro expresamente, falla el programa :-P
    ukn_vector[0] = analogRead(LDRPIN);
    ukn_vector[1] = analogRead(VOLTPIN);
    ukn_vector[2] = analogRead(AMPPIN);

    delay(500);
  
    
    timerStore=millis();
    timeNow=millis();
    cycleTimerCount = -1; // Reset case counter
    counter_log=8;
    delay(20);

}


////////////////////////////////////////////////////////////////////////////////
//MAIN LOOP
////////////////////////////////////////////////////////////////////////////////
void loop() {
    status_ret = 0; //empiexzo como fail
    timeNow = millis();
    if (timeNow<timerStore){//se produce desvordamiento de buffer del timer32 (49dias); reseteo la refernecia base
        timerStore=0;
    }

    
    
    if((timeNow-timerStore)>=20){  // Main loop runs at 50Hz -> 20 milisec
       timerStore = timeNow; //pongo la nueva referencia
       cycleTimerCount++; //incremento el contador del shedeluer 
       
       // Do these things every 6th time through the main cycle 
       // This section gets called every 1000/(20*5) = 10 Hz
       // doing it this way removes the need for another 'millis()' call
       // and balances the processing load across main loop cycles.
       switch (cycleTimerCount) {
          case(0):
             get_data_power(); //8Hz
             break;
 
          case(1):
             get_data_imu(); //8Hz
             break;
                        
          case(2):
             get_data_power(); //8Hz
             break;
                
          case(3): //8Hz
             get_data_imu();
              break;
                        
          case(4):
              //[LOG]: Write
              if (counter_log == 0){
                 print_report();
                  counter_log = 9;
                 
             }else{
                 counter_log--;    
             }

            cycleTimerCount = -1; // Reset case counter, will be incremented to zero before switch statement
            break;
        }
        
        
    }

}
    
//FUNCIOES DE DATOS:

void get_data_power(void){
  //voltage:
  ukn_vector[0]= analogRead(VOLTPIN);
  ukn_vector[1]= analogRead(VOLTPIN);
  ukn_vector[2]= analogRead(VOLTPIN);
  voltage.add(ukn_vector[2]); //A7
  
  //Amperes
  ukn_vector[0]= analogRead(AMPPIN);
  ukn_vector[1]= analogRead(AMPPIN);
  ukn_vector[2]= analogRead(AMPPIN);
  current.add(ukn_vector[2]); //A6 

  //Ligth
  ukn_vector[0]= analogRead(LDRPIN);
  ukn_vector[1]= analogRead(LDRPIN);
  ukn_vector[2]= analogRead(LDRPIN);
  ldr.add(ukn_vector[2]); //A2
    
}

////////////////////////////////////////////////////////////////////////////////
//MAIN FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void clear_values(void){
  
  //reset values:
  voltage.clear();
  current.clear();
  ldr.clear();
  pressure.clear();
  accx.clear();
  accy.clear();
  accz.clear();
  gyrx.clear();
  gyry.clear();
  gyrz.clear();
  magx.clear();
  magy.clear();
  magz.clear();
}



void get_data_imu(void){
    
  mpu_get_accel_raw(&ukn_vector[0],&ukn_vector[1],&ukn_vector[2]);
  accx.add(ukn_vector[0]); //
  accy.add(ukn_vector[1]); 
  accz.add(ukn_vector[2]);
  
  mpu_get_gyro_raw(&ukn_vector[0],&ukn_vector[1],&ukn_vector[2]);
  gyrx.add(ukn_vector[0]);
  gyry.add(ukn_vector[1]);
  gyrz.add(ukn_vector[2]);

  mpu_get_compass_raw(&ukn_vector[0],&ukn_vector[1],&ukn_vector[2]);
  magx.add(ukn_vector[0]);
  magy.add(ukn_vector[1]);
  magz.add(ukn_vector[2]);

  pressure.add(bme.readPressure()/100+calibrationp);//hPa
  
}

  
void print_report(void){
    //32bits:4294967295 (10)
    //16bits: 65535 (5)
    Serial.print("###################################################\n");
    Serial.print("ms: ");
    Serial.print(timeNow);
    Serial.print("\t\tVolt: ");
    Serial.print(voltage.average());
    Serial.print("\t\tAmp: ");
    Serial.print(current.average());
    Serial.print("\t\tLux: ");
    Serial.print(ldr.average());
    Serial.print("\t\tPres: ");
    Serial.print(pressure.average());
    
    Serial.print("\nAccX: ");
    Serial.print(accx.average());
    Serial.print("\t\t\tAccY: ");
    Serial.print(accy.average());
    Serial.print("\t\t\tAccZ: ");
    Serial.print(accz.average());
    
    Serial.print("\nGyrX: ");
    Serial.print(gyrx.average());
    Serial.print("\t\t\tGyrY: ");
    Serial.print(gyry.average());
    Serial.print("\t\t\tGyrZ: ");
    Serial.print(gyrz.average());
    
    Serial.print("\nMagX: ");
    Serial.print(magx.average());
    Serial.print("\t\tMagY: ");
    Serial.print(magy.average());
    Serial.print("\t\tMagZ: ");
    Serial.print(magz.average());
    
    
    Serial.print("\r\n");
    
    clear_values(); //limpio los buffers
    
}
