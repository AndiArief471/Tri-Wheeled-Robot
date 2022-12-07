// 1 coin = 1k
// 1 item = 3 coins
// 2 items = 5 coins
// 3 items = 7 coins

#include <NewPing.h>        // Ultra-sonic library
#include <SimpleTimer.h>    // SimpleTimer library

// Ultra-sonic used for the rotation of the motor
#define Trigger_Pin  D5 
#define Echo_Pin     D6
#define Max_Distance 250

// This is unique id and device name that only given to the author of the blynk user
#define BLYNK_TEMPLATE_ID "TMPLEgIjYgfi"
#define BLYNK_DEVICE_NAME "NodeMCU ESP8266"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial

#define APP_DEBUG

#define Motor1 D0
#define IR_Pin D1

// Virtual pin used to simulate a pin/button in the server
#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3
#define VPIN_BUTTON_4    V4 

#include "BlynkEdgent.h"

// Booleans for item quantity
bool Q1 = false;
bool Q2 = false;
bool Q3 = false;
int Item_Quantity;

NewPing sonar(Trigger_Pin, Echo_Pin, Max_Distance);

SimpleTimer timer;

int Reset = 1;          //reset rotation
int Coins;              //number of coins inputted
bool Start_Countdown = false;   //start 10 sec when first coin inserted

// synchronizing all the virtual pin
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);  
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);  
}

BLYNK_WRITE(VPIN_BUTTON_1)
{
  if(param.asInt() == 1){
    Q1 = true;
  }
  else{
    Q1 = false;
  }
}

BLYNK_WRITE(VPIN_BUTTON_2)
{
  if(param.asInt() == 1){
    Q2 = true;
  }
  else{
    Q2 = false;
  }
}

BLYNK_WRITE(VPIN_BUTTON_3)
{
  if(param.asInt() == 1){
    Q3 = true;
  }
  else{
    Q3 = false;
  }
}

BLYNK_WRITE(VPIN_BUTTON_4)
{
  if(param.asInt() == 1){
    if(Q3 == true){
      Item_Quantity = 3;
      if(Reset == 1){
        Reset = 0;
      }
    }
    else if(Q2 == true){
      Item_Quantity = 2;
      if(Reset == 1){
        Reset = 0;
      }
    }
    else if(Q1 == true){
      Item_Quantity = 1;
      if(Reset == 1){
        Reset = 0;
      }
    }
    else{
      Item_Quantity = 0;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  
  pinMode(Motor1, OUTPUT);
  pinMode(IR_Pin, INPUT);

  digitalWrite(Motor1, HIGH);

  timer.setTimeout(10000);

  delay(100);

  BlynkEdgent.begin();
}

void loop() {
  int Distance = sonar.ping_cm();

  if(digitalRead(IR_Pin) == LOW && Start_Countdown == false){
    if(Item_Quantity > 0){
      Coins += 1;
      Start_Countdown == true;
      timer.run
    }
  }
  else if(digitalRead(IR_Pin) == LOW){
    Coins += 1;
  }

  if(Item_Quantity == 1 && Coins == 3){
    digitalWrite(Motor1, LOW);
    if(Distance <= 5){
      if(Reset == 0){
        digitalWrite(Motor1, LOW);
        delay(200);
        digitalWrite(Motor1, HIGH);
        Reset = 1;
      }
    }
  }
  else if(Item_Quantity == 2 && Coins == 5){
    for(int i = 2; i > 0; i--){
      digitalWrite(Motor1, LOW);
      if(Distance <= 5){
        if(Reset == 0){
          digitalWrite(Motor1, LOW);
          delay(200);
          digitalWrite(Motor1, HIGH);
          Reset = 1;
        }
      }
    }
  }

  else if(Item_Quantity == 3 && Coins == 7){
    for(int i = 3; i > 0; i--){
      digitalWrite(Motor1, LOW);
      if(Distance <= 5){
        if(Reset == 0){
          digitalWrite(Motor1, LOW);
          delay(200);
          digitalWrite(Motor1, HIGH);
          Reset = 1;
        }
      }
    }
  }
  
  else{
    BlynkEdgent.run();
  }
}
