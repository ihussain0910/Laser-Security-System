//assign inputs and outputs to pins

int ledPins[] = {5,10,13}; //assigns LED Pins for the alarm as the digital pin they are in 
int button = 2;
int laser = 6;
int alarmStatus;//variable to store the alarm status 0 being off, 1 being on and 2 is triggered
int buttonStatus;//variable to store whether the button has been pressed or not 0 for it hasnt been pressed and one for it has been pressed 
int LDR = A1;//analogue inout to read the values of the LDR
int val = 0;
int photoTran = A2; //analouge input to read the values of the phototransistor 
int lightVal = 0;
int piezo = 8; 
void alarmTriggered();//function to check whether the alarm has been triggered 
void checkLight();// function to check whether or not the light has already been checked
void setup() { //setting the variables as inputs and outputs in the setup 
  pinMode(LDR, INPUT); //LDR as an input
  pinMode(photoTran,INPUT);//Phototransistor as an input
  Serial.begin(9600);
  pinMode(button,INPUT_PULLUP); //button as an input
  pinMode(ledPins, OUTPUT);//LEDS as an output
  pinMode(laser, OUTPUT);//laser as an output
  attachInterrupt(digitalPinToInterrupt(2), checkStatus, FALLING);//assigning an interrupt to the button which runs the checkStatus function when it is pressed
  alarmStatus = 0;//setting the alarm to off
  buttonStatus = 0;//setting the button to not being pressed
}

void loop() {//code inside this function will be repeated in a loop
  if (buttonStatus != 1){//if the button has not been pressed to turn the alarm on 
  checkLight();//checkLight function to decide whether to turn the alarm on depending on the amount of light the phototransistor is recieving 
  }
  if (alarmStatus == 0){//if the alarm is off or has been turned off
  digitalWrite(ledPins[0],LOW);//set all LEDs to low to turn them on 
  digitalWrite(ledPins[1],LOW); 
  digitalWrite(ledPins[2],LOW);
  digitalWrite(laser,LOW);//turn the laser off
  noTone(piezo);//stop the sound coming out of the piezo 


  }
  else if (alarmStatus == 1){ // if the alarm is on/intialised
  digitalWrite(ledPins[0],HIGH);//turn the green LED on to indicate it is on 
  digitalWrite(laser,HIGH);//turn the Laser on
  delay(100);//set a delay so the alarm does not go off straight away
  alarmTriggered();//check whether the alarm has been triggered
  }
  else if (alarmStatus == 2){//if the alarm is triggered
  digitalWrite(ledPins[0],LOW);//set the on LED (green) to off
  for(int f=635;f<=912;f++){//loop to create an alarm sound
    tone(piezo, f);//output a tone on the piezo
    digitalWrite(ledPins[2],HIGH);//set the blue led to on 
    digitalWrite(ledPins[1],HIGH);//set the white led to on
    delay(7);//set a delay to create a siren effect
  }
  digitalWrite(ledPins[2],LOW);//Turn both LEDs off
  digitalWrite(ledPins[1],LOW);
   
}
}
void alarmTriggered(){//function to  see when the alarm is triggered
  if (analogRead(LDR) < 600 && alarmStatus == 1){//if the LDR is recieveing less light than the threshold
      alarmStatus = 2;
  }
}

void checkLight(){ // function to check if the light in the room is off or not
  if ((analogRead(photoTran) < 5) && alarmStatus != 1 && alarmStatus !=2){//if the light is below the threshold value and the alarm is not on or triggered
    alarmStatus = 1;//set the alarm to on so the laser is on
  }
}
void checkStatus(){//function to check the alarm status and to act accordingly when the button is pressed
  delay(500);//I have added a delay to reduce debouncing when pressing the button
  if (alarmStatus == 0){//if the alarm status is off
  alarmStatus = 1;//initialise/turn the alarm on  
  buttonStatus = 0;//set the button status to 0 this is so that if the alarm is turned on due to lack of light it is able to be turned and will not be turned on again
  //I will talk more about this in my report 

  }
  else if (alarmStatus == 2){//if the alarm is triggered
  alarmStatus = 0;//turn the alarm off
  buttonStatus = 1;//set the button status to one

  }
  else if (alarmStatus == 1){//if the alarm is initiaised/on but not triggered 
  alarmStatus = 0;//turn the alarm off
  }

}
