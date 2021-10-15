#include <Arduino.h>

int values[4] = {1000,1000,1000,1000};      //set array to minimum values

void setup() {
  Serial.begin(9600);

  for(int i=2; i<=5; i++){
    pinMode(i, INPUT);        //setup pwm pins 
  }

  while(!Serial){         //wait for serial connection 

  }
}

void loop() {

    for(int i=0;i<=3;i++){
    int premap = pulseIn(i+2, HIGH);              // get pwm values 
    values[i] = map(premap,980,2000,100,900);    //map values for serial transmission 
  }

  if(Serial.available() > 0){               //check if characters in serial buffer 
    int inl = Serial.read();                // read characters in serial buffer 
    
    if(inl == 114){                         // if character == ascii code for r (request character)

      for(int i=0; i <=3 ; i++){            

        if(int l = String(values[i]).length() < 3){         // add leading zeroes to values to be sent 

          Serial.print("0");

          if(l < 2){

            Serial.print("0");

          }
        }

        Serial.print(values[i]);                // send values 

      }
    }

  }

}