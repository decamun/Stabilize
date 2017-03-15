

#define LOGGING 1
//0: no logging
//1: serial logging,
//2: SD card logging

#define LOOP_LOGGING 1
//1: enable logging in loop
//0: disable logging in loop



//
///
///////////////////////////////////////////////
//LOGGING//////////////////////////////////////
//use this function to log a runtime message
void logln(String msg);
//use this function to log a runtime message even if logging is disabled
void stronglogln(String msg);
//Setup for data logging to SD card
void dataLoggingSetup();
//use this function to log data to the SD card
void dataln(String dat);
//use this to set message logging permission
void setLogPermission(bool loggingP);
///////////////////////////////////////////////
//IMU//////////////////////////////////////////
#include <UM7.h>
UM7 imu;
//IMU setup
void imuSetup();
//IMU update
void updateImu();
//get pitch angle
double pitch(int IMU);
///////////////////////////////////////////////
///
//

void setup() {
  dataLoggingSetup();
  imuSetup();
  
  if(!LOOP_LOGGING) {
    setLogPermission(false);
  }
}

void loop() {
  
  static unsigned long t1 = millis();
  static unsigned long t2 = millis();
  static String string;

  string = "";

  //timing
  t2 = millis();
  string += String(t2 - t1);
  t1 = t2;

  //imu
  updateImu();
  string += String(",") + String(imu.pitch);

  //output
  dataln(string);
  logln(string);
}
