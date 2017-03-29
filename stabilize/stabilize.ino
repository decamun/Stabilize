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

float rollover(float* prev, float* mod, float value) {
  float margin = 50;

  //rollover 0
  if(*prev - margin < 0 && value + margin > 360.0){
    *mod = *mod - 360.0;
  }

  //rollover 360
  if(*prev + margin > 360.0 && value - margin < 0.0) {
    *mod = *mod + 360.0;
  }
  *prev = value;
  return value + *mod;
}

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
  //filter setup
  FilterOnePole rollHighpass(HIGHPASS, rollFrequencyCutoff);
  FilterOnePole pitchHighpass(HIGHPASS, pitchFrequencyCutoff);
  FilterOnePole yawHighpass(HIGHPASS, yawFrequencyCutoff);

  //rollover storage
  float roll_mod = 0.0;
  float pitch_mod = 0.0;
  float yaw_mod = 0.0;
  
  float roll_prev = 0.0;
  float pitch_prev = 0.0;
  float yaw_prev = 0.0;

  float roll_rollover = 0.0;
  float pitch_rollover = 0.0;
  float yaw_rollover = 0.0;
  
  while(1) {
    
    //imu or failover
    if(!updateImu()) {
      logln("Something went wrong with the IMU.");
      return; //skip the cycle if the imu doesn't update correctly
    }
    
    //limit cycle frequency to MAX_FREQUENCY
    static long time_last = 0;
    while(millis()-time_last < MIN_LOOP_TIME);
    long loop_time = millis() - time_last;
    time_last += loop_time;
    
    //update rollover
    roll_rollover = rollover(&roll_prev, &roll_mod, imu.roll);
    pitch_rollover = rollover(&pitch_prev, &pitch_mod, imu.pitch);
    yaw_rollover = rollover(&yaw_prev, &yaw_mod, imu.yaw);
    
    //update washout filters
    rollHighpass.input(roll_rollover);
    pitchHighpass.input(pitch_rollover);
    yawHighpass.input(yaw_rollover);
    
    //set motor commands
    setVal(2, ANGLE, degree2rad(rollHighpass.output()) + 3.14); //roll
    setVal(1, ANGLE, degree2rad(pitchHighpass.output()) + 1.5); //pitch
    setVal(3, ANGLE, degree2rad(-yawHighpass.output()) + 1.5);  //yaw
    
    //sync motor info
    syncMotor(2); //roll
    syncMotor(1); //pitch
    syncMotor(3); //yaw
  
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
