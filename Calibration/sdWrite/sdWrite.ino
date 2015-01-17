/* Temperature to SD Card 
 * UCLA Rocket Project
 * 16 January 2015
 */

#include <SD.h>
#include <SPI.h>

// Number of desired data points
const int numValues = 100;
// This is needed for the Duemilanove 
// and Uno without ethernet shield
const int SDCardPin = 10;
// Analog 0
const int tempPin = 11;
// Range of TMP36
const int TMP36_range = 1023;
// Maximum voltage response of TMP36
const int TMP36_maxVolt = 5;

void setup() 
{
    // TODO: For testing only 
    Serial.begin(9600);
    delay(1000);
    pinMode(10, OUTPUT);
  
    if (!SD.begin(SDCardPin)) {
        Serial.println("Card failed, or not present");
        // Don't do anything more:
        return; 
    }

    Serial.println("Card initialized.");
}

void loop() 
{
    String data;
    for (int i = 0; i < numValues; i++) {
        double temp 
            = temperature(TMP36_range, tempPin, TMP36_maxVolt);
        data += String(temp);
        data += '\n';
    }

    SD_write(data);
}

/*
 * Filename MUST be <=8 characters (not including the 
 * file extension) or the file will not be created
 */
bool SD_write(String data) 
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

/* TMP36 conversion from voltage to temperature in Celsius */
double voltToTemp(int voltage)
{
    return 100 * voltage - 50;
}

/* sensorRange is the range of values returned by 
 * the sensor, e.g. a temperature sensor with 1024 levels, 
 * spanning 0 to 1023. 
 * pinNum is the digital pin number. 
 * voltMax is the maximum voltage response of the sensor
 */
double temperature(int sensorRange, int pinNum, unsigned voltMax)
{
        // Read raw value, which is 0-sensorRange
        double sensorValue = (double)digitalRead(pinNum);

        // Compute as fraction of voltage against voltage max
        double voltage = (sensorValue / sensorRange) * voltMax;
    
        return voltToTemp(voltage);
}

