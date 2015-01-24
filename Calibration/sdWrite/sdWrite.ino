#include <SD.h>
#include <SPI.h>

void setup() {
  
  //this is needed for the duemilanove and uno without ethernet shield
  const int sdCardPin = 10;
  int loopCount = 0;

  //for testing. delete before launches.
  Serial.begin(9600);
  delay(1000);
  pinMode(10,OUTPUT);
  
  if (!SD.begin(sdCardPin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return; 
  }
  Serial.println("card initialized.");
}

void loop() {
  String test;
  test = "Kai";
  sdWrite(test);
}

/*
Filename MUST be <=8 characters (not including the file extension) or the
file will not be created
*/
bool sdWrite(String data) {
  File dataFile = SD.open("kai.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    // print to the serial port too:
    Serial.println(data);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
    dataFile.close();
  }
}


