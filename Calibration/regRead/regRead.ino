/*
*  Accelerometer skeleton code
*
*/
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <String.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_10DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);

void initAccel()
{
  if(!accel.begin())
  {
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
  Serial.begin(9600);
  delay(2000);
  initAccel();
  Serial.print("Data rate initially:");
  Serial.println(accel.getDataRate());
  accel.setDataRate((byte)LSM303_DATA_RATE_400HZ);
  Serial.print("Data rate afterward:");
  Serial.println(accel.getDataRate());
  char buf[60];
  float a = 10.0;
  float b = 11.0;
  float c = 12.0;
  int d = 5;
  sprintf(buf, "%d, %2.1f, %d", d, b, c);
  Serial.print(accel.getAccel);
}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  delay(5000);
}

