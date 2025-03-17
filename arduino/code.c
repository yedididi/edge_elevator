/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

#define DEBUG
//#define DEBUG_WIFI
#define SERVER_NAME "10.10.141.45"
#define SERVER_PORT 5000  

#define WIFITX 7  //7:TX -->ESP8266 RX
#define WIFIRX 6 //6:RX-->ESP8266 TX
#define LED_TEST_PIN 12
#define LED_BUILTIN_PIN 13
#define BUTTON_PIN 2
#define CMD_SIZE 50
#define ARR_CNT 5           

#include "WiFiEsp.h"
#include "SoftwareSerial.h"
#include <TimerOne.h>

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
char sendBuf[CMD_SIZE];

SoftwareSerial wifiSerial(WIFIRX, WIFITX); 
WiFiEspClient client;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int buttonPressed = 0;

void setup() 
{
    // initialize the LED pin as an output:
    pinMode(ledPin, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
    Serial.begin(38400); //DEBUG
    wifi_Setup();
    Serial.print("init ok\n");
}

void wifi_Setup() 
{
    wifiSerial.begin(38400);
    Serial.print("wifiSerial ok\n");
    wifi_Init();
    Serial.print("wifi init ok\n");
    server_Connect();
    Serial.print("server connect ok\n");
}

void wifi_Init()
{
    do 
    {
        WiFi.init(&wifiSerial);
        if (WiFi.status() == WL_NO_SHIELD) { }
        else
            break;   
    } while(1);

}

int server_Connect()
{
  client.connect(SERVER_NAME, SERVER_PORT);
}

void loop() 
{
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) 
    {
        // turn LED on:
        digitalWrite(ledPin, HIGH);
        sprintf(sendBuf,"Button Pressed\n");
        Serial.println("Button Pressed");
        client.write(sendBuf,strlen(sendBuf));
        client.flush();

    } 
    else 
    {
        // turn LED off:
        digitalWrite(ledPin, LOW);
    }
}
