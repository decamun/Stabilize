#define IMUSerial Serial6

void imuSetup() {
  IMUSerial.begin(115200);
}

bool updateImu() {
  logln("Updating IMU...");
  bool worked = false;
  if(IMUSerial.available()) {
    while (IMUSerial.available()){  // Reads byte from buffer.  Valid packet returns true.
      imu.encode(IMUSerial.read());
      worked = true;
    }
  }
  return worked;
}


