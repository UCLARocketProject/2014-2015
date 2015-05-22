/*
*  Accelerometer skeleton code
*/
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <URP_LSM303.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <String.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_Simple_AHRS.h>

/* Assign a unique ID to the sensors */
Adafruit_10DOF   dof();
URP_LSM303_Accel accel(30301);
URP_LSM303_Mag   mag(30302);
Adafruit_Simple_AHRS ahrs(&accel, &mag);

// Data buffer for the accel and orientation readings
char dataString[60];
File dataFile;
const char* FILENAME = "tlaunch.txt";
sensors_event_t accel_event;
sensors_vec_t orientation;

int mainLoopCounter;
void initAccel()
{
  // accel.begin() method includes code for G-force range increase
  if (!accel.begin())
  {
    Serial.println("accel.begin error");
    /* There was a problem detecting the LSM303 ... check your connections */
    while(1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  // This is needed for the duemilanove and uno without ethernet shield
  const int sdCardPin = 10;
  delay(2000);
  pinMode(10,OUTPUT);

  if (!SD.begin(sdCardPin)) {
    // Don't do anything more:
    return;
  }

  initAccel();
  accel.setDataRate((byte)LSM303_ACCEL_DATA_RATE_400HZ);
  mag.setDataRate((byte)LSM303_MAG_DATA_RATE_220HZ);
  dataFile = SD.open(FILENAME, FILE_WRITE);

  // If the file is available, write to it:
  dataFile.println("Accel X, Accel Y, Accel Z, Roll, Timestamp");
  dataFile.close();
}

void loop(void)
{
  openSd();
  for (mainLoopCounter = 0; mainLoopCounter < 1000; mainLoopCounter++) {
    accel.getEvent(&accel_event);
    ahrs.getOrientation(&orientation);
    sdWrite(accel_event.acceleration.x);
    sdWrite(accel_event.acceleration.y);
    sdWrite(accel_event.acceleration.z);
    sdWrite(orientation.roll);
    sdWriteNewline();  
  }
  closeSd();
}

// $dataFile is a global variable so you dont return anything
void openSd() {
  dataFile = SD.open(FILENAME, FILE_WRITE);
}

// $dataFile is a global
void closeSd() {
  dataFile.close();
}

void sdWriteNewline() {
  // File dataFile = SD.open(FILENAME, FILE_WRITE);
  // if the file is available, write to it:
  dataFile.println(millis());
  // dataFile.close();
}
  
void sdWrite(float data) {
  // File dataFile = SD.open(FILENAME, FILE_WRITE);
  // if the file is available, write to it:
  dataFile.print(data);
  dataFile.print(", ");
  // dataFile.close();
}
