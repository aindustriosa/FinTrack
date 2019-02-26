// -*- mode: C++ -*-
/*
 * FinTrackLegacyNano for FinTrack system
 * 
 * Description:
 * Baliza de telemetria para vehículos radiocontrol
 * Sobre dispositivo ArduinoMini Pro (5v16Mhz) y Hardware de comunicacion RF430 SX1278 LoRa
 * GPS NMEA
 * IMU MPU6050
 * temperatura: 1x ds18b20
 * Luz: 1xLDR
 * Voltage: ADC interno
 * Amperios: ADC interno
 * Log: SD Card guardando datos cada segundo
 */

//configuration
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Statistic.h"

#include "Hardware_cfg.h"

//para  el GPS libreria NEOGPS
#include <NMEAGPS.h>

//el sistema de IMU :  libreria GY91_I2C
#define FSR 2000
#include <MPU9250_I2C.h>
#include <I2Cdev_GY91.h> //library from Jeff Rowberg

//BMP280
#include <Wire.h>
#include <BMP280_I2C.h>
//-----------------------------------------------------------

// This object parses received characters
//   into the gps.fix() data structure
//RMC,GGA,GSA,GSV,GST
//[RAM: 84]
static NMEAGPS  gps;
static gps_fix fixGPS; // This holds the latest GPS fix

//elficherode datalog:
SdFat SD;
SdFile dataFile;
uint8_t use_log;
char filename[15] = "data_##.csv";


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


/// NMEA TelemetryGPS
struct telemetrygps_t {
    
    int32_t  latitudeL;
    int32_t  longitudeL;
    int32_t  altitudeL;
    uint16_t lat_err_cm;
    uint16_t lon_err_cm;
    uint16_t alt_err_cm; 
    
    int8_t year;
    int8_t month;
    int8_t day;
    int8_t hour;
    int8_t minute;
    int8_t second;
    
