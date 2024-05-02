#include <Dabble.h>
#include <GamePadModule.h>
#include <SoftwareSerial.h>
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

// Define pins
// Define pins
// Define pins
const int Trig = A3;
const int Echo = A2;
const int PWM2A = 11;      // M1 motor
const int PWM2B = 3;       // M2 motor
const int PWM0A = 6;       // M3 motor
const int PWM0B = 5;       // M4 motor
const int DIR_CLK = 4;     // Data input clock line
const int DIR_EN = 7;      // Equip the L293D enabling pins
const int DATA = 8;        // USB cable
const int DIR_LATCH = 12;  // Output memory latch clock

// Define motion states
const int Move_Forward = 39;    // Move Forward
const int Move_Backward = 216;   // Move Backward
const int Left_Move = 116;       // Left translation
const int Right_Move = 139;      // Right translation
const int Right_Rotate = 149;    // Rotate Left
const int Left_Rotate = 106;     // Rotate Left
const int Stop = 0;              // Parking variable
const int Upper_Left_Move = 36;  // Upper Left Move
const int Upper_Right_Move = 3;  // Upper Right Move
const int Lower_Left_Move = 80;  // Lower Left Move
const int Lower_Right_Move = 136;// Lower Right Move
const int Drift_Left = 20;       // Drift on Left
const int Drift_Right = 10;      // Drift on Right

// Set the default speed between 1 and 255
int Speed1 = 255;
int Speed2 = 255;
int Speed3 = 255;
int Speed4 = 255;

void Motor(int Dir, int Speed1, int Speed2, int Speed3, int Speed4) {
    analogWrite(PWM2A, Speed1); // Motor PWM speed regulation
    analogWrite(PWM2B, Speed2); // Motor PWM speed regulation
    analogWrite(PWM0A, Speed3); // Motor PWM speed regulation
    analogWrite(PWM0B, Speed4); // Motor PWM speed regulation

    digitalWrite(DIR_LATCH, LOW); // DIR_LATCH sets the low level and writes the direction of motion in preparation
    shiftOut(DATA, DIR_CLK, MSBFIRST, Dir); // Write Dir motion direction value
    digitalWrite(DIR_LATCH, HIGH); // DIR_LATCH sets the high level and outputs the direction of motion
}

void setup() {
    pinMode(DIR_CLK, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(DIR_EN, OUTPUT);
    pinMode(DIR_LATCH, OUTPUT);
    pinMode(PWM0B, OUTPUT);
    pinMode(PWM0A, OUTPUT);
    pinMode(PWM2A, OUTPUT);
    pinMode(PWM2B, OUTPUT);
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
    Serial.begin(250000);
    Dabble.begin(9600, A0, A1);
}

void loop() {
    Dabble.processInput();

    if (GamePad.isUpPressed()) {
        Serial.println("Forward");
        Motor(Move_Forward, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isDownPressed()) {
        Serial.println("Backward");
        Motor(Move_Backward, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isLeftPressed()) {
        Serial.println("Left");
        Motor(Left_Move, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isRightPressed()) {
        Serial.println("Right");
        Motor(Right_Move, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isSquarePressed()) {
        Serial.println("Left Rotate");
        Motor(Left_Rotate, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isCirclePressed()) {
        Serial.println("Right Rotate");
        Motor(Right_Rotate, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isCrossPressed()) {
        Serial.println("Drift Left");
        Motor(Drift_Left, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else if (GamePad.isStartPressed()) {
        Serial.println("Right Drift");
        Motor(Drift_Right, Speed1, Speed2, Speed3, Speed4);
        delay(100);
    }
    else {
        Serial.println("Stopped");
        Motor(Stop, 0, 0, 0, 0);
        delay(200); // Adjust delay as needed
    }
}