/*
*  Accelerometer skeleton code
*
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

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
URP_LSM303_Accel accel = URP_LSM303_Accel(30301);
const char* FILENAME = "run2-26.txt";
sensors_event_t accel_event;
sensors_vec_t   orientation;
//data buffer for the accel and orientation readings
char dataArray[80];

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
  delay(2000);
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
  accel.setDataRate((byte)LSM303_ACCEL_DATA_RATE_400HZ);
  accel.setDataRate((byte)LSM303_MAG_DATA_RATE_220HZ);

}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    sdWrite((String)accel_event.acceleration.x);
    sdWrite((String)accel_event.acceleration.y);
    sdWrite((String)accel_event.acceleration.z);
    sdWrite((String)accel_event.orientation.roll);
    sdWrite((String)accel_event.orientation.pitch);
    sdWrite((String) accel_event.orientation.heading);
    sdWriteNewline();
  }
}
bool sdWriteNewline() {
  File dataFile = SD.open(FILENAME, FILE_WRITE);
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
  File dataFile = SD.open(FILENAME, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(data);
    dataFile.print(", ");i
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
