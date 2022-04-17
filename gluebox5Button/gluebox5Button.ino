#include <TM1637.h>
#include <EasyButton.h>

//
int countTime = 0;
int currentTime = 0;
const int increment = 30;
bool lamp = LOW;
bool counting = LOW; 
bool countChange = LOW;
bool lastState = LOW;
#define lampRelay  10
#define ADD_TIME  2
#define START_BUTTON  3
#define STOP_BUTTON 0
#define FIVE_MINS 1
#define TEN_MINS 7
bool lastLamp = LOW;
int decrementTime;

uint8_t debounce = 15;
bool pullup = true;
bool invert = false;
TM1637 disp(4,5);
EasyButton addTime(ADD_TIME, debounce, pullup, invert);
EasyButton startButton(START_BUTTON, debounce, pullup, invert);
EasyButton stopButton(STOP_BUTTON, debounce, pullup, invert);
EasyButton fiveMins(FIVE_MINS, debounce, pullup, invert);
EasyButton tenMins(TEN_MINS, debounce, pullup, invert);

void addTimeISR(){
  addTime.read();
  }

void startButtonISR(){
  startButton.read();
  }

void stopButtonISR(){
  stopButton.read();
  }

void fiveMinsISR(){
  fiveMins.read();
  }

void tenMinsISR(){
  tenMins.read();
  }

 
void onAddTime(){
  Serial.print("add time + ");
  Serial.print(increment);
  Serial.print(" = ");
  countChange = HIGH;
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
  //start buttons
  addTime.begin();
  startButton.begin();
  stopButton.begin();
  fiveMins.begin();
  tenMins.begin();
  //button actions
  addTime.onPressed(onAddTime);
  startButton.onPressed(onStartPress);
  stopButton.onPressed(onStopPress);
  fiveMins.onPressed(fiveMinsGo);
  tenMins.onPressed(tenMinsGo);
  // setup other pins
  pinMode(lampRelay,OUTPUT);
  //setup interrupts
  if(addTime.supportsInterrupt()){
    addTime.enableInterrupt(addTimeISR);
    }
  if(startButton.supportsInterrupt()){
    startButton.enableInterrupt(startButtonISR);
    }
  if(stopButton.supportsInterrupt()){
    stopButton.enableInterrupt(stopButtonISR);
    }
  if(fiveMins.supportsInterrupt()){
    fiveMins.enableInterrupt(fiveMinsISR);
    }
  if(tenMins.supportsInterrupt()){
    tenMins.enableInterrupt(tenMinsISR);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*addTime.read();
  startButton.read();
  stopButton.read();*/
  if(countTime == 0 && counting == LOW){
    return;
    }
  
  //if the timer count has changed, set the count time
  //to the current time, and reset the state
  if(countChange == HIGH){
    countTime = currentTime;
    countChange = LOW;
    }
  if(countTime == 0){
    lastState = LOW; 
    counting = LOW;
    lightOff();
    }
  if(countTime>0 && counting == HIGH){
    lastState = HIGH;
    counting = lastState;
    if(lamp == LOW){
      lightOn();
    }
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

void fiveMinsGo(){
  countTime = 60*5;
  //lightOn();
  counting = HIGH;
  }

void tenMinsGo(){
  countTime = 60*10;
  //lightOn();
  counting = HIGH;
  }

void lightOn(){
  lamp = HIGH;
  //counting = HIGH;
  lastState = HIGH;
  digitalWrite(lampRelay,lamp);
  Serial.println("lamp on."); 
  }

void lightOff(){
  lamp = LOW;
  lastState = LOW;
  digitalWrite(lampRelay,lamp);
  Serial.println("lamp off.");
  }

void endCount(){
  disp.clearScreen();
  lastState = LOW;
  counting = LOW;
  }
