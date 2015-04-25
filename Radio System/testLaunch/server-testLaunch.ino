// RFM22B Server 
// UCLA Rocket Project, 2014-2015
// By: Ky-Cuong L. Huynh
// 24 April 2015

// Original example sketch: rf22_server.pde
// From http://www.airspayce.com/mikem/arduino/RadioHead/rf22_server_8pde-example.html
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
        Serial.println("RFM22B server initialization failed");  
}

void loop()
{
    if (rf22.available())
    {
        // There should be a message for us now   
        uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf22.recv(buf, &len))
        {
            //      RF22::printBuffer("request: ", buf, len);
            Serial.print("Got message from client: ");
            Serial.println((char*)buf);
            //      Serial.print("RSSI: ");
            //      Serial.println(rf22.lastRssi(), DEC);

            // Send a reply
            uint8_t ack[] = "Acknowledgement from server." 
            rf22.send(ack, sizeof(ack));
            rf22.waitPacketSent();
            Serial.println("Sent an acknowledgement");
        }
        else
        {
            Serial.println("Receive message from client failed");
        }
    }
}

