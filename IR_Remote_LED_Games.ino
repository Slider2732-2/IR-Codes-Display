// InfraRed Remote Control Games 1 - The LED's
// Slider2732_
// September/October 2026

// SSD1306 screen
// Arduino Nano
// 4x colour coded LED's - change codes for them based on readings taken with the decoder software :-)


#include <Arduino.h>

#define IR_USE_AVR_TIMER1
#include <IRremote.hpp>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_HEIGHT 32
#define IR_RECEIVE_PIN 2

Adafruit_SSD1306 display(128, 32, &Wire, -1);


int selection = 1; // Game or function selection

// Replace these HEX codes with the ones for your remote
// Variable name, hex code, IR remote symbol
int FuncStart = 0x43; // On
int FuncStop = 0xD; // Off
int FuncUp = 0x8; // +
int FuncDown = 0x5A; // -
int FuncLeft = 0x7; // 3000K
int FuncRight = 0x9; // 6000K
int FuncSelect = 0x1C; // 4500K 
int Func1 = 0x46; // 1H - Red LED 
int Func2 = 0x45; // 2H - Green LED
int Func3 = 0x15; // 4H - Yellow LED
int Func4 = 0x16; // 6H - Blue LED
int Func5 = 0x44; // Time symbol - Larsen scanner button


// Some game variables
long react;
long reactOld; // in case random number is the same as the last time - avoid double trigger
int reactSet = 0;
long reactTime;
bool playing = false;
int count = 0;
int TimeEnd = 0;
int speed = 2000;


void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT); // Speaker output

    // Start OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(17, 10);
    display.print(F("IR LED's"));

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
    display.display();

    react = 999;
    randomSeed(millis());
}


void loop()
{
  // Switch off all LED's
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW); 


if (selection == 1 && playing == true) {Reaction();}
if (selection == 2 && playing == true) {Sound2Light();}
if (selection == 3 && playing == true) {LEDHit();}
if (selection == 4 && playing == true) {LEDcheck();}

if (playing == false)
{
  display.clearDisplay();
  display.setTextSize(1);  
  display.setCursor(30,0);
  display.print("< >  Select");
  display.setTextSize(2);
  display.setCursor(0,12);
  display.print(selection);
  display.setTextSize(1);
  display.setCursor(30, 19);
  if (selection == 1) {display.print("Reaction");}
  else if (selection == 2) {display.print("Sound2Light");}
  else if (selection == 3) {display.print("LED Hit");}
  else if (selection == 4) {display.print("LED & Larsen");}
  display.display();
}

  // Selection is with left and right then Select to select

    if (IrReceiver.decode())
    {
        // Ignore repeated button transmissions for the LED actions
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            
        // Print command to Serial
        Serial.print(F("Command = 0x"));
        Serial.println(IrReceiver.decodedIRData.command, HEX);

            
            if (IrReceiver.decodedIRData.command == FuncLeft)
            {
                selection --;
                if(selection < 1) {selection = 1;}
            }

            else if (IrReceiver.decodedIRData.command == FuncRight)
            {
                selection ++;
                if(selection > 4) {selection = 4;}
            }

            else if (IrReceiver.decodedIRData.command == FuncSelect)
            {
                if(selection == 1) {display.clearDisplay(); display.display(); playing = true; Reaction();}
                else if (selection == 2) {display.clearDisplay(); display.display(); playing = true; Sound2Light();}
                else if (selection == 3) {display.clearDisplay(); display.display(); playing = true; count = 0; LEDHit();}
                else if (selection == 4) {display.clearDisplay(); display.display(); playing = true; LEDcheck();}
            }

           
        }

        IrReceiver.resume();
    }
}






