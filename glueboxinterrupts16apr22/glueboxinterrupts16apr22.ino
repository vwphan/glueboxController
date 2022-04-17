#include <TM1637.h>
#include <EasyButton.h>

//
int countTime = 0;
int currentTime = 0;
const int increment = 30;
bool lamp = LOW;
bool counting = LOW;
bool lastState = LOW;
#define lampRelay  10
#define ADD_TIME  4
#define START_BUTTON  6
#define STOP_BUTTON 8
bool lastLamp = LOW;
int decrementTime;

uint8_t debounce = 15;
bool pullup = true;
bool invert = false;
TM1637 disp(2,3);
EasyButton addTime(ADD_TIME, debounce, pullup, invert);
EasyButton startButton(START_BUTTON, debounce, pullup, invert);
EasyButton stopButton(STOP_BUTTON, debounce, pullup, invert);

void addTimeISR(){
  addTime.read();
  }

void startButtonISR(){
  startButton.read();
  }

void stopButtonISR(){
  stopButton.read();
  }


void onAddTime(){
  Serial.print("add time + ");
  Serial.print(increment);
  Serial.print(" = ");
  int newTime = currentTime + increment;
  currentTime = newTime;
  disp.clearScreen();
  disp.display(currentTime);
  Serial.println(currentTime);
  }

void onStartPress(){
  Serial.println("started");
  counting = HIGH;
  lightOn();
  countTime = currentTime;
  }

void onStopPress(){
  Serial.println("stop");
  counting = LOW;
  currentTime = 0;
  lightOff();
  //disp.clearScreen();
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  disp.begin();
  disp.setBrightness(2);
  addTime.begin();
  startButton.begin();
  stopButton.begin();
  addTime.onPressed(onAddTime);
  startButton.onPressed(onStartPress);
  stopButton.onPressed(onStopPress);
  //startStop.onPressedFor(1000, onShortPress);
  //startStop.onPressedFor(3000, onLongPress);
  pinMode(lampRelay,OUTPUT);
  addTime.enableInterrupt(addTimeISR);
  startButton.enableInterrupt(startButtonISR);
  stopButton.enableInterrupt(stopButtonISR);
}

void loop() {
  // put your main code here, to run repeatedly:
  addTime.update();
  startButton.update();
  stopButton.update();
  if(countTime == 0){
    lastState = LOW;
    counting = LOW;
    lightOff();
    }
  if(countTime>0 /*&& counting == HIGH*/){
    lastState = HIGH;
    lightOn();
    decrementTime = countTime - 1;
    Serial.print("newTime: ");
    Serial.println(decrementTime);
    countTime = decrementTime;
    disp.clearScreen();
    disp.display(countTime);
    delay(1000);
    if(countTime==0){
      endCount();
      }
    }

   else{
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
  }

void endCount(){
  disp.clearScreen();
  }
