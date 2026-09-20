/*
https://www.waveshare.com/servo-driver-with-esp32.htm

https://docs.waveshare.com/Servo_Driver_with_ESP32/Resources-And-Documents

https://docs.waveshare.com/Servo_Driver_with_ESP32/Product-Use
*/


#include <Arduino.h>

void setup() {
	// usb serial
	Serial.begin(115200);
}

void loop() {
	Serial.println("Buenos dias");
}