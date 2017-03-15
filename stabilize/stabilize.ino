#include <SD.h>
#include <SPI.h>

#define LOGGING 1 
//0: no logging 
//1: serial logging,
//2: SD card logging

#define LOGFILE "stabilize_log.txt"
#define NAMEFILE "stabilize_last.txt"

//use this function to log a line to the computer
void logln(String msg) {
  if(LOGGING == 1) {
    Serial.println(msg);
  } else if(LOGGING == 2) {
    //TODO
  }
}


String fileName = "stabilize_data_";
int fileNumber = 0;

//Setup for data loggin to SD card
void dataLoggingSetup() {
  while(!SD.begin(BUILTIN_SDCARD)) {
    logln("Card failed, or not present");
  }
  if(SD.exists(NAMEFILE)) {

    //read existing namefile and parse new file number
    File namefile = SD.open(NAMEFILE);
    String fileNumberS = "";
    while(namefile.available()){
      fileNumberS += namefile.read();
    }
    fileNumber = fileNumberS.toInt() + 1;
    
    //erase previous namefile
    namefile.close();
    SD.remove(NAMEFILE);
    
    //write a new namefile
    namefile = SD.open(NAMEFILE);
    fileName += String(fileNumber) + ".txt";
    char charbuf[50];
    String(fileNumber).toCharArray(charbuf, 50);
    namefile.write(charbuf, 50);
    namefile.close();
    
  } else {
    fileName += "0.txt";

    //make a namefile
    File namefile = SD.open(NAMEFILE);
    namefile.write('0');
    namefile.close();
  }

  //Done!
  logln("Initialized SD card logging. File Name: " + fileName + " File Number: " + String(fileNumber));
  
}



void setup() {
  dataLoggingSetup();
}

void loop() {
  // put your main code here, to run repeatedly:

}
