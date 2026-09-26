//
// Created by pawel on 22.09.2026.
//

/*
Tiny library to control st3215 servo
*/

#ifndef CODE_SERVO_H
#define CODE_SERVO_H

#include <SCServo.h>

#define HORIZONTAL 1
#define VERTICAL 2

void InitServos(SMS_STS &servos, u8 numberOfservos, ...);

void ClearServos(SMS_STS &servos, u8 numberOfservos, ...);

void GoToAngle(SMS_STS& servos, u8 id, u16 angle, u16 speedRPM, u8 acceleration);

void GoToAngleASAP(SMS_STS& servos, u8 id, u16 angle);

struct ServoInfo GetInfo(SMS_STS& servos, u8 id);

struct ServoInfo {
	int Pos;
	int Speed;
	int Load;
	int Voltage;
	int Temper;
	int Move;
	int Current;
};

#endif //CODE_SERVO_H