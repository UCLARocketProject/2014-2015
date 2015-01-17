#include <SD.h>
#include <SPI.h>

void setup() 
{
  // This is needed for the duemilanove and uno without ethernet shield
  const int sdCardPin = 10;
  int loopCount = 0;

  // TODO: Serial output for testing only. 
  Serial.begin(9600);
  delay(1000);

  // Pin 10 needs to be initialized as output
  // for SD operations to work
  pinMode(10, OUTPUT);
  
  if (!SD.begin(sdCardPin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return; 
  }

  Serial.println("Card initialized.");
}

void loop() 
{
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

  sdWrite("dataString");
}

/*
Filename MUST be <=8 characters (not including the file extension) or the
file will not be created
*/
bool sdWrite(String data) 
{
  File dataFile = SD.open("temp.txt", FILE_WRITE);
  // If the file is available, write to it:
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    // Print to the serial port too:
    Serial.println(data);
  }
  // If the file isn't open, pop up an error:
  else {
    Serial.println("Error opening file");
    dataFile.close();
  }
}

