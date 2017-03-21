#define IMUSerial Serial5

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

    logln(String("Roll: ") + String(imu.roll));
    logln(String("Pitch: ") + String(imu.pitch));
    logln(String("Yaw: ") + String(imu.yaw));
    logln(String("AX: ") + String(imu.ax));
    logln(String("AY: ") + String(imu.ay));
    logln(String("AZ: ") + String(imu.az));
  }
  return worked;
}


