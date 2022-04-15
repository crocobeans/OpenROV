#include <Arduino.h>
#include <Servo.h>

//define all the repeat use numbers
#define waitTime 500
#define packetrate 60
#define bufferlength 4
#define LEDpin 2
#define minvalue 3
#define maxvalue 255
#define reqchar 2

//calculate half of range for faster changing of min/max values and init arrays
int halfway =(maxvalue + minvalue)/2;
int values[4] = {halfway,halfway, halfway ,halfway};
int mixValues[4] = {halfway,halfway,halfway,halfway};

//init servos 
Servo a;
Servo b;
Servo c;

void setup() {

  //begin serial and attach servos
  Serial.begin(115200);
  a.attach(3);
  b.attach(5);
  c.attach(6);
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
  for(int i = 0; Serial.available() < bufferlength; i++){
    if(i > waitTime){
      breakstate = true;
      break;
    }
    delay(1);
  }

  //read data from packet into array 
  if(!breakstate){
    for(int i =0 ; i < bufferlength; i++){
      values[i] = Serial.read();
    }
  }

  //run mixes (and convert range to -128 <-> +128)
  mixValues[0] = (values[1]-halfway) - (values[0]-halfway);   
  mixValues[1] = (values[1]-halfway) + (values[0]-halfway);
  mixValues[2] = (values[2]-halfway);
  mixValues[3] = (values[3]-halfway);

  for(int i = 0 ; i <=3 ; i++){

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

  for(int i = 0; i<=3; i++){

    // map variables to within 0 - 180 range 
    mixValues[i] = map(mixValues[i],minvalue,maxvalue,0,180);         

  }

  //update motor speed
  a.write(mixValues[0]);
  b.write(mixValues[1]);
  c.write(mixValues[2]);

  //update light 
  if(mixValues[3] > 90){
    digitalWrite(LEDpin, HIGH);
  }
  else{
    digitalWrite(LEDpin, LOW);
  }

  //calculate delay time (if any) to maintain constant packet rate 
  int HzDelay = (1000/packetrate)-(millis()-t1);

  if(HzDelay < 0){
    HzDelay =0;
  }

  delay(HzDelay);

}

