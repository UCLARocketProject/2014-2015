/*
  SD card datalogger
 
Log data from temperature sensor to SD card. 
 
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 */

#include <SPI.h>
#include <SD.h>

// Note the chip select (CS) pin. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;

int failed = 0; 

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(1000);

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    failed = 1;
    return;
  }
  Serial.println("Card initialized.");
}

void loop()
{
  if (failed == 1) {
    return;
  }
  // make a string for assembling the data to log:
  String dataString = "";

  // Temperature sensor at A0
  int digitalPin = 11;

  int numValues = 100;
  for (int i = 0; i < numValues; i++) {
    // Read raw value, which is 0-1023
    double sensorValue = (double)digitalRead(digitalPin);

    // Compute as fraction of voltage against 5V max
    double voltage = (sensorValue / 1023) * 5;

    // Convert:        Temperature = 100 * voltage - 50
    // Equation form:  T = 100 * V - 50
    double temp = 100 * voltage - 50;
    dataString += String(sensorValue);
    dataString += "\n";
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("TEMP001.TXT", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
    return;
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("[ERROR]: opening datalog.txt");
    return;
  }
}

