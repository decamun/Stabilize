

void imuSetup() {
  Serial6.begin(115200);
}

void updateImu() {
  logln("Updating IMU...");
  if(Serial6.available()) {
    while (Serial6.available()){  // Reads byte from buffer.  Valid packet returns true.
      imu.encode(Serial6.read());
    }
    logln("   success!");
  }
}


