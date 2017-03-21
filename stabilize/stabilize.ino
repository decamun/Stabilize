//motor value codes
#define ANGLE 1
#define ABS_ANGLE 11

#include <MemoryFree.h>

#define LOGGING 0
//0: no logging
//1: serial logging,
//2: SD card logging

#define LOOP_LOGGING 1
//1: enable logging in loop
//0: disable logging in loop

#define MAX_FREQUENCY 50
const long MIN_LOOP_TIME = 1000/MAX_FREQUENCY;

//led output pin
const int kLedPin =  13;

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
bool updateImu();
//get pitch angle
double pitch(int IMU);
///////////////////////////////////////////////
//MOT//////////////////////////////////////////
//Call this first
void motControlSetup();
//Call this for each motor
void motSetup(int motNum, float Kp, float Ki, float Kd);
//Set a value
void setVal(int motNum, int val, float pos);
//Get a value
float getVal(int motNum, int val);
//Call this every loop
void syncMotor(int motNum);
///////////////////////////////////////////////
///
//

void setup() {
  pinMode(kLedPin, OUTPUT);
  dataLoggingSetup();
  imuSetup();
  motControlSetup();
  motSetup(1, 10,0,0);
  motSetup(2, 10,0,0);
  motSetup(3, 10,0,0);
  
  if(!LOOP_LOGGING) {
    setLogPermission(false);
  }
}

void loop() {
  static long time_last = 0;

  //limit cycle frequency to MAX_FREQUENCY
  while(millis()-time_last < MIN_LOOP_TIME);
  long loop_time = millis() - time_last;
  time_last += loop_time;
  
  syncMotor(1);
  syncMotor(2);
  syncMotor(3);

  setVal(1, ANGLE, 0);
  setVal(2, ANGLE, 0);
  setVal(3, ANGLE, 0);
  
  

  //imu
  if(!updateImu()) {
    stronglogln("Something went wrong with the IMU.");
    return; //skip the cycle if the imu doesn't update correctly
  }

  logln("Compiling output...");

  static long ct = 0;
  String string = String(ct) + String(",") + String(loop_time);
  ct++;
  string += String(",") + String(imu.roll);
  string += String(",") + String(imu.pitch);
  string += String(",") + String(imu.yaw);
  string += String(",") + String(imu.ax);
  string += String(",") + String(imu.ay);
  string += String(",") + String(imu.az);
  string += String(",") + String(getVal(1, ANGLE));
  string += String(",") + String(getVal(2, ANGLE));
  string += String(",") + String(getVal(3, ANGLE));

  string += String(",") + String(freeMemory());
  string += String(",") + String(freeMemory());

  //output
  dataln(string);
  logln(String("Data Output: ")+string);
}
