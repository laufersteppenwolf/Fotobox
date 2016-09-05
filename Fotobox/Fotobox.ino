int pinButtonCapture = 2;
int pinButtonFlash = 4;
int pinOutLED1 = 6;
int pinOutLED2 = 8;
int pinOutLED3 = 10;
int pinOutLED4 = 12;
int pinOutFlash = 5;
int pinOutFocus = 11;
int pinOutCapture = 9;
int pinPoti = A6;

#define TIME_UNTIL_CAPTURE_MS 4000
#define TIME_KEEP_CAPTURE_PRESSED_MS 200

int led = 13;        //Internal LED pin
int brightness = 25; //Initial brightness in %
int wait = 500;      //fade-delay in Microseconds

//Internal variables
int brVal = brightness;
int prevB = brVal;

void setup() {
  pinMode(pinButtonCapture, INPUT_PULLUP);
  pinMode(pinButtonFlash, INPUT_PULLUP);
  pinMode(pinOutLED1, OUTPUT);
  pinMode(pinOutLED2, OUTPUT);
  pinMode(pinOutLED3, OUTPUT);
  pinMode(pinOutLED4, OUTPUT);
  pinMode(pinOutFlash, OUTPUT);
  pinMode(pinOutFocus, OUTPUT);
  pinMode(pinOutCapture, OUTPUT);
  pinMode(led, OUTPUT);

  //Reset Relais
  digitalWrite(pinOutFocus, true);
  digitalWrite(pinOutCapture, true);
  
  Serial.begin(9600);

}


void loop() {
  if (!digitalRead(pinButtonFlash)){
    crossFade(brightness);
  } else {
    crossFade(0);
  }

  if(!digitalRead(pinButtonCapture)) {
    if (!digitalRead(pinButtonFlash)){
      crossFade(getFlashBrightness());
      Serial.print("Flash requested with value: ");
      Serial.println(getFlashBrightness());
    }
    digitalWrite(pinOutLED1, true);
    Serial.println("LED1 ON");
    
    delay(TIME_UNTIL_CAPTURE_MS / 4);
    digitalWrite(pinOutLED2, true);
    Serial.println("LED2 ON");
    
    delay(TIME_UNTIL_CAPTURE_MS / 4);
    digitalWrite(pinOutLED3, true);
    Serial.println("LED3 ON");
    
    delay(TIME_UNTIL_CAPTURE_MS / 4);
    digitalWrite(pinOutLED4, true);
    Serial.println("LED4 ON");
    digitalWrite(pinOutFocus, false); //FOCUS!!
    Serial.println("FOCUS!!");
    
    delay(TIME_UNTIL_CAPTURE_MS / 4);
    digitalWrite(pinOutCapture, false);
    Serial.println("CAPTURE!!");
    
    delay(TIME_KEEP_CAPTURE_PRESSED_MS);
    digitalWrite(pinOutFocus, true);
    digitalWrite(pinOutCapture, true);
    digitalWrite(pinOutLED1, false);
    digitalWrite(pinOutLED2, false);
    digitalWrite(pinOutLED3, false);
    digitalWrite(pinOutLED4, false);
    Serial.println("RESET!!");
  }
  
}


/**
 * Math... DO NOT TOUCH!
 */
int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

void crossFade(int brightness) {
  // Convert to 0-255
  int B = (brightness * 255) / 100;

  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    brVal = calculateVal(stepB, brVal, i);

    //analogWrite(led, brVal);
    analogWrite(pinOutFlash, brVal);
    //Serial.println(brVal);

    delayMicroseconds(wait); // Pause for 'wait' microseconds before resuming the loop
  }
  // Update current values for next loop
  prevB = brVal; 
}

int getFlashBrightness() {
  int brightnessFlashReq = analogRead(pinPoti) / 10.2;
  return brightnessFlashReq;
}

