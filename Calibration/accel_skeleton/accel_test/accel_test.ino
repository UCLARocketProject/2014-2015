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
  //this is needed for the duemilanove and uno without ethernet shield
  const int sdCardPin = 10;
  delay(1000);    
  pinMode(10,OUTPUT);
  if (!SD.begin(sdCardPin)) {
    // don't do anything more:
    return;
  }
  //format
  sdWrite("AccelX, AccelY, AccelZ, Roll, Pitch, Heading");
  sdWriteNewline();
  /* Initialise the sensors */
  initAccel();
}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  sensors_event_t accel_event;
  sensors_vec_t   orientation;
  //16 bits long because thats what floatToString takes
  char dataArray[16];
  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    floatToString(dataArray, accel_event.acceleration.x, 3, 5);
    sdWrite(dataArray);
    sdWrite(", ");
    floatToString(dataArray, accel_event.acceleration.y, 3, 5);
    sdWrite(dataArray);
    sdWrite(", ");
    floatToString(dataArray, accel_event.acceleration.z, 3, 5);
    sdWrite(dataArray);
    sdWrite(", ");
    floatToString(dataArray, orientation.roll, 3, 5);
    sdWrite(dataArray);
    sdWrite(", ");
    floatToString(dataArray, orientation.pitch, 3, 5);
    sdWrite(dataArray);
    sdWrite(", ");
    floatToString(dataArray, orientation.heading, 3, 5);
    sdWrite(dataArray);
    sdWriteNewline();
  }
  delay(5);
}
bool sdWriteNewline() {
  File dataFile = SD.open("run2-13.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("");
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    dataFile.close();
  }
}

bool sdWrite(String data) {
  File dataFile = SD.open("run2-13.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(data);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    dataFile.close();
  }
}
char * floatToString(char * outstr, double val, byte precision, byte widthp){
 char temp[16]; //increase this if you need more digits than 15
 byte i;

 temp[0]='\0';
 outstr[0]='\0';

 if(val < 0.0){
   strcpy(outstr,"-\0");  //print "-" sign
   val *= -1;
 }

 if( precision == 0) {
   strcat(outstr, ltoa(round(val),temp,10));  //prints the int part
 }
 else {
   unsigned long frac, mult = 1;
   byte padding = precision-1;
   
   while (precision--)
     mult *= 10;

   val += 0.5/(float)mult;      // compute rounding factor
   
   strcat(outstr, ltoa(floor(val),temp,10));  //prints the integer part without rounding
   strcat(outstr, ".\0"); // print the decimal point

   frac = (val - floor(val)) * mult;

   unsigned long frac1 = frac;

   while(frac1 /= 10) 
     padding--;

   while(padding--) 
     strcat(outstr,"0\0");    // print padding zeros

   strcat(outstr,ltoa(frac,temp,10));  // print fraction part
 }

 // generate width space padding 
 if ((widthp != 0)&&(widthp >= strlen(outstr))){
   byte J=0;
   J = widthp - strlen(outstr);

   for (i=0; i< J; i++) {
     temp[i] = ' ';
   }

   temp[i++] = '\0';
   strcat(temp,outstr);
   strcpy(outstr,temp);
 }

 return outstr;
}
