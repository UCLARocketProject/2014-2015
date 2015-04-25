// RFM22B Client 
// UCLA Rocket Project, 2014-2015
// By: Ky-Cuong L. Huynh
// 24 April 2015

// Original example sketch: rf22_client.pde
// From http://www.airspayce.com/mikem/arduino/RadioHead/rf22_client_8pde-example.html
// mode: C++

#include <SPI.h>
#include <RH_RF22.h>

// Create instance of the radio driver
RH_RF22 rf22;

void setup() 
{
    Serial.begin(9600);

    // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
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