void Reaction()
{
 // Random LED lights, time taken to press correct key
if (react == 999)
   {
    do {
    react = random(1, 5);
} while (react == reactOld);

reactOld = react;

    Serial.println(react);
    reactSet = 1;
   }

 display.setTextSize(1);
 display.setCursor(0,0);
 display.print("Reaction Timer");
 display.setTextSize(2);
 display.display();

 

 if (react == 1 && reactSet == 1) 
 {
    digitalWrite(3, HIGH);
    reactTime = millis();
    display.setCursor(40,10);
    display.print("RED");
    display.display();
    reactSet = 0;
 }

 else if (react == 2 && reactSet == 1) 
 {
    digitalWrite(4, HIGH);
    reactTime = millis();
    display.setCursor(40,10);
    display.print("GREEN");
    display.display();   
    reactSet = 0;
  }

 else if (react == 3 && reactSet == 1) 
 {
    digitalWrite(5, HIGH);
    reactTime = millis();
    display.setCursor(30,10);
    display.print("YELLOW");
    display.display();
    reactSet = 0;
 }

 else if (react == 4 && reactSet == 1) 
 {
     digitalWrite(6, HIGH);
     reactTime = millis();
     display.setCursor(40,10);
     display.print("BLUE");
     display.display();  
     reactSet = 0;
 }



 while (!IrReceiver.decode()) {}


 if (IrReceiver.decode())
    {
        // Ignore repeated button transmissions for the LED actions
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            
        // Print command to Serial
        Serial.print(F("Command = 0x"));
        Serial.println(IrReceiver.decodedIRData.command, HEX);

            
            if (IrReceiver.decodedIRData.command == Func1)
            {
               if (react == 1) { reactStats(); }

            }

            else if (IrReceiver.decodedIRData.command == Func2)
            {
              if (react == 2) {reactStats();}
            }

           else if (IrReceiver.decodedIRData.command == Func3)
            {
              if (react == 3) {reactStats();}
            }

           else if (IrReceiver.decodedIRData.command == Func4)
            {
              if (react == 4) {reactStats();}
            }

          else if (IrReceiver.decodedIRData.command == FuncStop)
            {
            playing = false;
            TimeEnd = 0;
            react = 999;
            return;
            }
        }
        IrReceiver.resume();
    }

}

void reactStats()
{
  display.clearDisplay();
  display.display();

  // Switch off all LED's
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);

    reactTime = (millis() - reactTime);
    display.setCursor(28, 10);
    display.print(reactTime);
    display.setTextSize(1);
    display.println(" ms");
    display.display();
    tone(9, 500, 100);
    delay(1000); 
    noTone(9);
    display.clearDisplay();
    display.display();
    react = 999;
    reactSet = 0; 
    reactTime = 0;
}



void Sound2Light()
{
  // Note plays, match it to the LED colour
  // count increases by 1 for each success
  // Score is how many correct in 30 seconds

 
 // Switch off all LED's
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
 

 randomSeed(analogRead(A0));

if (reactSet == 0) 
  {
   reactTime = millis();
    
  } 

 if (react == 999)
   {
    do {
    react = random(1, 5);
} while (react == reactOld);

reactOld = react;

    reactSet = 1;
   }

// Display score
display.setTextSize(1);
display.setCursor(0,0);
display.print("Score");
display.setCursor(82,0);
display.print("Time");
display.setTextSize(2);
display.setCursor(0,14);
display.print(count);
display.setCursor(80, 14);
TimeEnd = (30000 - (millis() - reactTime));
TimeEnd = TimeEnd/1000;
if (TimeEnd <=1) {TimeEnd = 0; react = 888;}
display.print(TimeEnd);
display.display();


// Play note relating to number chosen
if (reactSet == 1)
   {
    if (react == 1) {tone(9, 100, 100);}
    else if (react == 2) {tone(9, 300, 100);}
    else if (react == 3) {tone(9, 500, 100);}
    else if (react == 4) {tone(9, 700, 100);}
   }


 while (!IrReceiver.decode()) {}


 if (IrReceiver.decode())
    {
        // Ignore repeated button transmissions for the LED actions
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            
        // Print command to Serial
        Serial.print(F("Command = 0x"));
        Serial.println(IrReceiver.decodedIRData.command, HEX);

            display.clearDisplay();
            display.display();
            
            if (IrReceiver.decodedIRData.command == Func1)
            {
               if (react == 1) {digitalWrite(3, HIGH); S2LStats();}
              
            }

            else if (IrReceiver.decodedIRData.command == Func2)
            {
              if (react == 2) {digitalWrite(4, HIGH); S2LStats();}
            }

           else if (IrReceiver.decodedIRData.command == Func3)
            {
              if (react == 3) {digitalWrite(5, HIGH); S2LStats();}
            }

           else if (IrReceiver.decodedIRData.command == Func4)
            {
              if (react == 4) {digitalWrite(6, HIGH); S2LStats();}
            }

          else if (IrReceiver.decodedIRData.command == FuncStop)
            {
            playing = false;
            count = 0;
            react = 999;
            reactSet = 0;
            return;
            }
        }
        IrReceiver.resume();
    }

}

void S2LStats()
{
  if (TimeEnd != 888)
    {
     display.setTextSize(2);
     display.setCursor(38, 14);
     display.print("YES!");
     display.display();
     delay(1000);
     display.clearDisplay();
     display.display();
     count ++;
     react = 999;
    }
}




