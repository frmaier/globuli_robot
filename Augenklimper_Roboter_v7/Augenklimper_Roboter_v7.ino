/* 
2019-05-05: Zur veröffentlichung schön gemacht, DEBUG
2016-12-26: für freifunk 33C3 sensoren getausch, deht sich sen vorderen sensoren zu
2015-08-19: Für Spamfilterfestival ein haufen random eingefügt!
2015-08-17: int moveEyes_nachvorne(long cTime,  int value){
  //TODO nach vorne kucken
2015-08-16: wendet den kopf der nächsten person zu
            augen bewegen modden, eigene funktionen
2015-08-15: sensoren tun, 1 und 2 haben evtl. wackler

by Friederike Maier 
CC-SA 4.0 international license 
http://creativecommons.org/licenses/by-sa/4.0/
*/
//#define DEBUG_SENSORS
#define DEBUG

const int timeout = 5000; //10000 ~1,50 m


#include <Servo.h> 
#include "RunningAverage.h"

#define triggerPin 8 //Ultraschall Trigger
#define echoPin 17  //Ultraschall Echo
#define ledPin 13
#define PIRinPin 49 //Input vom PIR Sensor
#define PIRoutPin1 50 //PIR Sensor output zum Pi
#define PIRoutPin2 52 //PIR Sensor output zum Pi
#define PIRoutPin3 53 //PIR Sensor output zum Pi



Servo myservo_b1;  // Servo blinzel 1 
Servo myservo_b2;  //Servo blinzel 2
Servo myservo_a1;  // augedrehen 1
Servo myservo_a2; //auge 2
Servo myservo_d1; // Servo zum Kopf drehen


int pos = 0;   //Servo Position b1 und b2
int lastPos;
int val; //PIR Output, ob sich was bewegt
int x; //Wie oft blinzelt der Roboter
int ledStatus = LOW; // die LED ist erstmal aus
long PIRnextWakeUp;
long nextBlinzel;
long USnextWakeUp;
int wait=15; // 15 minimal wait time needed for servos
long zeit;
int minAbstand = 100;
int zeitlupe;


int eyePos1, eyePos2;
int headPos = 90;
int headPos2;
int inc, incE1, incE2, incDiff;
int posd1, posa1, posa2, diff;

const int triggerPin1 = 44; 
const int echoPin1 = 45;
const int triggerPin2 = 40; 
const int echoPin2 = 41; 
const int triggerPin3 = 36;
const int echoPin3 = 37; 
const int triggerPin4 = 28;
const int echoPin4 = 29;
const int triggerPin5 = 30;
const int echoPin5 = 31;
const int triggerPin6 = 32;
const int echoPin6 = 33;
const int triggerPin7 = 52;
const int echoPin7 = 53;
const int triggerPin8 = 48;
const int echoPin8 = 49;
const int triggerPin9 = 38;
const int echoPin9 = 39;



int maxAbstand; //Maximaler Abstand, der mit dem timeout erreicht werden kann
long abstand, abstand_US, zeit1, zeit2, zeit3, zeit4, zeit5, abstand1, abstand2, abstand3, abstand4, abstand5; 
long zeit6, zeit7, zeit8, zeit9, zeit10, abstand6, abstand7, abstand8, abstand9, abstand10; 
//int abstand[8][20]; //über 20 abstandwerte werte mitteln
int meanAbstand[8];
int sensorID,i;
int RunningAvgSize=5;
RunningAverage myRA0(RunningAvgSize);
RunningAverage myRA1(RunningAvgSize);
RunningAverage myRA2(RunningAvgSize);
RunningAverage myRA3(RunningAvgSize);
RunningAverage myRA4(RunningAvgSize);
RunningAverage myRA5(RunningAvgSize);
RunningAverage myRA6(RunningAvgSize);
RunningAverage myRA7(RunningAvgSize);
RunningAverage myRA8(RunningAvgSize);
int headPosMapping[]={155,110,60,20,350,310,270,240,200};//mappt die sensoren auf winkel

