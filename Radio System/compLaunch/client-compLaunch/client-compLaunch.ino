// RFM22B Client 
// UCLA Rocket Project, 2014-2015
// By: Ky-Cuong L. Huynh
// 24 April 2015

// Original example sketch: rf22_client.pde
// From http://www.airspayce.com/mikem/arduino/RadioHead/rf22_client_8pde-example.html
// mode: C++

#include <SPI.h>
#include <RH_RF22.h>
#include <Wire.h>
#include <URP_LSM303.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <String.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>

// Create instance of the radio driver
RH_RF22 rf22;

/* Assign a unique ID to the sensors */
Adafruit_10DOF    dof   = Adafruit_10DOF();
URP_LSM303_Accel  accel = URP_LSM303_Accel(30301);

// TODO: Generate this name dynamically
const char* FILENAME = "run4-24.txt";

// Hold acceleration and orientation events
sensors_event_t accel_event;
sensors_vec_t   orientation;

// Data buffer for the accel and orientation readings
char dataArray[80];

void setup() 
{
    // This is needed for the duemilanove 
    // and Uno without ethernet shield
    const int sdCardPin = 10;
    delay(1000);    
    pinMode(10,OUTPUT);

    if (!SD.begin(sdCardPin)) {
        // don't do anything more:
        return;
    }

    // Format of data from DOF
    sdWrite("AccelX, AccelY, AccelZ, Roll, Pitch, Heading");
    sdWriteNewline();

    // Initialize the sensors
    initAccel();
    accel.setDataRate((byte)LSM303_ACCEL_DATA_RATE_400HZ);
    accel.setDataRate((byte)LSM303_MAG_DATA_RATE_220HZ);

}
Serial.begin(9600);

// Defaults after init are 434.0MHz, 
// 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
if (!rf22.init())
    Serial.println("RFM22B client initialization failed");
    }

void loop()
{
    Serial.println("Sending message to rf22_server");
    // Send a message to rf22_server
    uint8_t msg[] = "Hello World!";
    rf22.send(msg, sizeof(msg));

    rf22.waitPacketSent();

    // Now wait for a reply
    uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf22.waitAvailableTimeout(500))
    { 
        // There should be a reply message for us now   
        if (rf22.recv(buf, &len))
        {
            Serial.print("Got acknowledgement from server: ");
            Serial.println((char*)buf);
        }
        else
        {
            Serial.println("Receive acknowledgement from server failed");
        }
    }
    else
    {
        Serial.println("No reply, is rf22_server running?");
    }
    delay(400);
}

bool sdWriteNewline() {
    File dataFile = SD.open(FILENAME, FILE_WRITE);
    // If the file is available, write to it:
    if (dataFile) {
        dataFile.println("");
        dataFile.close();
        // print to the serial port too:
    }
    // If the file isn't open, pop up an error:
    else {
        dataFile.close();
    }
}

bool sdWrite(String data) {
    File dataFile = SD.open(FILENAME, FILE_WRITE);
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

