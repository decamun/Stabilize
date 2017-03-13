#include <bipbuffer.h>
#include <byte_queue.h>
#include <communication_interface.h>
#include <crc_helper.h>
#include <packet_finder.h>

#include <i2c_t3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

#define ALPHA 0.993 //ammount to trust gyro data
#define LPF 0.05 //low pass output
#define CALIBRATION_ALPHA 0.05 //calibration LPF

/* 
 *  IQinetics serial communication example.
 *  
 *  Turns off the LED when the motor's position is under pi.
 *  Turns on the LED when the motor's position is over pi.
 *  
 *  The circuit:
 *    LED attached from pin 13 to ground
 *    Arduino RX is directly connected to motor TX
 *    Arduino TX is directly connected to motor RX
 *    
 *  Created 2016/12/28 by Matthew Piccoli
 *  
 *  This example code is in the public domain.
 */

// Includes required for communication
// Message forming interface
#include <generic_interface.hpp> 
// Client that speaks to complex motor controllers
#include <complex_motor_control_client.hpp>

// LED pin
const int kLedPin =  13;

// This buffer is for passing around messages.
// We use one buffer here to save space.
uint8_t communication_buffer_1[256];
// Stores length of message to send or receive
uint8_t communication_length_1;
// Time in milliseconds since we received a packet
unsigned long communication_time_last;

// Make a communication interface object
GenericInterface com1;
// Make a complex motor control object
ComplexMotorControlClient mot_client1(0);


/* Assign a unique ID to the sensors */
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(30303);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

float gyro_calibration_factor_x = 0.0;
float gyro_calibration_factor_y = 0.0;


void displaySensorDetails(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void calibrateGyro(void)
{
   
  Serial.println("Calibrating Gyro...");
  for(int i = 0; i < 100; i++) 
  { 
    sensors_event_t gyro_event;
    gyro.getEvent(&gyro_event);
    gyro_calibration_factor_x = gyro_calibration_factor_x * (1-CALIBRATION_ALPHA) + gyro_event.gyro.x * CALIBRATION_ALPHA;
    gyro_calibration_factor_y = gyro_calibration_factor_y * (1-CALIBRATION_ALPHA) + gyro_event.gyro.y * CALIBRATION_ALPHA;
    delay(10);
  }
  Serial.println("Gyro Calibration Factor X: " + String(gyro_calibration_factor_x));
}

/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{

  gyro.enableAutoRange(true);
  
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 (accel) detected ... Check your wiring!"));
    while(1);
  }
    
  if(!gyro.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no L3GD20 (gyro) detected ... Check your wiring!"));
    while(1);
  }

  Serial.println("Initialized the Sensors");

  displaySensorDetails();

  calibrateGyro();

  
}

void motSet() {
  
  float AngleKp = 0;
  float AngleKd = 0;
  float AngleKi = 0;
  mot_client1.AngleKp.Set(com1,AngleKp);
  mot_client1.AngleKd.Set(com1,AngleKd);
  mot_client1.AngleKi.Set(com1,AngleKi);
  Serial.println("Set up the motors");
}

void setup() {
  Serial.begin(115200);
  while(!Serial.available());
  Serial.println("Initializing...");
  // Initialize the Serial peripheral
  Serial1.begin(115200);
  // Initialize the LED pin as an output:
  pinMode(kLedPin, OUTPUT);

  // Initialize communication time
  communication_time_last = millis();

  motSet();


  /* Initialise the sensors */
  initSensors();
}



void loop() {
  //////////////////////
  //Sensor Code/////////
  //////////////////////
  sensors_event_t accel_event;
  sensors_event_t gyro_event;
  sensors_vec_t   orientation;
  
  //Serial.println("starting loop");

  static double pitch_angle = 0.00;
  static double roll_angle = 0.00;
  static long start_time = millis();
  static long end_time = 0;

  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  float accel_y = accel_event.acceleration.y;
  
  dof.accelGetOrientation(&accel_event, &orientation);
  //above contains: [orientation.roll, orientation.pitch]

  /* find rotational acceleration from gyroscope data*/
  gyro.getEvent(&gyro_event);
  //above contains: [gyro_event.gyro.x, gyro_event.gyro.y, gyro_event.gyro.z]

  //get pitch angle information
  static double gyro_data_x = 0.0;
  gyro_data_x = (gyro_event.gyro.x - gyro_calibration_factor_x) * 180/PI;
  static double acc_data_roll = 0.0;
  acc_data_roll = orientation.roll;

  //get roll angle information
  static double gyro_data_y = 0.0;
  gyro_data_y = -(gyro_event.gyro.y - gyro_calibration_factor_y) * 180/PI;
  static double acc_data_pitch = 0.0;
  acc_data_pitch = orientation.pitch;

  int time_elapsed = 0;
  end_time =millis();
  time_elapsed = end_time - start_time; 
  start_time = end_time;
  
  pitch_angle = (ALPHA) * (pitch_angle + gyro_data_x * time_elapsed / 1000) + (1-ALPHA) *  acc_data_roll;
  roll_angle  = (ALPHA) * (roll_angle + gyro_data_y  * time_elapsed / 1000) + (1-ALPHA) * acc_data_pitch;
  //////////////////////////
  //End of sensor code./////
  //////////////////////////

  double pitch_angle_rad = pitch_angle * PI/180.0;
  double roll_angle_rad  = roll_angle  * PI/180.0;

// Puts an absolute angle request message in the outbound com queue
  mot_client1.ObsAngle.Get(com1);
  mot_client1.EstMotorTorque.Get(com1);
  
  // Grab outbound messages in the com queue, store into buffer
  // If it transferred something to communication_buffer...
  if(com1.GetTxBytes(communication_buffer_1,communication_length_1))
  {
    // Use Arduino serial hardware to send messages
    Serial1.write(communication_buffer_1,communication_length_1);
    //Serial.println("Grabbed outbound messages from motor1");
  }
  
  
  // wait a bit so as not to send massive amounts of data
  delay(10);

  // Reads however many bytes are currently available
  communication_length_1 = Serial1.readBytes(communication_buffer_1, Serial1.available());
  // Puts the recently read bytes into com's receive queue
  com1.SetRxBytes(communication_buffer_1,communication_length_1);

  uint8_t *rx_data;   // temporary pointer to received type+data bytes
  uint8_t rx_length;  // number of received type+data bytes
  // while we have message packets to parse
  while(com1.PeekPacket(&rx_data,&rx_length))
  {
    // Remember time of received packet
    communication_time_last = millis();
    
    // Share that packet with all client objects
    mot_client1.ReadMsg(com1,rx_data,rx_length);

    // Once we're done with the message packet, drop it
    com1.DropPacket();
  }


  mot_client1.CmdAngle.Set(com1,4.9571);
  Serial.printf("%f, %f, %f, %d\n\r", accel_y, mot_client1.ObsAngle.GetReply(), mot_client1.EstMotorTorque.GetReply(), millis());
  //Serial.println("cycling");

  // If we haven't heard from the motor in 250 milliseconds
  if(millis() - communication_time_last > 250)
  {
    // Toggle the LED
    // Should flash at 5 hz thanks to the delay(100) above
    digitalWrite(kLedPin, !digitalRead(kLedPin));
  }
}
