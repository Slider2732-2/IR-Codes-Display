// InfraRed Remote Control Code Display
// Slider2732_
// September 2026

// SSD1306 screen
// Arduino Nano
// 4x colour coded LED's - change codes for them based on readings taken with the software :-)


#include <Arduino.h>

#define IR_USE_AVR_TIMER1
#include <IRremote.hpp>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_HEIGHT 32
#define IR_RECEIVE_PIN 2

Adafruit_SSD1306 display(128, 32, &Wire, -1);


void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    // Start OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(10, 10);
    display.print(F("IR Codes"));

    display.display();


    // Start IR receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.println(F("IR receiver ready."));

     // LED's check
    digitalWrite(3, HIGH); delay(500); digitalWrite(3, LOW);
    digitalWrite(4, HIGH); delay(500); digitalWrite(4, LOW);
    digitalWrite(5, HIGH); delay(500); digitalWrite(5, LOW);
    digitalWrite(6, HIGH); delay(500); digitalWrite(6, LOW);

    display.clearDisplay();
}


void loop()
{
    if (IrReceiver.decode())
    {
        // Show received command on OLED
        display.clearDisplay();

        display.setTextSize(2);
        display.setCursor(0,0);
        display.print("0x");
        display.println(IrReceiver.decodedIRData.command, HEX);

        if (IrReceiver.decodedIRData.protocol == UNKNOWN)
        {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print(F("Unknown"));
        }

        display.display();


        // Print command to Serial
        Serial.print(F("Command = 0x"));
        Serial.println(IrReceiver.decodedIRData.command, HEX);


        // Ignore repeated button transmissions for the LED actions
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            if (IrReceiver.decodedIRData.command == 0x7A)
            {
                digitalWrite(3, HIGH);
                delay(1000);
                digitalWrite(3, LOW);
            }

            else if (IrReceiver.decodedIRData.command == 0x7B)
            {
                digitalWrite(4, HIGH);
                delay(1000);
                digitalWrite(4, LOW);
            }

            else if (IrReceiver.decodedIRData.command == 0x7C)
            {
                digitalWrite(5, HIGH);
                delay(1000);
                digitalWrite(5, LOW);
            }

            else if (IrReceiver.decodedIRData.command == 0x7D)
            {
                digitalWrite(6, HIGH);
                delay(1000);
                digitalWrite(6, LOW);
            }
        }

        IrReceiver.resume();
    }
}
