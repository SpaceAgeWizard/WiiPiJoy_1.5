/* WiiPi Controls
   Adapted from: Complete USB Joystick Example
   Teensy becomes a USB joystick with 20 buttons and 4 axis input
   
   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons/digital input: digital 0-15, 18, 19, 22, 23
   Potentiometers/analog input: analog 0, 1, 4, 5
*/

const int numButtons = 20;  //digital 0-15, 18, 19, 22, 23

void setup() {
  // Begin serial communication
  Serial.begin(9600);
  // configure the joystick to manual send mode.  This gives precise
  // control over when the computer receives updates, but it does
  // require you to manually call Joystick.send_now().
  Joystick.useManualSend(true);
  //use pullup resistors, only for digital buttons
  for (int i=0; i<16; i++) { //digital 0-15 are used for buttons
    pinMode(i, INPUT_PULLUP);
  }
  //non-contiguous pin usage (AKA I wired it like an idiot)
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
}

byte allButtons[numButtons];
byte indexButtonPins[numButtons]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,18,19,22,23}; //array for pinout

void loop() {
  //read 4 analog inputs and use them for the joystick axis
  Joystick.X(analogRead(5)); //Left stick horiz
  Joystick.Y(1023-analogRead(4)); //Left stick vert
  Joystick.Z(analogRead(1)); //Right stick vert
  Joystick.Zrotate(analogRead(0)); //Right stick horiz
  
  //read digital pins and use them for the buttons
  for (int i=0; i<numButtons; i++) {
    if (digitalRead(indexButtonPins[i])) { //access target pin via pinout array
      //when a pin reads high, the button is not pressed
      //the pullup resistor creates the "on" signal
      allButtons[i] = 0;
    } else {
      //when a pin reads low, the button is connecting to ground.
      allButtons[i] = 1;
    }
    Joystick.button(i + 1, allButtons[i]);
  }
  
  //transmits everything all at once.
  Joystick.send_now();
  //a brief delay, runs 100 times per second
  delay(10);
}
