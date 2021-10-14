#include <Arduino.h>

int values[4] = {0,0,0,0};

void setup() {
  Serial.begin(9600);

  for(int i=2; i<=5; i++){
    pinMode(i, INPUT);
  }

  while(!Serial){
    
  }
}

void loop() {

  if(Serial.available() > 0){
    int inl = Serial.read();
    
    if(inl == 114){

      for(int i=0; i <=3 ; i++){

        if(int l = String(values[i]).length() < 3){

          Serial.print("0");

          if(l < 2){

            Serial.print("0");

          }
        }

        Serial.print(values[i]);

      }
    }

  }

  for(int i=0;i<=3;i++){
    int premap = pulseIn(i+2, HIGH);
    values[i] = map(premap,1000,2000,100,900);
  }

}