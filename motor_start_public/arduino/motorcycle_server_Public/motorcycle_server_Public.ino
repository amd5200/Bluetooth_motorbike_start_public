#include <SoftwareSerial.h>
#include <Wire.h>
// Android app v4.6_2018.05.20
SoftwareSerial I2CBT(10,11);//定義PIN10及PIN11分別為RX及TX腳位

int LED = 13;
int relay_key = 5;
int relay_brakes = 6;
int relay_CDI = 7;
int relay_start = 8;

int shock_sensor = 2;
int voltPin = 0;
float voltVal = 0;

int key_flag = 0;           //
int shockState = 0; 
int shock_counter = 0;

float volt = 0;            //電壓

String readwords;

//////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  I2CBT.begin(9600);//bluetooth baud rate
  pinMode(LED, OUTPUT);
  pinMode(relay_key, OUTPUT);  //設定 pin5 為輸出，key就接在這
  pinMode(relay_brakes, OUTPUT);  //設定 pin6 為輸出，brakes就接在這
  pinMode(relay_CDI, OUTPUT);  //設定 pin7 為輸出，CDI就接在這
  pinMode(relay_start, OUTPUT);  //設定 pin8 為輸出，start就接在這
  pinMode(shock_sensor, INPUT);  //設定 pin2 為輸出，shock sensor就接在這
  
      
}

///////////////////main///////////////////////////

void loop() {


  while (I2CBT.available()){  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = I2CBT.read(); //Conduct a serial read
  readwords += c; //build the string- "passward", "d", "e" and "f"
  }

  //shock detection
   shockState = digitalRead(shock_sensor);//read sensor State
   
   if (shockState == HIGH & key_flag == 0 ) {
         shock_counter = shock_counter +1 ;
         digitalWrite(LED,HIGH); 
         delay(400); 
         digitalWrite(LED,LOW);
         I2CBT.print("6 ");      //send to cell phon say "someone moving your bike"
         delay(400);   
   }
  
  if (readwords.length() > 0) {
   Serial.println(readwords);      


   
//android phone control CODE
      
   if (readwords == "aaba") {
      
                   digitalWrite(LED, HIGH);
                   delay(40);
                   digitalWrite(relay_key, HIGH);
                   delay(40);
                   digitalWrite(relay_CDI, HIGH);

                   key_flag = 1;
                   shock_counter = 0;
                   delay(300);
       
           }         
          
    else if (readwords == "bbab") 
         {       // ASCII CODE    passwd:"bbab"    
          
               digitalWrite(LED, LOW);   //熄滅LED
               delay(40);
               digitalWrite(relay_key, LOW);
               delay(40);
               digitalWrite(relay_CDI, LOW);

               key_flag = 0;
               delay(300);
                         
         }
//*
    else if (readwords == "ccbab")
         {
              auto_key_on_start();
          
         }
//*/        
    else if (readwords == "d")
         {            // "d"  start
         
              start();
              delay(10);

         }  
    else if (readwords == "e")
         {            // "e"  volt_detect
         
              volt_detect();
              delay(10);

         }      
    else if (readwords == "f")
         {            // "f"  shock_count
         
              shock_count();
              delay(10);

         }      
    
   
  
   

  
readwords="";} } //Reset the variable

//start/////////////////////////////

void start()
 {
    digitalWrite(relay_brakes, HIGH);
    delay(200);
    digitalWrite(relay_start, HIGH);
    delay(800);   
    digitalWrite(relay_brakes, LOW);
    digitalWrite(relay_start, LOW);
    delay(30); 
 }

//key ON //////////////////////////////////

void auto_key_on_start()
 {  
     digitalWrite(LED, HIGH);
     delay(40);
     digitalWrite(relay_key, HIGH);
     delay(40);
     digitalWrite(relay_CDI, HIGH);
     key_flag = 1;
     shock_counter = 0;
     delay(300);

     digitalWrite(relay_brakes, HIGH);
     delay(200);
     digitalWrite(relay_start, HIGH);
     delay(800);   
     digitalWrite(relay_brakes, LOW);
     digitalWrite(relay_start, LOW);
     delay(30); 
  
 }

//volt///////////////////////////////////////
void volt_detect()
 {
  voltVal=analogRead(voltPin);//read sensor value
  
  volt=(voltVal*5/1023)*5 ; // R=10k 5個, voltVal test 5v = 202
  
  Serial.print(volt);
  I2CBT.print(volt);
  delay(400); 
 }

////shock count
void shock_count()
 {
  
  Serial.print(shock_counter);
  I2CBT.print(shock_counter);
  delay(400); 
  
  
  
 }



