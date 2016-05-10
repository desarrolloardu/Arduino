// EspDebug - Test sketch for ESP8266 module

// emulate Serial1 if not present
/*
#ifndef HAVE_HWSERIAL1
  #include "SoftwareSerial.h"
  SoftwareSerial Serial1(10,11); // RX, TX
#endif
*/

#include <SoftwareSerial.h>
SoftwareSerial BT1(10,11); // RX | TX

void setup()
{
  Serial.begin(9600); // serial port used for debugging
  BT1.begin(19200);  // your ESP's baud rate might be different
}
 
void loop()
{
  if(BT1.available())  // check if the ESP is sending a message
  {
    while(BT1.available())
    {
      int c = BT1.read(); // read the next character
      Serial.print((char)c);  // writes data to the serial monitor
    }
  }
 
  if(Serial.available())
  {
    // wait to let all the input command in the serial buffer
    delay(10);

    // read the input command in a string
    String cmd = "";
    while(Serial.available())
    {
      cmd += (char)Serial.read();
    }

    // print the command and send it to the ESP
    Serial.println();
    Serial.print(">>>> ");
   // Serial.println(cmd);

    // send the read character to the ESP
    BT1.print(cmd);
  }
}
