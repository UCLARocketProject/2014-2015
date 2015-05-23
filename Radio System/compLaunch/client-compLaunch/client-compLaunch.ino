// RFM22B Client 
// UCLA Rocket Project, 2014-2015
// By: Ky-Cuong L. Huynh
// Created: 24 April 2015
// Updated: 1 May 2015

// Original example sketch: rf22_client.pde
// From http://www.airspayce.com/mikem/arduino/RadioHead/rf22_client_8pde-example.html
// mode: C++

// Basic libraries
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <String.h>
// Radio and sensors
#include <RH_RF22.h>
#include <URP_LSM303.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_Simple_AHRS.h>

// Create instance of the radio driver
RH_RF22 rf22;

/* Assign a unique ID to the sensors */
Adafruit_10DOF   dof();
URP_LSM303_Accel accel(30301);
URP_LSM303_Mag   mag(30302);
Adafruit_Simple_AHRS ahrs(&accel, &mag);

// Hold sensor events
sensors_event_t accel_event;
sensors_vec_t   orientation;
char dataBuffer[RH_RF22_MAX_MESSAGE_LEN];

// This must be less than 8 characters (9 bytes
// including the null byte)
const int fileNameSize = 9;
char strFileName[fileNameSize];
File dataFile;

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

void setup() 
{
    Serial.begin(9600);
    delay(1000); 

    // This is needed for the duemilanove 
    // and Uno without Ethernet shield
    const int sdCardPin = 10;
    pinMode(10, OUTPUT);

    // Defaults after init are 434.0MHz, 
    // 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
    if (!rf22.init()) {
        Serial.println("RFM22B client initialization failed");
    }

    if (!accel.begin()) {
        // TODO: Handle accelerometer failure
    }
     // TODO: How should we handle SD card failure?
    //if (!SD.begin(sdCardPin)) {
    //     // don't do anything more:
    //     return;
    // }
    // TODO: Add barometer and temperature sensors
    // Initialize the sensors

    initAccel();
    accel.setDataRate((byte)LSM303_ACCEL_DATA_RATE_400HZ);
    accel.setDataRate((byte)LSM303_MAG_DATA_RATE_220HZ);

    getNewFileName();

    // Format of data from DOF
    // Need to write this manually, as it's a string
    dataFile = SD.open(strFileName, FILE_WRITE);
    dataFile.println("AccelX, AccelY, AccelZ, Roll, Pitch, Heading");
    sdWriteNewline();
    dataFile.close();
}

void loop()
{
    openSd();
    int mainLoopCounter;
    for (mainLoopCounter = 0; mainLoopCounter < 1000; mainLoopCounter++) {
        accel.getEvent(&accel_event);
        ahrs.getOrientation(&orientation);

        sdWrite(accel_event.acceleration.x);
        sdWrite(accel_event.acceleration.y);
        sdWrite(accel_event.acceleration.z);
        sdWrite(orientation.roll);
        sdWrite(orientation.pitch);
        sdWrite(orientation.heading);
        sdWriteNewline();  

        sprintf(dataBuffer, "%f, %f, %f, %f, %f, %f", 
            accel_event.acceleration.x, 
            accel_event.acceleration.y, 
            accel_event.acceleration.z,
            orientation.roll, 
            orientation.pitch, 
            orientation.heading
           );

        closeSd();
    }

    // Debug: Serial.println("Sending message to rf22_server");
    // Send a message to rf22_server
    rf22.send((const uint8_t*)dataBuffer, sizeof(dataBuffer));

    rf22.waitPacketSent();

    // Now wait for a reply
    uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    // TODO: We need to determine proper timeouts
    if (rf22.waitAvailableTimeout(10))
    { 
        // TODO: What to do with server acknowledgements?
        // There should be a reply message for us now   
        if (rf22.recv(buf, &len))
        {
            // Debug: Serial.print("Got acknowledgement from server: ");
            // Debug: Serial.println((char*)buf);
        }
        else
        {
            // Debug: Serial.println("Receive acknowledgement from server failed");
        }
    }
    else
    {
        // TODO: Handle nonresponsive server
        // Serial.println("No reply, is rf22_server running?");
    }
    delay(5);
}

/*
   Gives a new filename so we can open a new file each time
   Returns no failure code. If we write enough files, 
   the length will eventually overflow, and we'll 
   default to nameOF.txt in that case
 */
void getNewFileName() {
    // Debug: Serial.println("started to get filename");
    strncpy(strFileName, "run0.txt", fileNameSize);
    int nameSize = 0;
    for(int i = 0; SD.exists(strFileName); nameSize = sprintf(strFileName, "run%d.txt", i)) {
        // Catch potential overflow with the name size then we have an issue.
        // so we write to the file "runOF.txt"
        // Debug: Serial.print("Checking filename ");
        // Debug: Serial.println(strFileName);
        if (nameSize > 12) {
            strncpy(strFileName, "runOF.txt", fileNameSize);
            return;
        }
        i++;
    }
    // Debug: Serial.println("Got filename");
    return;
}

// Write a newline '\n' to the log file
bool sdWriteNewline() {
    dataFile.println(millis());
}

bool sdWrite(float data) {
    dataFile.print(data);
    dataFile.print(", ");
}

// $dataFile is a global variable so you dont return anything
void openSd() {
    dataFile = SD.open(strFileName, FILE_WRITE);
}

// $dataFile is a global
void closeSd() {
    dataFile.close();
}


