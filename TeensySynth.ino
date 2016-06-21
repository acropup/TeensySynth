// Teensy 3 Audio Board - https://www.pjrc.com/store/teensy3_audio.html
#include "KeyboardSensor.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthKarplusStrong  string1;        //xy=162,50
AudioSynthKarplusStrong  string2;        //xy=162,97
AudioSynthKarplusStrong  string3;        //xy=162,144
AudioSynthKarplusStrong  string4;        //xy=162,192
AudioSynthKarplusStrong  string7;        //xy=162,336
AudioSynthKarplusStrong  string5;        //xy=163,239
AudioSynthKarplusStrong  string6;        //xy=163,286
AudioSynthKarplusStrong  string8;        //xy=163,385
AudioMixer4              mixer1;         //xy=351,173
AudioMixer4              mixer2;         //xy=352,258
AudioMixer4              mixer3;         //xy=507,211
AudioOutputI2S           i2s1;           //xy=666,212
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(string2, 0, mixer1, 1);
AudioConnection          patchCord3(string3, 0, mixer1, 2);
AudioConnection          patchCord4(string4, 0, mixer1, 3);
AudioConnection          patchCord5(string7, 0, mixer2, 2);
AudioConnection          patchCord6(string5, 0, mixer2, 0);
AudioConnection          patchCord7(string6, 0, mixer2, 1);
AudioConnection          patchCord8(string8, 0, mixer2, 3);
AudioConnection          patchCord9(mixer1, 0, mixer3, 0);
AudioConnection          patchCord10(mixer2, 0, mixer3, 1);
AudioConnection          patchCord11(mixer3, 0, i2s1, 0);
AudioConnection          patchCord12(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=373,417
// GUItool: end automatically generated code


const int pin_RingCounterClock = 20; //Teensy pin connected to 4017 Clock pin
const int pin_RingCounterReset = 21; //Teensy pin connected to 4017 Reset pin
//Teensy pins used to sense keyboard key presses in the currently illuminated OCTave
const int KeySensePins[8] = { 0, 1, 2, 3, 4, 5, 8, 16 };

void setup() {
  InitKeyboardSensor(pin_RingCounterClock, pin_RingCounterReset, KeySensePins);
  Serial.begin(9600);

  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  mixer1.gain(0, 0.12);
  mixer1.gain(1, 0.12);
  mixer1.gain(2, 0.12);
  mixer1.gain(3, 0.12);
  mixer2.gain(0, 0.12);
  mixer2.gain(1, 0.12);
  mixer2.gain(2, 0.12);
  mixer2.gain(3, 0.12);

  
  delay(1000);
  Serial.println("Hello");
  delay(1000);
  Serial.println("Hello");
  delay(1000);
  Serial.println("Hello");
}

void keyChangeEvent(int midi_note, bool keyIsDown) {
  if (keyIsDown) {
    Serial.print("note down "); Serial.println(midi_note);   
    string1.noteOn((midi_note)*20, 0.7);   
  }
  else {
    Serial.print("note up "); Serial.println(midi_note);        
  }
}

void loop() {
  senseKeys(keyChangeEvent);
}

