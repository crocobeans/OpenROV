#include <Arduino.h>

//define repeat use numbers
#define minvalue 3
#define maxvalue 255
#define reqchar 2
#define packLen 5
#define channels 4

//calculate half way point to init arrays 
uint8_t halfway =(maxvalue + minvalue)/2;

//set array to mid values
uint8_t values[packLen] = {halfway,halfway,halfway,halfway};

uint8_t crcGen(uint8_t inNums[packLen]);

void setup() {

  //start serial and init pwm read pins 
  Serial.begin(115200);
  for(int i=2; i<6; i++){
    pinMode(i, INPUT);
  }
}

void loop() {

  for(int i=0;i<channels;i++){
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

  values[packLen-1]= crcGen(values);

  while(Serial.available() > 0){

    if(Serial.peek() == reqchar){

      //send packet
      for(int i = 0; i < packLen; i++){
        Serial.write(values[i]);
      }
      //clear buffer
      while(Serial.available()>0){
        Serial.read();
      }

    }
    else{

      //clear buffer
      while(Serial.available()>0){
        Serial.read();
      }

    }
  }

}

uint8_t crcGen(uint8_t inNums[packLen]){
  int answer =0 ;
  for(int i =0;i<channels;i++){
    answer += inNums[i];
  }
  answer %= maxvalue-minvalue;
  answer += minvalue;
  return(answer);
}