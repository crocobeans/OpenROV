#include <Arduino.h>
#include <Servo.h>

//define all the repeat use numbers
#define waitTime 500
#define packetrate 60
#define channels 8
#define packLen channels+1
#define LEDpin 2
#define minvalue 3
#define maxvalue 255
#define reqchar 2


uint8_t halfway = (maxvalue + minvalue)/2;

uint8_t values[packLen];
int mixValues[channels];

//init servos 
Servo a;
Servo b;
Servo c;

uint8_t crcGen(uint8_t inNums[packLen]);

void setup() {

  for(int i=0; i<packLen; i++){
    values[i] = halfway;
  }

  //begin serial and attach servos
  Serial.begin(115200);
  a.attach(3);
  b.attach(4);
  c.attach(5);
  pinMode(LEDpin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

  //begin timer for computation time compensation
  int t1 = millis();

  //request packet
  Serial.write(reqchar);

  bool breakstate = false;

  //hang program until packet is recieved or timeout 
  for(int i = 0; Serial.available() < packLen; i++){
    if(i > waitTime){
      breakstate = true;
      break;
    }
    delay(1);
  }

  //read data from packet into array 
  if(!breakstate){
    for(int i =0 ; i < packLen; i++){
      values[i] = Serial.read();
    }
    
    if(values[packLen-1] == crcGen(values)){
      
      //run mixes (and convert range to -128 <-> +128)
      mixValues[0] = (values[1]-halfway) - (values[0]-halfway);   
      mixValues[1] = (values[1]-halfway) + (values[0]-halfway);
      
      for(int i=2; i < channels; i++){
        mixValues[i] = values[i];
      }

      for(int i = 0 ; i < 2 ; i++){

        // convert numbers back to 3 <-> 255 range (when i tried to map negatives it broke everything)
        mixValues[i] += halfway;          
        
        //clamp numbers to range so mixes dont create negative numbers
        if(mixValues[i] > maxvalue){      
          mixValues[i] = maxvalue;
        }
        else if(mixValues[i] < minvalue ){
          mixValues[i] = minvalue;
        }

      }

      for(int i = 0; i<channels; i++){

        // map variables to within 0 - 180 range 
        mixValues[i] = map(mixValues[i],minvalue,maxvalue,0,180);         

      }

      //update motor speed
      a.write(mixValues[0]);
      b.write(mixValues[1]);
      c.write(mixValues[2]);

      //update light 
      if(mixValues[4] > 90){
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else{
        digitalWrite(LED_BUILTIN, LOW); 
      }
    }
  } //calculate delay time (if any) to maintain constant packet rate 
  int HzDelay = (1000/packetrate)-(millis()-t1);

  if(HzDelay < 0){
    HzDelay =0;
  }

  delay(HzDelay);

}

//create crc for packet
uint8_t crcGen(uint8_t inNums[packLen]){
  int total =0 ;
  for(int i =0;i<channels;i++){
    total += inNums[i];
  }
  return((total % (maxvalue-minvalue))+minvalue);
}

