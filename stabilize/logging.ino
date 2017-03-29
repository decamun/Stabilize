#include <SD.h>
#include <SPI.h>

//Filenames for logging
#define LOGFILE "slog.txt"
#define NAMEFILE "slast.txt"
#define DATAFILESTEM "sdat"//##.txt
char logs[500];
char fileName[20] = DATAFILESTEM;
int fileNumber = 0;
const int chipSelect = BUILTIN_SDCARD;

bool sdActive = false;

//logging permission control
bool logging = true;
void setLogPermission(bool loggingP) {
  logging = loggingP;
}

//use this function to log a runtime message
void logln(char * msg) {
  
  
  if(!logging) {
    return;
  }
  if(LOGGING == 1) {
    Serial.println(msg);
  } else if(LOGGING == 2 && sdActive) {
    File logfile = SD.open(LOGFILE, FILE_WRITE);
    if(logfile){
      char msgcat[300];
      sprintf(msgcat, "%d|%s", fileNumber, msg);
      logfile.println(msgcat);
      logfile.close();
    } else {
      return;
    }
  }
}

void stronglogln(char* msg) {
  bool prevPerm = logging;
  setLogPermission(true);
  logln(msg);
  setLogPermission(prevPerm);
}


//Setup for data logging to SD card
void dataLoggingSetup() {
  if(LOGGING == 1) {
    //wait for USB
    Serial.begin(115200);
    while (!Serial);
  }

  //activate SD card
  logln("Initializing SD card...");
  if(!SD.begin(chipSelect)) {
    logln("Card failed, or not present");
    return;
  }
  sdActive = true;
  
  sprintf(logs,"Looking for NAMEFILE");
  logln(logs);

  if(SD.exists(NAMEFILE)) {
    logln("Previous namefile exists");

    //read existing namefile and parse new file number
    File namefile = SD.open(NAMEFILE, FILE_READ);
    String fileNumberS = "";
    while(namefile.available()){
      char next = namefile.read();
      fileNumberS += next;
    }

    fileNumber = fileNumberS.toInt() + 1;

    //log last file number
    sprintf(logs, "Previous file number:%d", fileNumber-1);
    logln(logs);
    

    //erase previous namefile
    namefile.close();
    SD.remove(NAMEFILE);

    //write a new namefile
    namefile = SD.open(NAMEFILE, FILE_WRITE);
    if(namefile) {
      sprintf(fileName, "%s%d.txt", DATAFILESTEM, fileNumber);

      char fileNumberS[20];
      sprintf(fileNumberS,"%d",fileNumber);
      namefile.print(fileNumberS);
      namefile.close();
    } else {

      sprintf(logs, "Error opening %s!", NAMEFILE);
      logln(logs);
      return;
    }


  } else {
    logln("No previous namefile exists. Creating new namefile.");

    sprintf(fileName, "%s0.txt", DATAFILESTEM);

    //make a namefile
    File namefile = SD.open(NAMEFILE, FILE_WRITE);
    if(namefile) {
      namefile.print("0");
      namefile.close();
    } else {
      sprintf(logs, "Error opening %s!", NAMEFILE);
      logln(logs);
      return;
    }

  }

  //Done!

  sprintf(logs, "Initialized SD card logging. \nFile Name: %s \nFile Number: %d", fileName, fileNumber);
  logln(logs);
}

//use this function to log data to the SD card
void dataln(char* dat) {
  File datafile = SD.open(fileName, FILE_WRITE);
  if(datafile) {
    datafile.println(dat);
    datafile.close();
  } else {
    sprintf(logs, "Failed to open %s for fata logging. data: %s", fileName, dat);
    stronglogln(logs);
  }
  
}
