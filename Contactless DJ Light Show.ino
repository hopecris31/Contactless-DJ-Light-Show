#include <SoftwareSerial.h>

/*
* Code for an Arduino system that would play music and flash lights depending on 
* proximity to an ultrasonic sensor.  Music is played by sending MIDI signals to
* a digitial audio software, which would then produce the sound.
*
*   Hope Crisafi, Matthew Bohl
*   11/17/2022
*/

//rx, tx pins. pin 5 is the transmit pin
SoftwareSerial SerialMIDI(4,5);

//motion sensor pins 
int trigPin = 11;
int echoPin = 12;

//LED pins
int green = 8; 
int blue = 9; 
int red = 6; 
int yellow = 3;

//motion sensor variables 
int duration;
int inches;

/*
*   A function that sends a MIDI note signal
*   param note: a musical note to plat
*   param velocity: note velocity; loudness
*   param duration: the duration of the note (ms)
*/
void midiNoteOn(byte note, byte velocity, int duration) { 
  SerialMIDI.write(0x90);
  SerialMIDI.write(note);
  SerialMIDI.write(velocity);
  delay(duration);
  SerialMIDI.write(0x80);
  SerialMIDI.write(note);
  SerialMIDI.write(velocity);
}

/*
* Turns all lights off
*/
void lightsOff(){
  digitalWrite(green, LOW); 
  digitalWrite(red, LOW); 
  digitalWrite(blue, LOW);
  digitalWrite(yellow, LOW);
}

/*
* Light sequence for closest sensor proximity
*/
void lightSequenceOne(){
  digitalWrite(yellow, HIGH); 
  delay(50); 
  digitalWrite(red, HIGH); 
  delay(50); 
  digitalWrite(yellow, LOW); 
  delay(50); 
  digitalWrite(blue, HIGH); 
  delay(50); 
  digitalWrite(red, LOW); 
  delay(50); 
  digitalWrite(green, HIGH); 
  delay(50); 
  digitalWrite(blue, LOW);
   delay(50);
  digitalWrite(green, LOW); 
  delay(50); 
  digitalWrite(red, HIGH); 
  delay(50);
}

/*
* Light sequence for mid-range sensor proximity
*/
void lightSequenceTwo(){
  digitalWrite(blue, HIGH); 
  delay(100); 
  digitalWrite(yellow, HIGH); 
  delay(50); 
  digitalWrite(blue, LOW); 
  delay(50); 
  digitalWrite(green, HIGH); 
  delay(50); 
  digitalWrite(yellow, LOW); 
  delay(50); 
  digitalWrite(red, HIGH); 
  delay(50); 
  digitalWrite(green, LOW); 
  delay(50); 
  digitalWrite(red, LOW);
}

/*
* Light sequence for furthest sensor proximity
*/
void lightSequenceThree(){
  digitalWrite(blue, HIGH); 
  delay(10); 
  digitalWrite(blue, LOW); 
  delay(10); 
  digitalWrite(yellow, HIGH); 
  delay(10); 
  digitalWrite(yellow, LOW);
}

void setup() { 
  Serial.begin (9600); 
  SerialMIDI.begin(31250);

  //Define inputs and outputs 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  //Define lights 
  pinMode(green, OUTPUT); 
  pinMode(blue, OUTPUT); 
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT); 
}

void loop() {
  //motion sensor initialization 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT); 
  duration = pulseIn(echoPin, HIGH);

  //convert the time into a distance 
  inches = (duration/2) / 74;

  if (inches < 10){
    midiNoteOn(60, 125, 100);

    lightSequenceOne();
  } 
  else if(inches < 20 && inches > 10){
    midiNoteOn(60, 125, 100); 
    delay(10);
    midiNoteOn(57, 125, 100);

    lightSequenceTwo();
  }
  else if(inches>20 && inches <30){   
    midiNoteOn(60, 125, 100); 
    delay(20);
    midiNoteOn(57, 125, 100); 
    delay(20);
    midiNoteOn(55, 125, 100);

    lightSequenceThree();
  }
  else{
    lightsOff();
  }
  //print for Serial Monitor
  Serial.print(inches); 
  Serial.print("in, ");  
  Serial.print(duration); 
  Serial.println();
  delay(250);

}