void setup() {
  #ifdef DEBUG
  Serial.print("Some debug stuff follows");
  zeitlupe=500;
  #endif
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
  myservo_b1.attach(10);  // attaches the servo on pin 9 to the servo object 
  myservo_b2.attach(11);
  myservo_a1.attach(5);
  myservo_a2.attach(6);
  myservo_d1.attach(9); 
  
  pinMode(PIRinPin, INPUT); // der Pin für den PIR-Sensor wird als Input definiert
  pinMode(PIRoutPin1, OUTPUT);
  pinMode(PIRoutPin2, OUTPUT);
  pinMode(PIRoutPin3, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  eyePos1 = 50;
  eyePos2 = 0;
  posd1=0;posa1=100; posa2=180; //0, 80
  diff = 80;
  //linkes auge: A1: 0-90, 0 in die mitte schaun
  //rechtes Auge A2: 90-180, 180 in die Mitte 
  
  inc=4; incE1=2; incE2=2;incDiff=-4;
  i=0;
   pinMode(triggerPin1, OUTPUT);
   pinMode(triggerPin2, OUTPUT);
   pinMode(triggerPin3, OUTPUT);
   pinMode(triggerPin4, OUTPUT);
   pinMode(triggerPin5, OUTPUT);
   pinMode(triggerPin6, OUTPUT);
   pinMode(triggerPin7, OUTPUT);
   pinMode(triggerPin8, OUTPUT);
   pinMode(triggerPin9, OUTPUT);
   pinMode(echoPin1, INPUT);
   pinMode(echoPin2, INPUT);
   pinMode(echoPin3, INPUT);
   pinMode(echoPin4, INPUT);
   pinMode(echoPin5, INPUT);
   pinMode(echoPin6, INPUT);
   pinMode(echoPin7, INPUT);
   pinMode(echoPin8, INPUT);
   pinMode(echoPin9, INPUT);
    
}

void loop() {
  maxAbstand= timeout/58.3;
  long currentTime = millis(); //millis() gibt die Programmlaufzeit in Millisekunden zurück
  //int val = getPIR(currentTime); //TODO: Fix PIR Sensor, dieser soll anzeigen, ob sich jemand in der Umgebung befindet und nur dann den restichen code anschmeissen
  int val =1;
  if (val == 1){
    digitalWrite(PIRoutPin1, HIGH);
  }
  else{
    digitalWrite(PIRoutPin1, LOW);
  }
  // Serial.print("********PIR: "); Serial.println(val);
  //int val =1;
  
  blinzel(currentTime,val); 
 //moveEyes_drehen(currentTime,val);
 //moveEyes_schielen(currentTime,val);
 // moveEyes_nachvorne(currentTime,val);
  
  delay(wait);
    

  
   //Abstand aller US-Sensoren in array schreiben
   //abstand[0][i]=getAbstand(currentTime, triggerPin1, echoPin1);//
   //abstand[0][i]=getAbstand(currentTime, triggerPin1, echoPin1);//
   myRA0.addValue(getAbstand(currentTime,triggerPin1, echoPin1));
   myRA1.addValue(getAbstand(currentTime,triggerPin2, echoPin2));
   myRA2.addValue(getAbstand(currentTime,triggerPin3, echoPin3));
   myRA3.addValue(getAbstand(currentTime,triggerPin4, echoPin4));
   myRA4.addValue(getAbstand(currentTime,triggerPin5, echoPin5));
   myRA5.addValue(getAbstand(currentTime,triggerPin6, echoPin6));
   myRA6.addValue(getAbstand(currentTime,triggerPin7, echoPin7));
   myRA7.addValue(getAbstand(currentTime,triggerPin8, echoPin8));
   myRA8.addValue(getAbstand(currentTime,triggerPin9, echoPin9));
   
   
    //moving average über die letzten 8(??) Werte   
  // for(int y= 0;y++;x<8)
   meanAbstand[0]=myRA0.getAverage();
   meanAbstand[1]=myRA1.getAverage();
   meanAbstand[2]=myRA2.getAverage();
   meanAbstand[3]=myRA3.getAverage();
   meanAbstand[4]=myRA4.getAverage();
   meanAbstand[5]=myRA5.getAverage();
   meanAbstand[6]=myRA6.getAverage();
   meanAbstand[7]=myRA7.getAverage();
   meanAbstand[8]=myRA8.getAverage();
   
   #ifdef DEBUG_SENSORS
     Serial.print("Average0:");Serial.println(meanAbstand[0]);
     Serial.print("Average1:");Serial.println(meanAbstand[1]);
     Serial.print("Average2:");Serial.println(meanAbstand[2]);
     Serial.print("Average3:");Serial.println(meanAbstand[3]);
     Serial.print("Average4:");Serial.println(meanAbstand[4]);
     Serial.print("Average5:");Serial.println(meanAbstand[5]);
     Serial.print("Average6:");Serial.println(meanAbstand[6]);
     Serial.print("Average7:");Serial.println(meanAbstand[7]);
     Serial.print("Average8:");Serial.println(meanAbstand[8]);
   #endif
   //sensor, der den kleinsten abstand ausgibt
   sensorID= getIndexOfMinimumValue(meanAbstand,9)+1;
   //sensorID=10;
  // Serial.print("meanabstand:");Serial.println(meanAbstand[sensorID]);
   if (meanAbstand[sensorID]==maxAbstand){
     sensorID=10;
   }
     #ifdef DEBUG
      Serial.println("-----------------------------------------");
      Serial.print("SensorID:");Serial.println(sensorID);
     #endif

   // if (headPos>=180) {   //min: 0, max: 180
   //   inc=-1;
   // }
   // if (headPos<=0) {
   //   inc=1;
   // }

//random für spamfilter
//    if (headPos>=random(120,180) ){   //min: 0, max: 180
//      inc=random(-4,-1);
//    }
//    if (headPos<=random(0,40)) {
//      inc=random(1,5);
//    }
  //}
   //sensorID=10;
   headPos = moveHead(currentTime, val, inc);
   moveEyes_random(currentTime,val);   
    if(sensorID==10){//nix da
     //inc=2;;
     if (headPos>160){
        inc=-4;
      }
      if (headPos<20){
        inc=4;
      }
      moveEyes_schielen(currentTime,val);
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    
    if(sensorID==1){
      inc=-4;
      moveEyes_drehen_nach_0(currentTime,val);
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==2){
      inc=0;
      moveEyes_drehen_nach_1(currentTime,val);
      //moveEyes_schielen(currentTime,val);
      digitalWrite(PIRoutPin2, LOW);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==3){
      inc=0;
      moveEyes_drehen_nach_2(currentTime,val);
      digitalWrite(PIRoutPin2, LOW);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==4){
      if (headPos<180){
        inc=4;
      }
      moveEyes_drehen_nach_3(currentTime,val);
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==5){
      if (headPos<180){
        inc=4;
      }
      moveEyes_drehen_nach_4(currentTime,val);
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==6){
      //inc=-2;
      if (headPos>160){
        inc=-4;
      }
      if (headPos<20){
        inc=4;
      }
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==7){
      inc=4;
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==8){
     if (headPos>160){
        inc=-4;
      }
      if (headPos<20){
        inc=4;
      }
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }
    if(sensorID==9){
      inc=-4;
      digitalWrite(PIRoutPin2, HIGH);
      digitalWrite(PIRoutPin3, HIGH);
    }

    
    #ifdef DEBUG
      Serial.print("inc:");Serial.println(inc);
      Serial.print("headpos:");Serial.println(headPos);
      Serial.print("eyePos1: "); Serial.println(eyePos1);
      Serial.print("eyePos2: "); Serial.println(eyePos1+diff);
      delay(zeitlupe);//zeitlupe
    #endif
   Serial.print("headPos: "); Serial.println(headPos);
   //Serial.print("inc: "); Serial.println(inc);
  //Serial.print("inc: "); Serial.println(inc);
  //Serial.print("eyePos2: "); Serial.println(eyePos2);
  delay(wait);
 
}


int getPIR(unsigned long cTime){
  if(cTime>PIRnextWakeUp){
    val = digitalRead(PIRinPin); 
    return val;
    PIRnextWakeUp=cTime+ 100;
  }
}
 
void blinzel(unsigned long cTime, int value){
  if (cTime>nextBlinzel){
     if (value == 1) {
      //for (x = 0; x < 3; x +=1){
      for (x = 0; x < random(1,6); x +=1){ //random für spamfilter
        for(pos = 20; pos>=4; pos-=2){                                
          myservo_b1.write((pos+30)*3);              
          myservo_b2.write(pos-2);
          delay(15);
        } 
        for(pos = 4; pos < 20; pos += 2){                                 
          myservo_b1.write((pos+30)*3);     
          myservo_b2.write(pos-2);
          delay(15);                     
        } 
      }
      nextBlinzel=cTime +random(1000,4000); //nicht zu oft blinzeln 5000
    }
   } 
}  

int getAbstand(unsigned long cTime, int tPin, int ePin){
  if(cTime>USnextWakeUp){
   digitalWrite(tPin, LOW); 
   delayMicroseconds(2); 
   digitalWrite(tPin, HIGH); 
   delayMicroseconds(10); 
   digitalWrite(tPin, LOW);
   zeit = pulseIn(ePin, HIGH, timeout); 
   abstand_US = zeit/58.3;
   if (abstand_US>maxAbstand || abstand_US==0)
     {
     abstand_US=maxAbstand;
     }
     return abstand_US;
   USnextWakeUp=cTime+60; //warte 60 ms bis zum nächsten auslesen
  }
}

int moveEye1(long cTime, int value, int incr){
    if(value ==1) { 
       posa1=posa1+incr;
       myservo_a1.write(posa1);            
     }
     return posa1; 
}
int moveEye2(long cTime,  int value, int incr){
    //if(abstand <= minAbstand && value ==1) { 
    if(value ==1) { 
      //Serial.print("dreh********* ");
      posa2=posa2+incr;
       myservo_a2.write(posa1+diff);            
     }
     return posa2; 
}
int moveEyes_drehen(long cTime,  int value){
  if(value ==1) { 
     eyePos1=eyePos1+incE1;
     myservo_a1.write(eyePos1);   
     if (eyePos1==90) {   //100
      incE1=-2;
     }
     if (eyePos1==0) {  //0
      incE1=2;
     }
     //Serial.print("eyePos1: "); Serial.println(eyePos1);
     delay(wait);
     diff=80; 
     myservo_a2.write(eyePos1+diff);   
     //Serial.print("eyePos2: "); Serial.println(eyePos1+diff); 
  }    
}

int moveEyes_drehen_nach_9(long cTime,  int value){
  if(value ==1) { 
     eyePos1=-40;
     myservo_a1.write(eyePos1);   
     delay(wait);
     diff=80; 
     myservo_a2.write(eyePos1+diff);   
     }    
}
int moveEyes_drehen_nach_0(long cTime,  int value){
  if(value ==1) { 
     eyePos1=-30;
     myservo_a1.write(eyePos1);   
     delay(wait);
     diff=80; 
     myservo_a2.write(eyePos1+diff);   
     }    
}
int moveEyes_drehen_nach_1(long cTime,  int value){
  if(value ==1) { 
     eyePos1=30;
     myservo_a1.write(eyePos1);   
     delay(wait);
     diff=80; 
     myservo_a2.write(eyePos1+diff);   
     }    
}
int moveEyes_drehen_nach_2(long cTime,  int value){
  if(value ==1) { 
     eyePos1=40;
     myservo_a1.write(eyePos1);   
     delay(wait);
     diff=100; 
     myservo_a2.write(eyePos1+diff);   
     }    
}
int moveEyes_drehen_nach_3(long cTime,  int value){
  if(value ==1) { 
     eyePos1=60;
     myservo_a1.write(eyePos1);   
     delay(wait);
     diff=80; 
     myservo_a2.write(eyePos1+diff);   
     }    
}
int moveEyes_drehen_nach_4(long cTime,  int value){
  if(value ==1) { 
     eyePos1=90;
     myservo_a1.write(eyePos1);   
       //Serial.print("eyePos1: "); Serial.println(eyePos1);
     delay(wait);
     diff=80; 
     myservo_a2.write(eyePos1+diff);   
     //Serial.print("eyePos2: "); Serial.println(eyePos1+diff); 
  }    
}


int moveEyes_random(long cTime,  int value){
 if(value ==1) { 
     eyePos1=eyePos1+incE1;
     myservo_a1.write(eyePos1);   
     if (eyePos1==90) {   //100
      incE1=random(-3,-2);
     }
     if (eyePos1==0) {  //0
      incE1=random(2,3);
     }
     //Serial.print("eyePos1: "); Serial.println(eyePos1);
     delay(wait);
     myservo_a2.write(eyePos1+diff);  
     if (diff==180) {  
      incDiff=-4;
     }
     if (diff==0) { 
      incDiff=4;
     }
     diff=diff+incDiff; 
     myservo_a2.write(eyePos1+diff);   
    // Serial.print("eyePos2: "); Serial.println(eyePos1+diff); 
    // Serial.print("diff: "); Serial.println(diff); 
  }    
}
int moveEyes_schielen(long cTime,  int value){
 if(value ==1) { 
     eyePos1=eyePos1+incE1;
     myservo_a1.write(eyePos1);   
     if (eyePos1==90) {   //100
      incE1=-2;
     }
     if (eyePos1==0) {  //0
      incE1=2;
     }
     delay(wait);
     myservo_a2.write(eyePos1+diff);  
     if (diff==180) {  
      incDiff=-4;
     }
     if (diff==0) { 
      incDiff=4;
     }
     diff=diff+incDiff; 
     myservo_a2.write(eyePos1+diff);   
    // Serial.print("eyePos2: "); Serial.println(eyePos1+diff); 
    // Serial.print("diff: "); Serial.println(diff); 
  }    
}
int moveEyes_nachvorne(long cTime,  int value){
  //TODO nach vorne kucken
 if(value ==1) { 
     eyePos1=eyePos1+incE1;
     myservo_a1.write(eyePos1);   
     if (eyePos1==46) {   //100
      incE1=-2;
     }
     if (eyePos1==44) {  //0
      incE1=2;
     }
     //Serial.print("eyePos1: "); Serial.println(eyePos1);
     delay(wait);
     myservo_a2.write(eyePos1+diff);  
     if (diff==180) {  
      incDiff=-4;
     }
     if (diff==0) { 
      incDiff=4;
     }
     diff=diff+incDiff; 
     myservo_a2.write(eyePos1+diff);   
     //Serial.print("eyePos2: "); Serial.println(eyePos1+diff); 
     //Serial.print("diff: "); Serial.println(diff); 
  }    
}

//int moveEyes_nachvorne(long cTime,  int value){
// if(value ==1) { 
//     eyePos1=eyePos1+incE1;
//     myservo_a1.write(eyePos1);   
//     if (eyePos1>52) {   //100
//      incE1=-2;
//     }
//     if (eyePos1==52) {   //100
//      incE1=0;
//     }
//     //if (eyePos1==0) {  //0
//      //incE1=2;
//     //}
//     diff=80; 
//     myservo_a2.write(eyePos1+diff);   
//     Serial.print("eyePos1: "); Serial.println(eyePos1);
//     Serial.print("eyePos2: "); Serial.println(eyePos1+diff); 
//     delay(wait);
//     
//     
//  }    
//}


int moveHead(long cTime, int value, int incr){
    //if(abstand <= minAbstand && value ==1) { 
    if(value ==1) { 
       lastPos= posd1;
       posd1=lastPos+incr;
      // Serial.print("PosD1: "); Serial.println(posd1);
       myservo_d1.write(posd1);            
     }
     return posd1; 
}

int getIndexOfMaximumValue(int* array, int size){
 int maxIndex = 0;
 int max = array[maxIndex];
 for (int i=1; i<size; i++){
   if (max<array[i]){
     max = array[i];
     maxIndex = i;
   }
 }
 return maxIndex;
}

int getIndexOfMinimumValue(int* array, int size_){
 int minIndex = 0;
 int minimum = array[minIndex];
 for (int i=1; i<size_; i++){
   if (minimum>array[i]){
     minimum = array[i];
     minIndex = i;
   }
 }
 return minIndex;
}
