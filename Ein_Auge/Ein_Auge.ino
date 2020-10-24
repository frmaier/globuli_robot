
/*
** Ein Auge, Servo+ US-Sensor
** blinzelt schneller je Näher man kommt.
* 36C3 Version: Filter Nullen und die nachfolgende Zahl aus 'distance' raus, irgendwelche komischen US Signale in der Luft hier... 
 */
#include <Servo.h>

// US sensor pin definitions:
int pingPin = 8; // trigger pin in the US sensor
int pongPin = 9; // data pin in the US sensor

long unsigned soundSpeed; // approx 340000 mm/s -> long is ok
long unsigned distance; // array with the readings of the repeated measurements, mm; precision of the readings is not beyond mm, so further digits are just false precision;
                            // long because of the operations involved.
//int AT[2]={1,100}; // time interval between consecutive readings, milliseconds
int pos = 0;    // variable to store the servo position
int pos_closed=0;//8;
int pos_open =130;
int speed_1=3; //delay wird unten eingestellt
int speed_e=5; 
int del=1; //adding delay for debugging
int maxdist=1000;
int mindist=10;
int meddist=300;
int i=0; //interferer detected 
int x=0;

Servo myservo;
//
// Setup
//
void setup() 
{
  Serial.begin(9600); 
  soundSpeed = 343200; // mm/s; source: wikipedia

  pinMode(pingPin, OUTPUT);
  pinMode(pongPin, INPUT);
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object


}

//
// Loop
//
void loop()
{
    distance = soundSpeed/100; // speed is in mm/s, duration in microseconds: 6 zeroes need to be removed, 2 go out here
    distance = distance*durCalc(pingPin, pongPin); // distance (mm) = time (total sound travelling distance, microseconds) * speed (mm/s)
    distance = distance/20000; // I remove the remaing 4 zeros here, plus the effect of two-way travel of the sound; this care with the zeros is because of size limitations of long 
    Serial.println("distance"); 
    Serial.println(distance);
    
    delay(10);
    if (distance==0)
       {
        i=1;
        x=5;
       }
    //Serial.println("----------I:"); Serial.println(i);  
    
    if(distance>maxdist | distance <mindist | i==1)
      { pos=pos_closed;
        myservo.write(pos);             
            delay(speed_1+del);  
            i=0;
      }
    if (x==5)
    {
      delay(100);
      x=x-1;

    }
       //Serial.println("xxxxxxxxxX:");
       //Serial.println(x);
      if (distance<meddist && distance >mindist && x<2) {
        speed_1=10;
        speed_e=7;
  
          for (pos = pos_open; pos >= pos_closed; pos -= speed_e) { 
            myservo.write(pos);              
            delay(speed_1);                      
          }
          for (pos = pos_closed; pos <= pos_open; pos += speed_e) { 
            myservo.write(pos);              
            delay(speed_1);                      
          }
          for (pos = pos_open; pos >= pos_closed; pos -= speed_e) { 
            myservo.write(pos);             
            delay(speed_1);                       
          }
           for (pos = pos_closed; pos <= pos_open; pos += speed_e) { 
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(speed_1+del);                       // waits speed_1 ms for the servo to reach the position
          }
      // ready for next loop:
      i=0;
      x=x-1;
      delay(10+distance*2);
      }
      
      if (distance<maxdist && distance >meddist && x<2) {
        speed_1=12;
        speed_e=6;
          for (pos = pos_open; pos >= pos_closed; pos -= speed_e) { 
            myservo.write(pos);              
            delay(speed_1);                       
          }
          for (pos = pos_closed; pos <= pos_open; pos += speed_e) { 
            myservo.write(pos);              
            delay(speed_1);                       
          }
          for (pos = pos_open; pos >= pos_closed; pos -= speed_e) { 
            myservo.write(pos);              
            delay(speed_1);                      
          }
          for (pos = pos_closed; pos <= pos_open; pos += speed_e) { 
            myservo.write(pos);             
            delay(speed_1+del);                      
          }
      // ready for next loop:
      delay(200+distance*3);
      i=0;
     }
     x=x-1;

}

//
// Functions
//
unsigned long durCalc(int pinI, int pinO)
{
  // The PING is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(pinI, LOW);
  delayMicroseconds(2);
  digitalWrite(pinI, HIGH);
  delayMicroseconds(12);
  digitalWrite(pinI, LOW);
  //delay(10);
  // The pongPin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  return(pulseIn(pinO, HIGH, 35000)); // microseconds of (total) sound travel;
}
