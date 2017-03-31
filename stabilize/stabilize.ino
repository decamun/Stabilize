//motor value codes
#define ANGLE 1
#define ABS_ANGLE 11

#include <MemoryFree.h>

#define ARM_LENGTH 0.1016 //meters

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
float rollFrequencyCutoff = 0.25;
float pitchFrequencyCutoff = 0.25;
float yawFrequencyCutoff = 0.25;
float zFrequencyCutoff = 0.25;

//Motor Calibration
const float roll_zero = 3.9331;
const float pitch_zero = -1.1137;
const float yaw_zero = 5.7421;
const float z_zero = 0;


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

float alpha(long loop_time, float frequency_cutoff) {
  return 1.0/(2*PI*(loop_time*0.001)*frequency_cutoff + 1.0);
}

void setup() {
  pinMode(kLedPin, OUTPUT);
  dataLoggingSetup();
//  imuSetup();
  motControlSetup();
  motSetup(1, 10,0.01,0.01); //pitch
  motSetup(2, 100,0.01,0.05); //roll
  motSetup(3, 100,0.01,0.05); //yaw
  motSetup(4, 150,0.05,0.01); //z
  
  if(!LOOP_LOGGING) {
    setLogPermission(false);
  }
}

void loop() {
  //filter setup
//  FilterOnePole rollHighpass(HIGHPASS, rollFrequencyCutoff);
//  FilterOnePole pitchHighpass(HIGHPASS, pitchFrequencyCutoff);
//  FilterOnePole yawHighpass(HIGHPASS, yawFrequencyCutoff);
//  FilterOnePole zHighpass(HIGHPASS, zFrequencyCutoff);

  float rollHighpass = 0;
  float pitchHighpass = 0;
  float yawHighpass = 0;
  float zHighpass = 0;

  //rollover storage///////////

  //modifiers
  float roll_mod = 0.0;
  float pitch_mod = 0.0;
  float yaw_mod = 0.0;

  //previous imu values
  float roll_prev = 0.0;
  float pitch_prev = 0.0;
  float yaw_prev = 0.0;

  //rolled over values
  float roll_rollover = 0.0;
  float pitch_rollover = 0.0;
  float yaw_rollover = 0.0;

  //previous rollod over values
  float roll_prev_r = 0.0;
  float pitch_prev_r = 0.0;
  float yaw_prev_r = 0.0;

  ////////////////////////////

  //z position values
  float z_position = 0;
  float z_position_prev = 0;

  long start_time = millis();
  
  while(1) {

    //failover tasks/////

    //imu
    if(!updateImu()) {
      logln("Something went wrong with the IMU.");
      continue; //skip the cycle if the imu doesn't update correctly
    }

    //wait for highpass to steady out
    if(millis() - start_time < 2000) {
      continue;
    }

    //////////////////////
    
    //limit cycle frequency to MAX_FREQUENCY
    static long time_last = 0;
    while(millis()-time_last < MIN_LOOP_TIME);
    long loop_time = millis() - time_last;
    time_last += loop_time;
    
    //update rollover
    roll_rollover = rollover(&roll_prev, &roll_mod, imu.roll);
    pitch_rollover = rollover(&pitch_prev, &pitch_mod, imu.pitch);
    yaw_rollover = rollover(&yaw_prev, &yaw_mod, imu.yaw);

    //update high pass filters
    float rollAlpha = alpha(loop_time, rollFrequencyCutoff);
    rollHighpass = rollAlpha*rollHighpass + rollAlpha*(roll_rollover - roll_prev_r);
    roll_prev_r = roll_rollover;

    float pitchAlpha = alpha(loop_time,pitchFrequencyCutoff);
    pitchHighpass = pitchAlpha*pitchHighpass + pitchAlpha*(pitch_rollover - pitch_prev_r);
    pitch_prev_r = pitch_rollover;

    float yawAlpha = alpha(loop_time, yawFrequencyCutoff);
    yawHighpass = yawAlpha*yawHighpass + yawAlpha*(yaw_rollover - yaw_prev_r);
    yaw_prev_r = yaw_rollover;

    //update z_position and filter
    z_position_prev = z_position;
    z_position = z_position - (imu.az+1.0) * (loop_time*0.001);
    float zAlpha = alpha(loop_time, zFrequencyCutoff);
    zHighpass = zAlpha*zHighpass + zAlpha*(z_position - z_position_prev);
    
    //set motor commands
    setVal(2, ANGLE, degree2rad(rollHighpass) + roll_zero); //roll
    setVal(1, ANGLE, degree2rad(-pitchHighpass) + pitch_zero); //pitch
    setVal(3, ANGLE, degree2rad(-yawHighpass) + yaw_zero);  //yaw
    setVal(4, ANGLE, -zHighpass/ARM_LENGTH + z_zero);
    
    //sync motor info
    syncMotor(2); //roll
    syncMotor(1); //pitch
    syncMotor(3); //yaw

    syncMotor(4); //z
  
    logln("Compiling output...");
  
    static long ct = 0;
    ct++;
    
    static char string[300];
    
    //compile output
    sprintf(string, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",ct,loop_time,imu.roll,imu.pitch,imu.yaw,imu.ax,imu.ay,imu.az,getVal(1, ANGLE) - pitch_zero,getVal(2, ANGLE) - roll_zero,getVal(3, ANGLE) - yaw_zero, getVal(4, ANGLE) - z_zero);
  
    //output
    dataln(string);
    stronglogln(string);
  }
}
