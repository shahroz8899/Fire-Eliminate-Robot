#include <Servo.h>

#include<SoftwareSerial.h>
SoftwareSerial gsm(11,12); //rx,tx

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


  Serial.begin(9600);
  pinMode(Left_S, INPUT);
  pinMode(Right_S, INPUT);
  pinMode(Forward_S, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(spary, OUTPUT);
  pinMode (fan, OUTPUT);
 
 // myservo.attach(11);
 // myservo.write(90); 
   
}

void AlertSMS()
{
  
  Serial.begin(9600);
  gsm.begin(9600);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+923114095878\"\r"); //replace x by your number
  delay(1000);
  gsm.println("Alert!!!! Fire in the house");
  gsm.println("http://maps.google.com/maps");
  
  delay(100);
  gsm.println((char)26);
  delay(1000);
  
  
  } 

void SafeSMS()
{
  
  Serial.begin(9600);
  gsm.begin(9600);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+923114095878\"\r"); //replace x by your number
  delay(1000);
  gsm.println("Fire Fxtinguish Successfully");

  
  delay(100);
  gsm.println((char)26);
  delay(1000);
  
  
  } 
  
void put_off_fire()
{
    delay (500);
 
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    
  AlertSMS();
    
   digitalWrite(spary, HIGH);
   delay(1000);

    digitalWrite(spary,LOW);
 delay(2000);

  digitalWrite(fan, HIGH);
   delay(3000);

    digitalWrite(fan,LOW);
    
delay(500);
 
SafeSMS();

    
  /*  for (pos = 70; pos < 180; pos += 1) { 
    myservo.write(pos); 
    delay(5);  
  }

  delay(1000);
  for (pos = 180; pos >= 70; pos -= 1) { 
    
    myservo.write(pos); 
    delay(1);
  }
  
  //digitalWrite(pump,LOW);
  myservo.write(90);
*/  
  fire=false;
}
 
void loop()
{
  // myservo.write(90); //Sweep_Servo();  
 
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