void LEDHit()
{
  delay(100);
  noTone(9); // Reset speaker

  // Switch off all LED's
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
 
  randomSeed(millis());


  display.clearDisplay();
  display.setTextSize(1);  
  display.setCursor(38,0);
  display.print("Score");
  display.setTextSize(2);
  display.setCursor(40,14);
  display.print(count);
  display.display();


 if (react == 999)
   {
    react = random(3, 7);
    
    digitalWrite(react, HIGH);
    delay(50);
    reactTime = millis();
   }
   
       
       if ((millis() - reactTime) > speed && react != 789)
          {
            Serial.print(react);
            react = 789; // end of game
            digitalWrite(react, LOW);
            for (speed = 120; speed >= 60; speed--)
               {tone(9, speed, 180);} // end game tone
               delay(1000);
          }


     if (IrReceiver.decode())
    {
            // Ignore repeated button transmissions for the LED actions
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            if (IrReceiver.decodedIRData.command == Func1)
            {
             if (react == 3)
                { 
                  count ++;
                  react = 999;
                  tone (9, 600, 40); // win ping
                }
                  else if (react !=3)
                  {tone(9,100,40);
                   react = 777;}
            }

            else if (IrReceiver.decodedIRData.command == Func2)
            {
               if (react == 4)
                { 
                  count ++;
                  react = 999;
                  tone (9, 600, 40); // win ping
                }   
              else if (react !=4)
                 {tone(9,100,40);           
                  react = 777;}
            }

            else if (IrReceiver.decodedIRData.command == Func3)
            {
               if (react == 5)
                { 
                  count ++;
                  react = 999;
                  tone (9, 600, 40); // win ping
                }
                  else if (react !=5)
                  {tone(9,100,40);
                   react = 777;}
            }

            else if (IrReceiver.decodedIRData.command == Func4)
            {
               if (react == 6)
                { 
                  count ++;
                  react = 999;
                  tone (9, 600, 40); // win ping
                }
                  else if (react != 6)
                  {tone(9,100,40);
                   react = 777;}
            }  
                
            

            

            else if (IrReceiver.decodedIRData.command == FuncStop)
            {
              react = 999;
              reactTime = 0;
              count = 0;
              speed = 2000;
              playing = false;
              return;
            }
        }

        IrReceiver.resume(); 
    } 
}




void LEDcheck()
{
  if (IrReceiver.decode())
    {
        // Set up OLED
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0,0);
        display.display();      

     
        // Ignore repeated button transmissions for the LED actions
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            if (IrReceiver.decodedIRData.command == Func1)
            {
                digitalWrite(3, HIGH);
                display.setCursor(50,10);
                display.print("RED");
                display.display();
                delay(1000);
                digitalWrite(3, LOW);
                display.clearDisplay();
                display.display();
            }

            else if (IrReceiver.decodedIRData.command == Func2)
            {
                digitalWrite(4, HIGH);
                display.setCursor(40,10);
                display.print("GREEN");
                display.display();
                delay(1000);
                digitalWrite(4, LOW);
                display.clearDisplay();
                display.display();
            }

            else if (IrReceiver.decodedIRData.command == Func3)
            {
                digitalWrite(5, HIGH);
                display.setCursor(30,10);
                display.print("YELLOW");
                display.display();
                delay(1000);
                digitalWrite(5, LOW);
                display.clearDisplay();
                display.display();
            }

            else if (IrReceiver.decodedIRData.command == Func4)
            {
                digitalWrite(6, HIGH);
                display.setCursor(44,10);
                display.print("BLUE");
                display.display();
                delay(1000);
                digitalWrite(6, LOW);
                display.clearDisplay();
                display.display();
            }

             else if (IrReceiver.decodedIRData.command == Func5)
            {
                // Larson scanner - one run up and down
                digitalWrite(3, HIGH);
                delay(200);
                digitalWrite(3, LOW);
                digitalWrite(4, HIGH);
                delay(200);
                digitalWrite(4, LOW);
                digitalWrite(5, HIGH);
                delay(200);
                digitalWrite(5, LOW);
                digitalWrite(6, HIGH);
                delay(200);
                digitalWrite(6, LOW);
                digitalWrite(5, HIGH);
                delay(200);
                digitalWrite(5, LOW);
                digitalWrite(4, HIGH);
                delay(200);
                digitalWrite(4, LOW);
                digitalWrite(3, HIGH);
                delay(200);
                digitalWrite(3, LOW);
            }

            else if (IrReceiver.decodedIRData.command == FuncStop)
            {
              playing = false;
              return;
            }
        }

        IrReceiver.resume();
    }


    LEDcheck();
}
