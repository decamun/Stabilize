//motor value codes
#define ANGLE 1
#define ABS_ANGLE 11

#include <MemoryFree.h>
#include <Filters.h>

#define LOGGING 1
//0: no logging
//1: serial logging,
//2: SD card logging

#define LOOP_LOGGING 0
//1: enable logging in loop
//0: disable logging in loop

#define RUN_MOTORS 1

#define MAX_FREQUENCY 50
const long MIN_LOOP_TIME = 1000/MAX_FREQUENCY;


//Washout Settings
float rollFrequencyCutoff = 0.5;
float pitchFrequencyCutoff = 0.5;
float yawFrequencyCutoff = 0.5;


//led output pin
const int kLedPin =  13;

//
///
///////////////////////////////////////////////
//LOGGING//////////////////////////////////////
//use this function to log a runtime message
void logln(char* msg);
//use this function to log a runtime message even if logging is disabled
void stronglogln(char* msg);
//Setup for data logging to SD card
void dataLoggingSetup();
//use this function to log data to the SD card
void dataln(char* dat);
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

float degree2rad(float degree) {
  return degree*2.0*PI/360.0;
}

float rad2degree(float rad) {
  return rad * 360.0/(2.0*PI/360);
}

//float yawRollover(float yaw) {
//  static float prev = 0;
//  static float mod = 0;
//  if(abs() > ) 
//}

void setup() {
  pinMode(kLedPin, OUTPUT);
  dataLoggingSetup();
  imuSetup();
  motControlSetup();
  motSetup(1, 100,0,0);
  motSetup(2, 100,0,0);
  motSetup(3, 100,0,0);
  
  if(!LOOP_LOGGING) {
    setLogPermission(false);
  }
}

void loop() {
  FilterOnePole rollHighpass(HIGHPASS, rollFrequencyCutoff);
  FilterOnePole pitchHighpass(HIGHPASS, pitchFrequencyCutoff);
  FilterOnePole yawHighpass(HIGHPASS, yawFrequencyCutoff);
  while(1) {
    //failover tasks
    
    //imu
    if(!updateImu()) {
      logln("Something went wrong with the IMU.");
      return; //skip the cycle if the imu doesn't update correctly
    }
    
    static long time_last = 0;
  
    //limit cycle frequency to MAX_FREQUENCY
    while(millis()-time_last < MIN_LOOP_TIME);
    long loop_time = millis() - time_last;
    time_last += loop_time;
  
    //update washout filters
    rollHighpass.input(imu.roll);
    pitchHighpass.input(imu.pitch);
    yawHighpass.input(imu.yaw);
    
    if(RUN_MOTORS) {
      //sync and update motor commands
      syncMotor(2); //roll
      syncMotor(1); //pitch
      syncMotor(3); //yaw
      
      setVal(2, ANGLE, degree2rad(imu.roll)+3.14); //roll
      setVal(1, ANGLE, degree2rad(imu.pitch)+1.5); //pitch
      setVal(3, ANGLE, degree2rad(-imu.yaw) + 1.5); //yaw
    }
  
    logln("Compiling output...");
  
    static long ct = 0;
    ct++;
    
    static char string[300];
    //compile output
    sprintf(string, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f",ct,loop_time,imu.roll,imu.pitch,imu.yaw,imu.ax,imu.ay,imu.az,getVal(1, ANGLE),getVal(2, ANGLE),getVal(3, ANGLE));
  
    //output
    dataln(string);
    stronglogln(string);
  }
}
