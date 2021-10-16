#include <Arduino.h>
#include <Servo.h>

int values[4] = {0,0,0,0};
char inbytes[12];
bool breakstate = false; 
int mixValues[4] ;


//init servos 
Servo a;
Servo b;
Servo c;



void setup() {

  //begin serial and attach servos
  Serial.begin(57600);
  a.attach(2);
  b.attach(3);
  c.attach(4);
  pinMode(5, OUTPUT);

  //wait for serial connection
  while(!Serial){        

  }
}

//serial data function 
void getDataStream(){       

  int counter = 0;

  breakstate = false;
  
  // check if serial buffer is empty so bad data isnt loaded
  if(Serial.available() == 0){  

    // request data from topside
    Serial.print("r");           

    //wait for packet
    while (Serial.available()  < 12){   

      //timeout to scrap partial / broken packets 
      counter ++;                        

      delay(1);

      if(counter > 50){
        breakstate = true;
        break;

      }
    }

    if(breakstate == false){

      //load packet into array 
      Serial.readBytes(inbytes, 12);       

    }

    for(int i = 0 ; i<=11; i++){
      if(int(inbytes[i]) < 48 or int(inbytes[i]) > 57){
        breakstate = true;
      }
    }
  }
  
  // clear serial buffer 
  Serial.read();                        

  if(breakstate == false){

    for(int i = 0; i <= 3; i++){

      // convert ascii values into their integers and convert them into their hundreds, tens and units columns 
      values[i]= ((int(inbytes[i*3])-48)* 100) + ((int(inbytes[(i*3)+1])-48)*10) + (int(inbytes[(i*3)+2])-48);          

    }
  }

  
}

void loop() {

  //call serial function
  getDataStream();            
  if(breakstate == false){

    //run mixes (and convert range to -400 - +400)
    mixValues[0] = (values[1]-500) - (values[0]-500);   
    mixValues[1] = (values[1]-500) + (values[0]-500);
    mixValues[2] = (values[2]-500);
    mixValues[3] = (values[3]-500);

    for(int i = 0 ; i <=3 ; i++){

      // convert numbers back to 100 - 900 range (when i tried to map negatives it broke everything)
      mixValues[i] = mixValues[i] + 500;          
      
      //clamp numbers to range so mixes dont create negative numbers
      if(mixValues[i] > 900){      
        mixValues[i] = 900;
      }
      else if(mixValues[i] < 100 ){
        mixValues[i] = 100;
      }

    }

    for(int i = 0; i<=3; i++){

      // map variables to within 0 - 180 range 
      mixValues[i] = map(mixValues[i],100,900,0,180);         

    }
  }

  a.write(mixValues[0]);
  b.write(mixValues[1]);
  c.write(mixValues[2]);

  //change light setting 
  if(mixValues[3] > 90){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  }

}

