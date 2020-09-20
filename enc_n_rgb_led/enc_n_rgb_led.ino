#include <Encoder.h>
#include <Bounce2.h>

#define redLED 11
#define greenLED 10
#define blueLED 9

Encoder enc(7,8);
Bounce enc_button = Bounce(12, 5); // pin 12, 5ms

byte R, G, B;
typedef enum {
  changeRED,
  changeGREEN,
  changeBLUE
} activeColor_t;

activeColor_t active = changeRED;
long pos = -999;
long millisNow;

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redLED, redValue);
  analogWrite(greenLED, greenValue);
  analogWrite(blueLED, blueValue);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Encoder test");
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  R = G = B  = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  long newPos;
  newPos = enc.read();
  if (newPos != pos) {
    if (active == changeRED) {
      if ((newPos > pos) && (R < 255)) {
        R += 1;
      }
      else if ((newPos < pos) && (R > 0)) {
        R -= 1;
      }
    }
    if (active == changeGREEN) {
      if ((newPos > pos) && (G < 255)) {
        G += 1;
      }
      else if ((newPos < pos) && (G > 0)) {
        G -= 1;
      }
    }
    if (active == changeBLUE) {
      if ((newPos > pos) && (B < 255)) {
        B += 1;
      }
      else if ((newPos < pos) && (B > 0)) {
        B -= 1;
      }
    }
    setColor(R, G, B);
    Serial.print("R = ");
    Serial.print(R);
    Serial.print(" G = ");
    Serial.print(G);
    Serial.print(" B = ");
    Serial.println(B);
    pos = newPos;
  }

  if (0) /*(millis() > (millisNow + 10))*/ {
    switch (active) {
      case changeRED:
        if (R == 255) {
          R = 0;
          active = changeGREEN;
        }
        else {
          R++;
        }
        break;
  
      case changeGREEN:
        if (G == 255) {
          G = 0;
          active = changeBLUE;
        }
        else {
          G++;
        }
        break;
      
      case changeBLUE:
        if (B == 255) {
          B = 0;
          active = changeRED;
        }
        else {
          B++;
        }
        break;
    }
    setColor(R, G, B);
    millisNow = millis();
  }
  
  enc_button.update();
  if (enc_button.rose()) {
    Serial.println("Click");
    pos = 0;
    if (active == changeRED) {
      active = changeGREEN;
    }
    else if (active == changeGREEN) {
      active = changeBLUE;
    }
    else if (active == changeBLUE) {
      active =changeRED;
    }
  } 

}