    void init() {
        latitudeL = -500;
        longitudeL = -500;
        altitudeL=-500;

        lat_err_cm=32000;
        lon_err_cm=32000;
        alt_err_cm=32000;
        
        year = -1;
        month= -1;
        day= -1;
        hour= -1;
        minute= -1;
        second= -1;
    }
};
telemetrygps_t gps_now ; //los datos adquiridos en la trasmision



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
  
  // see if the card is present and can be initialized:
    
    use_log=1;
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!SD.begin(SDCARD_CS, SD_SCK_MHZ(50))) {
        use_log=0;
        //Serial.print(F("#DEBUG, FAIL SDCARD init"));
    } else{
        for (uint8_t i=0; i<100; i++) {
            filename[5] = '0' + i/10;
            filename[6] = '0' + i%10;
            if (!SD.exists(filename)) {
                // Use this one!
                //Serial.print(filename);
                break;
            }
        }
        //Open: 
        dataFile.open(filename, O_WRONLY | O_CREAT | O_EXCL); //voy prepardo el ficherode log
        write_data_header();
        
    }
    
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

    
    //compruebo que hay datos GPS nuevos y lo meto en le fix
    get_data_gps();
    
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
              //[LOG]: Prepare
              if (counter_log == 1){
                 if (use_log){
                     get_data_report();
                 }
              }
              //[LOG]: Write
              if (counter_log == 0){
                  if (use_log){
                      dataFile.sync();
                      //Serial.println(sd_buffer);
                  }
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
void get_data_gps(void){

  if (gps.available( Serial )){
    fixGPS = gps.read();
    
    if (fixGPS.valid.location) {
        gps_now.latitudeL=fixGPS.latitudeL();
        gps_now.longitudeL=fixGPS.longitudeL();
    }

    if (fixGPS.valid.altitude){
        gps_now.altitudeL=fixGPS.altitude_cm();
    }
    
    if (fixGPS.valid.lon_err){
        gps_now.lat_err_cm = fixGPS.lat_err_cm;
        gps_now.lon_err_cm = fixGPS.lon_err_cm;
        gps_now.alt_err_cm = fixGPS.alt_err_cm;
    }
        
    if(fixGPS.valid.time){
        gps_now.year=fixGPS.dateTime.year;
        gps_now.month=fixGPS.dateTime.month;
        gps_now.day=fixGPS.dateTime.date;
        gps_now.hour=fixGPS.dateTime.hours;
        gps_now.minute=fixGPS.dateTime.minutes;
        gps_now.second=fixGPS.dateTime.seconds;
    }
  }
    
}

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
  
  gps_now.init();
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

void write_data_header(void){
  dataFile.print(F("ID,timestamp_ms,year,month,day,hour,min,sec,lat_E7,lon_E7,alt_cm,lat_err_cm,lon_err_cm,alt_err_cm,volt_avg,volt_std,amp_avg,amp_std,ldr_avg,ldr_std,pres_avg_hpa,pres_std,"));
  dataFile.print(F("acx_avg,acx_std,acy_avg,acy_std,acz_avg,acz_std,gyx_avg,gyx_std,gyy_avg,gyy_std,gyz_avg,gyz_std,mgx_avg,mgx_std,mgy_avg,mgy_std,mgz_avg,mgz_std"));
  dataFile.write('\r\n');
  dataFile.sync();
  
  }
  
void get_data_report(void){
    //32bits:4294967295 (10)
    //16bits: 65535 (5)
    dataFile.print("FTLN,");
    dataFile.print(timeNow);
    dataFile.write(',');
    dataFile.print("20");
    dataFile.print(gps_now.year);
    dataFile.write(',');
    dataFile.print(gps_now.month);
    dataFile.write(',');
    dataFile.print(gps_now.day);
    dataFile.write(',');
    dataFile.print(gps_now.hour);
    dataFile.write(',');
    dataFile.print(gps_now.minute);
    dataFile.write(',');
    dataFile.print(gps_now.second);
    dataFile.write(',');
    
    dataFile.print(gps_now.latitudeL);
    dataFile.write(',');
    dataFile.print(gps_now.longitudeL);
    dataFile.write(',');
    dataFile.print(gps_now.altitudeL);
    dataFile.write(',');
    dataFile.print(gps_now.lat_err_cm);
    dataFile.write(',');
    dataFile.print(gps_now.lon_err_cm);
    dataFile.write(',');
    dataFile.print(gps_now.alt_err_cm);
    dataFile.write(',');
    
    dataFile.print(voltage.average());
    dataFile.write(',');
    dataFile.print(voltage.pop_stdev());
    dataFile.write(',');
    dataFile.print(current.average());
    dataFile.write(',');
    dataFile.print(current.pop_stdev());
    dataFile.write(',');
    dataFile.print(ldr.average());
    dataFile.write(',');
    dataFile.print(ldr.pop_stdev());
    dataFile.write(',');
    dataFile.print(pressure.average());
    dataFile.write(',');
    dataFile.print(pressure.pop_stdev());
    dataFile.write(',');
    
    dataFile.print(accx.average());
    dataFile.write(',');
    dataFile.print(accx.pop_stdev());
    dataFile.write(',');
    dataFile.print(accy.average());
    dataFile.write(',');
    dataFile.print(accy.pop_stdev());
    dataFile.write(',');
    dataFile.print(accz.average());
    dataFile.write(',');
    dataFile.print(accz.pop_stdev());
    dataFile.write(',');
    
    dataFile.print(gyrx.average());
    dataFile.write(',');
    dataFile.print(gyrx.pop_stdev());
    dataFile.write(',');
    dataFile.print(gyry.average());
    dataFile.write(',');
    dataFile.print(gyry.pop_stdev());
    dataFile.write(',');
    dataFile.print(gyrz.average());
    dataFile.write(',');
    dataFile.print(gyrz.pop_stdev());
    dataFile.write(',');
    
    dataFile.print(magx.average());
    dataFile.write(',');
    dataFile.print(magx.pop_stdev());
    dataFile.write(',');
    dataFile.print(magy.average());
    dataFile.write(',');
    dataFile.print(magy.pop_stdev());
    dataFile.write(',');
    dataFile.print(magz.average());
    dataFile.write(',');
    dataFile.print(magz.pop_stdev());
    dataFile.write('\r\n');
    
    clear_values(); //limpio los buffers
    
}
