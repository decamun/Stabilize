
#include <bipbuffer.h>
#include <byte_queue.h>
#include <communication_interface.h>
#include <crc_helper.h>
#include <packet_finder.h>

// Includes required for communication
// Message forming interface
#include <generic_interface.hpp> 
// Client that speaks to complex motor controllers
#include <complex_motor_control_client.hpp> 





// This buffer is for passing around messages.
// We use one buffer here to save space.
uint8_t communication_buffer[256];
// Stores length of message to send or receive
uint8_t communication_length;

ComplexMotorControlClient mot_client1(0);
GenericInterface com1;

ComplexMotorControlClient mot_client2(0);
GenericInterface com2;

ComplexMotorControlClient mot_client3(0);
GenericInterface com3;

ComplexMotorControlClient mot_client4(0);
GenericInterface com4;

void motControlSetup() {
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  Serial4.begin(115200);
}

void motSetup(int motNum, float Kp, float Ki, float Kd) {
  ComplexMotorControlClient *mot_client;
  GenericInterface *com;

  switch(motNum) {
    case 1:
      mot_client = &mot_client1;
      com = &com1;
      break;
    case 2:
      mot_client = &mot_client2;
      com = &com2;
      break;
    case 3:
      mot_client = &mot_client3;
      com = &com3;
      break;
    case 4:
      mot_client = &mot_client4;
      com = &com4;
      break;
  }

  
  mot_client->AngleKp.Set(*com,Kp);
  mot_client->AngleKi.Set(*com,Ki);
  mot_client->AngleKd.Set(*com,Kd);
  
  logln(String("Motor Setup for ") + String(motNum) + String(" done."));
}

void setVal(int motNum, int val, float pos) {
  logln(String("Sending motor command to ") + String(motNum) + String("..."));
  
  ComplexMotorControlClient *mot_client;
  GenericInterface *com;

  switch(motNum) {
    case 1:
      mot_client = &mot_client1;
      com = &com1;
      break;
    case 2:
      mot_client = &mot_client2;
      com = &com2;
      break;
    case 3:
      mot_client = &mot_client3;
      com = &com3;
      break;
    case 4:
      mot_client = &mot_client4;
      com = &com4;
      break;
    default:
      return;
  }

  switch(val) {
    case ANGLE:
      mot_client->CmdAngle.Set(*com, pos);
      break;
    default:
      return;
  }
  
}

float getVal(int motNum, int val) {

  logln(String("Requesting value from ") + String(motNum) + String("..."));
  
  ComplexMotorControlClient *mot_client;

  switch(motNum) {
    case 1:
      mot_client = &mot_client1;
      break;
    case 2:
      mot_client = &mot_client2;
      break;
    case 3:
      mot_client = &mot_client3;
      break;
    case 4:
      mot_client = &mot_client4;
      break;
    default:
      return 0;
  }

  switch(val) {
    case ANGLE:
      return mot_client->ObsAngle.GetReply();
    case ABS_ANGLE:
      return mot_client->ObsAbsoluteAngle.GetReply();
    default:
      return 0;
  }
}

void syncMotor(int motNum) {
  ComplexMotorControlClient *mot_client;
  GenericInterface *com;

  logln(String("Syncing motor ") + String(motNum) + String(":"));
  
  switch(motNum) {
    case 1:
      mot_client = &mot_client1;
      com = &com1;
      break;
    case 2:
      mot_client = &mot_client2;
      com = &com2;
      break;
    case 3:
      mot_client = &mot_client3;
      com = &com3;
      break;
    case 4:
      mot_client = &mot_client4;
      com = &com4;
      break;
    default:
      return;
  }

  logln("Making 'get' commands...");

  mot_client->ObsAngle.Get(*com);
  mot_client->ObsAbsoluteAngle.Get(*com);

  logln("Recieving Updates...");
  
  if(com->GetTxBytes(communication_buffer,communication_length)) {
    switch(motNum) {
      case 1:
        Serial1.write(communication_buffer,communication_length);
        communication_length = Serial1.readBytes(communication_buffer, Serial1.available());
        break;
      case 2:
        Serial2.write(communication_buffer,communication_length);
        communication_length = Serial2.readBytes(communication_buffer, Serial2.available());
        break;
      case 3:
        Serial3.write(communication_buffer,communication_length);
        communication_length = Serial3.readBytes(communication_buffer, Serial3.available());
        break;
      case 4:
        Serial4.write(communication_buffer,communication_length);
        communication_length = Serial4.readBytes(communication_buffer, Serial4.available());
        break;
      default:
        return;
    }
  }

  logln("Serial operations complete...");

  com->SetRxBytes(communication_buffer,communication_length);

  logln("Transmitting commands...");
  
  uint8_t *rx_data;   // temporary pointer to received type+data bytes
  uint8_t rx_length;  // number of received type+data bytes
  // while we have message packets to parse
  while(com->PeekPacket(&rx_data,&rx_length))
  { 
    // Share that packet with all client objects
    mot_client->ReadMsg(*com,rx_data,rx_length);

    // Once we're done with the message packet, drop it
    com->DropPacket();
  }
}





