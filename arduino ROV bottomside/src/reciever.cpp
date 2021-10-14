#include <Arduino.h>
#include <Servo.h>

int values[4] = {0,0,0,0};
char inbytes[12];

Servo a;
Servo b;
Servo c;
Servo d;



void setup() {
  Serial.begin(9600);
  a.attach(2);
  b.attach(3);
  c.attach(4);
  d.attach(5);

  while(!Serial){

  }
}

void getDataStream(){
  int counter = 0;
  delay(20);
  if(Serial.available() == 0){
    Serial.print("r");
    while (Serial.available()  < 12){
     counter ++;
     delay(1);
     if(counter > 50){
        break;
     }
    }
    
    Serial.readBytes(inbytes, 12);

  }
  
  Serial.read();

  for(int i = 0; i <= 3; i++){
    values[i]= ((int(inbytes[i*3])-48)* 100) + ((int(inbytes[(i*3)+1])-48)*10) + (int(inbytes[(i*3)+2])-48);
    if(values[i]< 0){
      values[i] = 0;
    }
  }
}

void loop() {
  getDataStream();

  for(int i = 0; i<=3; i++){
    values[i] = map(values[i],100,900,0,180);
  }

  a.write(values[0]);
  b.write(values[1]);
  c.write(values[2]);
  d.write(values[3]);

}

