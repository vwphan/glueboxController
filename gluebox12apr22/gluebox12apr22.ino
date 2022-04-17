#include <TM1637.h>
#include <EasyButton.h>

//
int countTime = 0;
int currentTime = 0;
const int increment = 30;
bool lamp = LOW;
bool counting = LOW;
#define lampRelay  10
#define ADD_TIME  4
#define START_STOP  6
bool lastLamp = LOW;

uint8_t debounce = 5;
bool pullup = true;
bool invert = false;
TM1637 disp(2,3);
EasyButton addTime(ADD_TIME, debounce, pullup, invert);
EasyButton startStop(START_STOP, debounce, pullup, invert);

void onAddTime(){
  Serial.println("add time");
  int newTime = currentTime + increment;
  currentTime = newTime;
  Serial.print(newTime);
  }

void onShortPress(){
  Serial.println("started");
  counting = HIGH;
  lightOn();
  countTime = currentTime;
  }

void onLongPress(){
  Serial.println("stop");
  counting = LOW;
  currentTime = 0;
  lightOff();
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  disp.begin();
  disp.setBrightness(4);
  addTime.begin();
  startStop.begin();
  addTime.onPressed(onAddTime);
  startStop.onSequence(1, 1000, onShortPress);
  startStop.onSequence(2, 5000, onLongPress);
  
  pinMode(lampRelay,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  addTime.read();
  startStop.read();
  Serial.print("count time: ");
  Serial.print(countTime);
  Serial.print(" lamp status: ");
  if(lamp==0){Serial.print("off");}
  else if(lamp==1){Serial.print("on");}
  //Serial.print(lamp);
  Serial.println(" end.");
  if(countTime == 0){
    counting = LOW;
    lightOff();
    }
  if(countTime>0 /*&& counting == HIGH*/){
    //Serial.println(countTime);
    int newTime = countTime - 1;
    Serial.print("newTime: ");
    Serial.print(newTime);
    countTime = newTime;
    disp.display(countTime);
    delay(1000);}
/*
  if(countTime>100){
    int minDisp = (countTime/60)*100;
    Serial.println("mins: ");
    Serial.print(minDisp);
    int secDisp = countTime%60;
    Serial.print("  Sec: ");
    Serial.print(secDisp);
    int dispTime = minDisp+dispTime;
    Serial.print(" disp: ");
    Serial.print(dispTime);
    disp.display(dispTime);
    }
*/ 
   else{
    //Serial.println("off");
    delay(1000);
    }

}

void lightOn(){
  lamp = HIGH;
  digitalWrite(lampRelay,lamp);
  Serial.println("lamp on.");  
  }

void lightOff(){
  lamp = LOW;
  digitalWrite(lampRelay,lamp);
  Serial.println("lamp off.");
  currentTime=0;
  }
