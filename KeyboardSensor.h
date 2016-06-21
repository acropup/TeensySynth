
//  44-key keyboard, split into OCTaves as the keyboard circuit was designed
//  I'm referring to OCTaves as a set of 8 adjacent keys, not a musical octave (which is 12 keys).
//  _______________________________________________________________________________________________________
// |  | | |A| | |  |  | | | |  |  | | | | | |  |  | | | |  |  | | |A| | |  |  | | | |  |  | | | | | |  |   | 
// |  | | |b| | |  |  | | | |  |  | | | | | |  |  | | | |  |  | | |b| | |  |  | | | |  |  | | | | | |  |   | 
// |  | | |2| | |  |  | | | |  |  | | | | | |  |  | | | |  |  | | |4| | |  |  | | | |  |  | | | | | |  |   | 
// |  |_| |_| |_|  |  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |  |_| |_| |_|  |   |
// | F |   |   |   |   |   | E |   |   |   |   | C |   |   |   |   |   |   |   |   | E |   |   |   |   | C |
// | 1 |   |   |   |   |   | 2 |   |   |   |   | 3 |   |   |   |   |   |   |   |   | 4 |   |   |   |   | 5 |
// |___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|
//        |                 |                  |                  |                 |                  |
//  OCT0  | OCT1            | OCT2             | OCT3             | OCT4            | OCT5             | OCT6
const int MiddleC = 60;
int OCT3_Midi_Note = MiddleC;

// 74HC4017 ring counter
// http://www.doctronics.co.uk/4017.htm
// http://pdf1.alldatasheet.com/datasheet-pdf/view/221933/NXP/74HC4017DB/+Q2Q9W5VlUOaYtKhvBN.Ll+/datasheet.pdf

//      4017 Ring Counter Pinout
//              _______
// [OCT6]  o5 -|o      |- Vcc    [3.3V]
// [OCT5]  o1 -|       |- Reset  [Teensy D21]
// [OCT4]  o0 -|   4   |- Clock  [Teensy D20]
// [OCT3]  o2 -|   0   |- ClockEnable
// [OCT2]  o6 -|   1   |- %10
// [OCT1]  o7 -|   7   |- o9
// [OCT0]  o3 -|       |- o4
//        Gnd -|_______|- o8

//Ring Counter illuminates one OCTave at a time, but 4017 pinout means they are not done in order.
//RingCounterStateToOCTave lists the order in which each OCTave is illuminated.
const int RingCounterStateToOCTave[] = { 4, 5, 3, 0, 8, 6, 2, 1, 7, 9 };

//Note: Microcontroller pins must be set and initialized using function InitKeybaordSensor()
int clockPin; //Microcontroller pin connected to 4017 Clock pin
int resetPin; //Microcontroller pin connected to 4017 Reset pin
//Microcontroller pins used to sense keyboard key presses in the currently illuminated OCTave
int keySensePins[8];

const int numRingCounterStates = 10; //Must be <= 10 for 4017-series chips
uint8_t keyStates[numRingCounterStates] = { 0 }; 
int ringCounterState = 0;

// Initializes the keyboard sensor code. Must be called upon startup.
// ringCounterClockPin is the microcontroller pin that is connected to the 4017 clock pin.
// ringCounterResetPin is the microcontroller pin that is connected to the 4017 reset pin.
// keyboardSensePins must be an array of 8 integers, for each of the microcontroller pins used to sense keypresses.
void InitKeyboardSensor(int ringCounterClockPin, int ringCounterResetPin, const int keyboardSensePins[]) {
  clockPin = ringCounterClockPin;
  pinMode(clockPin, OUTPUT);
  resetPin = ringCounterResetPin;
  pinMode(resetPin, OUTPUT);
  for (int i = 0; i < 8; i++) {
    keySensePins[i] = keyboardSensePins[i];
    pinMode(keySensePins[i], INPUT);
  }
}

// Send a pulse to, for example, clock the ring counter
void sendPulse(int clockPin) {
  digitalWrite(clockPin, HIGH);
  delayMicroseconds(10); //TODO: figure out how much delay is actually necessary
  digitalWrite(clockPin, LOW);
  delayMicroseconds(10);
}

void shiftRingCounter() {
  ringCounterState++;
  if (ringCounterState < numRingCounterStates) {
    sendPulse(clockPin);
  }
  else {
    sendPulse(resetPin);
    ringCounterState = 0;
  }
}

int getMidiNote(int baseOCTave, int keyOffset) {
  int midi_note = OCT3_Midi_Note + 8 * (baseOCTave - 3) + keyOffset;
  if (midi_note > 20 && midi_note <= 108) {
    return midi_note;
  }
  Serial.print("Invalid MIDI note: "); Serial.println(midi_note);
  return 60; //TODO: we should do something smarter than play Middle C when there's an invalid note
}

void senseKeys(void (*onKeyChange)(int midi_note, bool keyIsDown)) {
  do { //Loop through each OCTave
    shiftRingCounter(); //Shift to illuminate next OCTave
    int OCTave = RingCounterStateToOCTave[ringCounterState];
    uint8_t oldOCTaveKeyState = keyStates[OCTave];
    //For the currently illuminated OCTave, check all 8 keys to see which are pressed
    for (int i = 0; i < 8; i++) {
      bool keyWasDown = oldOCTaveKeyState & _BV(i);
      bool keyIsDown = digitalRead(keySensePins[i]);
      //Check if key was pressed or released
      if (keyWasDown != keyIsDown) {
        //Update the key state
        keyStates[OCTave] ^= _BV(i);
        int midi_note = getMidiNote(OCTave, i);
        
        onKeyChange(midi_note, keyIsDown);
//      Serial.print("o"); Serial.print(OCTave);
//      Serial.print("k"); Serial.println(i);
      }
    }
  } while (ringCounterState != 0);
}

