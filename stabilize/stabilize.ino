#include <SD.h>
#include <SPI.h>

#define LOGGING 1 
//0: no logging 
//1: serial logging,
//2: SD card logging

//Filenames for logging
#define LOGFILE "slog.txt"
#define NAMEFILE "slast.txt"
#define DATAFILESTEM "sdat"//##.txt
String fileName = DATAFILESTEM;
int fileNumber = 0;
const int chipSelect = BUILTIN_SDCARD;

//use this function to log a line to the computer
void logln(String msg) {
  if(LOGGING == 1) {
    Serial.println(msg);
  } else if(LOGGING == 2) {
    File logfile = SD.open(LOGFILE, FILE_WRITE);
    if(logfile){
      logfile.println(String(fileNumber) + String("|") + msg);
      logfile.close();
    } else {
      return;
    }
  }
}


//Setup for data loggin to SD card
void dataLoggingSetup() {
  logln("Initializing SD card...");
  
  if(!SD.begin(chipSelect)) {
    logln("Card failed, or not present");
    return;
  }

  logln(String("Looking for ") + String(NAMEFILE) + String("..."));
  
  if(SD.exists(NAMEFILE)) {
    logln("Previous namefile exists");

    //read existing namefile and parse new file number
    File namefile = SD.open(NAMEFILE, FILE_READ);
    String fileNumberS = "";
    while(namefile.available()){
      char next = namefile.read();
      fileNumberS += next;
    }
    logln(String("Previous file number:") + fileNumberS);
    fileNumber = fileNumberS.toInt() + 1;
    
    //erase previous namefile
    namefile.close();
    SD.remove(NAMEFILE);
    
    //write a new namefile
    namefile = SD.open(NAMEFILE, FILE_WRITE);
    if(namefile) {
      fileName += String(fileNumber) + ".txt";
      namefile.print(String(fileNumber));
      namefile.close();
    } else {
      logln(String("Error opening ") + String(NAMEFILE) + String("!"));
      return;
    }
    
    
  } else {
    logln("No previous namefile exists. Creating new namefile.");
    
    fileName += "0.txt"; 

    //make a namefile
    File namefile = SD.open(NAMEFILE, FILE_WRITE);
    if(namefile) {
      namefile.print(String("0"));
      namefile.close();
    } else {
      logln(String("Error opening ") + String(NAMEFILE) + String("!"));
      return;
    }
    
  }

  //Done!
  logln("Initialized SD card logging. File Name: " + fileName + " File Number: " + String(fileNumber));
  
}


void dataln(String dat) {
  char fileNameC[fileName.length()+1];
  fileName.toCharArray(fileNameC, fileName.length()+1);
  File datafile = SD.open(fileNameC, FILE_WRITE);
  if(datafile) {
    datafile.println(dat);
    datafile.close();
  } else {
    logln(String("Failed to open ") + fileName + String(" for data logging. data: ") + dat);
  }
}


void setup() {
  if(LOGGING == 1) {
    //wait for USB
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  }
  dataLoggingSetup();
}

void loop() {
  static unsigned long t1 = millis();
  static unsigned long t2 = millis();
  
  String string = String("time elapsed (milliseconds): ");
  t2 = millis();
  string += String(t2 - t1);
  t1 = t2;
  dataln(string);
  
}
