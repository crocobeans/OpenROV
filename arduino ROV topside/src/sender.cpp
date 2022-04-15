#include <Arduino.h>

//define repeat use numbers
#define minvalue 3
#define maxvalue 255
#define reqchar 2

//calculate half way point to init arrays 
int halfway =(maxvalue + minvalue)/2;

//set array to mid values
int values[4] = {halfway,halfway,halfway,halfway};

void setup() {

  //start serial and init pwm read pins 
  Serial.begin(115200);
  for(int i=2; i<6; i++){
    pinMode(i, INPUT);
  }
}

void loop() {

  while(Serial.peek() == reqchar){

    for(int i = 0; i < 4; i++){
      Serial.write(values[i]);
    }

    for(int i=0;i<4;i++){
      // get pwm values
      int premap = pulseIn(i+2, HIGH);

      //map values to 3 <-> 255 for serial transmission
      values[i] = map(premap,1000,2000,minvalue,maxvalue);

      //clamp values
      if(values[i]<minvalue){
        values[i]=minvalue;
      }
      else if(values[i]>maxvalue){
        values[i]=maxvalue;
      }
    }
  }

  //wipe serial buffer 
  do{
  Serial.read();
  }while(Serial.available()>0);

}