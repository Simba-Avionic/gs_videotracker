/*
https://www.waveshare.com/servo-driver-with-esp32.htm

https://docs.waveshare.com/Servo_Driver_with_ESP32/Resources-And-Documents

https://docs.waveshare.com/Servo_Driver_with_ESP32/Product-Use

https://github.com/hybotix/STS3215-Tutorial/blob/main/REGISTER_REFERENCE.md

https://www.waveshare.com/wiki/ST3215_Servo
*/


#include <Arduino.h>

#include "../lib/Servo/Servo.h"

#define S_RXD 18
#define S_TXD 19

SMS_STS servos;

void setup() {
    // usb serial
    Serial.begin(115200);

    // serva
    Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
    servos.pSerial = &Serial1;

    InitServos(servos, 1, VERTICAL);

    GoToAngle(servos, VERTICAL, 180, 45, 150);

    delay(5000);
    ClearServos(servos, 1, VERTICAL);
}

void loop() {
	Serial.println("Buenos dias");
}