#include <Dabble.h>
#include <GamePadModule.h>
#include <SoftwareSerial.h>
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

// Define pins
const int Trig = A3;
const int Echo = A2;
const int PWM2A = 11;      // Rear Left Motor (M1)
const int PWM2B = 3;       // Rear Right Motor (M2)
const int PWM0A = 6;       // Front Right Motor (M3)
const int PWM0B = 5;       // Front Left Motor (M4)
const int DIR_CLK = 4;     // Data input clock line
const int DIR_EN = 7;      // Enable the L293D pins
const int DATA = 8;        // USB cable
const int DIR_LATCH = 12;  // Output memory latch clock

// Define motion states
const int Move_Forward = 39;    // Move Forward
const int Move_Backward = 216;   // Move Backward
const int Left_Move = 116;       // Left translation
const int Right_Move = 139;      // Right translation
const int Right_Rotate = 149;    // Rotate Right
const int Left_Rotate = 106;     // Rotate Left
const int Stop = 0;              // Stop

// Set the default speed between 1 and 255
int Speed1 = 255;
int Speed2 = 255;
int Speed3 = 255;
int Speed4 = 255;

// Speed presets
const int Low_Speed = 150;
const int Medium_Speed = 200;
const int High_Speed = 230;
const int Max_Speed = 255;

void Motor(int Dir, int Speed1, int Speed2, int Speed3, int Speed4) {
    analogWrite(PWM2A, Speed1); // Rear Left Motor (M1)
    analogWrite(PWM2B, Speed2); // Rear Right Motor (M2)
    analogWrite(PWM0A, Speed3); // Front Right Motor (M3)
    analogWrite(PWM0B, Speed4); // Front Left Motor (M4)

    digitalWrite(DIR_LATCH, LOW);
    shiftOut(DATA, DIR_CLK, MSBFIRST, Dir);
    digitalWrite(DIR_LATCH, HIGH);
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

    if (GamePad.isSelectPressed()) {
        // Decrease speed
        Speed1 = max(Speed1 - 45, 0);
        Speed2 = max(Speed2 - 45, 0);
        Speed3 = max(Speed3 - 45, 0);
        Speed4 = max(Speed4 - 45, 0);
        delay(100); // Debounce delay
    }
    else if (GamePad.isStartPressed()) {
        // Increase speed
        Speed1 = min(Speed1 + 45, 255);
        Speed2 = min(Speed2 + 45, 255);
        Speed3 = min(Speed3 + 45, 255);
        Speed4 = min(Speed4 + 45, 255);
        delay(100); // Debounce delay
    }

    // Control motion based on button presses
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
        Motor(Left_Move, Speed4, Speed3, Speed2, Speed1); // Swapping motor assignments for left movement
        delay(100);
    }
    else if (GamePad.isRightPressed()) {
        Serial.println("Right");
        Motor(Right_Move, Speed3, Speed4, Speed1, Speed2); // Swapping motor assignments for right movement
        delay(100);
    }
    else if (GamePad.isSquarePressed()) {
        Serial.println("Left Rotate");
        Motor(Left_Rotate, Speed4, Speed3, Speed2, Speed1);
        delay(100);
    }
    else if (GamePad.isCirclePressed()) {
        Serial.println("Right Rotate");
        Motor(Right_Rotate, Speed3, Speed4, Speed1, Speed2);
        delay(100);
    }
    else {
        Serial.println("Stopped");
        Motor(Stop, 0, 0, 0, 0);
        delay(200); // Adjust delay as needed
    }
}