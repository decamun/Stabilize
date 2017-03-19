#define IMUSerial Serial6

void imuSetup() {
  IMUSerial.begin(115200);
}

void updateImu() {
  logln("Updating IMU...");
  if(IMUSerial.available()) {
    while (IMUSerial.available()){  // Reads byte from buffer.  Valid packet returns true.
      imu.encode(IMUSerial.read());
    }
    logln("   success!");
  }
}


