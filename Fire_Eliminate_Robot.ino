#include <Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial gsm(11,12); //rx,tx
String state,timegps,latitude,longitude;
#define DEBUG true
String data[5];

Servo myservo;
 
int pos = 0;    
boolean fire = false;
 
/*-------defining Inputs------*/
#define Left_S 9      // left sensor
#define Right_S 10      // right sensor
#define Forward_S 8 //forward sensor
 
/*-------defining Outputs------*/
#define LM1 2       // left motor
#define LM2 3       // left motor
#define RM1 4       // right motor
#define RM2 5       // right motor
#define spary 6     // spary
#define fan 7       //fan
 
void setup()
{


  
  pinMode(Left_S, INPUT);
  pinMode(Right_S, INPUT);
  pinMode(Forward_S, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(spary, OUTPUT);
  pinMode (fan, OUTPUT);

  gsm.begin(9600);
 Serial.begin(9600);
 delay(50);

 delay(100);
 gsm.print("AT+CMGF=1\r"); 
 delay(400);

 sendData("AT+CGNSPWR=1",1000,DEBUG);
 delay(50);
 sendData("AT+CGNSSEQ=RMC",1000,DEBUG);
 delay(150);

 
 // myservo.attach(11);
 // myservo.write(90); 
   
}

void AlertSMS()
{
  
  sendTabData("AT+CGNSINF",1000,DEBUG);
  
    Serial.println("State  :"+state);
    Serial.println("Time  :"+timegps);
    Serial.println("Latitude  :"+latitude);
    Serial.println("Longitude  :"+longitude);

   
  gsm.println("AT+CMGF=1");
  delay(1000);

  gsm.println("AT+CMGS=\"+923114095878\"\r"); //replace x by your number
  delay(1000);
  gsm.println("Alert!!! Fire Detected At");

gsm.print("http://maps.google.com/maps?q=loc:");
gsm.print(latitude);
gsm.print(",");
gsm.print (longitude);

   Serial.println("SMS sent");

  delay(100);
  gsm.println((char)26);
  delay(1000);

  gsm.println();
//delay(20000);
//gsm.flush();

  
  
  } 

void SafeSMS()
{
  sendTabData("AT+CGNSINF",1000,DEBUG);
  
    Serial.println("State  :"+state);
    Serial.println("Time  :"+timegps);
    Serial.println("Latitude  :"+latitude);
    Serial.println("Longitude  :"+longitude);

   
  gsm.println("AT+CMGF=1");
  delay(1000);

  gsm.println("AT+CMGS=\"+923114095878\"\r"); //replace x by your number
  delay(1000);
  gsm.println("Fire Extinguished Successfully");

gsm.print("http://maps.google.com/maps?q=loc:");
gsm.print(latitude);
gsm.print(",");
gsm.print (longitude);

   Serial.println("SMS sent");

  delay(100);
  gsm.println((char)26);
  delay(1000);

  gsm.println();
//delay(2000);
//gsm.flush();

  
  
  
  } 



  void sendTabData(String command , const int timeout , boolean debug){

  gsm.println(command);
  long int time = millis();
  int i = 0;

  while((time+timeout) > millis()){
    while(gsm.available()){
      char c = gsm.read();
      if (c != ',') {
         data[i] +=c;
         delay(100);
      } else {
        i++;  
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }exitL:
  if (debug) {
    state = data[1];
    timegps = data[2];
    latitude = data[3];
    longitude =data[4];  
  }
}
String sendData (String command , const int timeout ,boolean debug){
  String response = "";
  gsm.println(command);
  long int time = millis();
  int i = 0;

  while ( (time+timeout ) > millis()){
    while (gsm.available()){
      char c = gsm.read();
      response +=c;
    }
  }
  if (debug) {
     Serial.print(response);
     }
     return response;
}




  
void put_off_fire()
{
    delay (500);
  
  
  
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
   
    delay (1000);
   digitalWrite(spary, HIGH);
   delay(1000);

    digitalWrite(spary,LOW);
 delay(2000);

AlertSMS();

  digitalWrite(fan, HIGH);
   delay(3000);

    digitalWrite(fan,LOW);
    
delay(500);
 
SafeSMS();

    
  
  fire=false;
}
 
void loop()
{
    
 
    if (digitalRead(Left_S) ==1 && digitalRead(Right_S)==1 && digitalRead(Forward_S) ==1) //If Fire not detected all sensors are zero
    {
      
    //Do not move the robot
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    digitalWrite(spary,LOW);
    digitalWrite(fan,LOW);
    
    }
    
    else if (digitalRead(Forward_S) ==0) //If Fire is straight ahead
    {



      
      
    //Move the robot forward
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);

   
    fire = true;
    }
    
    else if (digitalRead(Left_S) ==0) //If Fire is to the left
    {


 
    
   
    //Move the robot left
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    
 

    }
    
    else if (digitalRead(Right_S) ==0) //If Fire is to the right
    {





      
    //Move the robot right
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);


    }
    
delay(300); //Slow down the speed of robot
 
     while (fire == true)
     {
        //AlertSMS();
      
      put_off_fire();
     }
}
