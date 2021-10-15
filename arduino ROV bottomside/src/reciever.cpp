#include <Arduino.h>
#include <Servo.h>

int values[4] = {0,0,0,0};
char inbytes[12];

//init servos 
Servo a;
Servo b;
Servo c;
Servo d;



void setup() {
  //begin serial and attach servos
  Serial.begin(57600);
  a.attach(2);
  b.attach(3);
  c.attach(4);
  d.attach(5);

  while(!Serial){        //wait for serial connection

  }
}

void getDataStream(){       //serial data function 

  int counter = 0;

  delay(5);                // needed to allow all of the data in a packet be recieved

  if(Serial.available() == 0){  // check if serial buffer is empty

    Serial.print("r");          // request data from topside 

    while (Serial.available()  < 12){   //wait until whole packet has been sent 

     counter ++;                        //code to break while if whole packet not recieved 

     delay(1);

     if(counter > 50){

        break;

     }
    }
    
    Serial.readBytes(inbytes, 12);        //load packet into array 

  }
  
  Serial.read();                        //make sure serial buffer is cleared

  for(int i = 0; i <= 3; i++){

    values[i]= ((int(inbytes[i*3])-48)* 100) + ((int(inbytes[(i*3)+1])-48)*10) + (int(inbytes[(i*3)+2])-48);          // math to convert characters into their int values

    if(values[i]< 0){

      values[i] = 0;                //if value is negative (never should be in any cases) reset it to 0 

    }
  }
}

void loop() {

  getDataStream();            //call serial function

  for(int i = 0; i<=3; i++){

    values[i] = map(values[i],100,900,0,180);         // map variables to within 0 - 180 range 

  }

//set pwm outputs to variables (need to add mixers etc)
  a.write(values[0]);
  b.write(values[1]);
  c.write(values[2]);
  d.write(values[3]);

}

