#include <Arduino.h>

//define repeat use numbers
#define minvalue 3
#define maxvalue 255
#define reqchar 2
#define channels 8
#define packLen channels+1

//calculate half way point to init arrays 
uint16_t halfway =(maxvalue + minvalue)/2;

//set array to mid values
uint16_t values[packLen];

uint16_t crcGen(uint16_t inNums[packLen]);

void setup() {

  for(int i=0;i<packLen;i++){
  values[i] = halfway;
  }

  //start serial and init pwm read pins 
  Serial.begin(115200);

  pinMode(2, INPUT);
  pinMode(LED_BUILTIN,OUTPUT);

}

void loop() {

  //wait for ppm sync pulse
  while(pulseIn(2,LOW,1000) != 0){

  }
  //get ppm values
  for(int i=0;i<channels;i++){
    values[i] = pulseIn(2,HIGH);
  }

  for(int i=0;i<channels;i++){

    //map values to 3 <-> 255 for serial transmission
    values[i] = map(values[i],590,1610,minvalue,maxvalue);

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

//create crc
uint16_t crcGen(uint16_t inNums[packLen]){
  int total =0 ;
  for(int i =0;i<channels;i++){
    total += inNums[i];
  }
  return((total % (maxvalue-minvalue))+minvalue);
}