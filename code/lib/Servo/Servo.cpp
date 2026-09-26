//
// Created by pawel on 22.09.2026.
//

/*
https://www.waveshare.com/servo-driver-with-esp32.htm

https://docs.waveshare.com/Servo_Driver_with_ESP32/Resources-And-Documents

https://docs.waveshare.com/Servo_Driver_with_ESP32/Product-Use

https://github.com/hybotix/STS3215-Tutorial/blob/main/REGISTER_REFERENCE.md

https://www.waveshare.com/wiki/ST3215_Servo
*/

#include "Servo.h"

# define steps 4096
# define SPEED_UNITS_PER_RPM (1.0 / 0.01464)

int currentSerwoPositionVertical = 0;
int currentSerwoPositionHorizontal = 0;

/*
Calibrates and initiates servos.
*/
void InitServos(SMS_STS &servos, u8 numberOfservos, ...) {
	va_list ap;
	va_start(ap, numberOfservos);
	for(int i = 0; i < numberOfservos; i++) {
		const int id = va_arg(ap, int);

		servos.EnableTorque(id, 0);

		servos.unLockEprom(id);
		// 3 => step servo mode
		servos.writeByte(id, SMS_STS_MODE, 3);

		servos.writeByte(id, SMS_STS_MIN_ANGLE_LIMIT_L, 0);
		servos.writeByte(id, SMS_STS_MIN_ANGLE_LIMIT_H, 0);

		servos.writeByte(id, SMS_STS_MAX_ANGLE_LIMIT_L, 0);
		servos.writeByte(id, SMS_STS_MAX_ANGLE_LIMIT_H, 0);

		servos.LockEprom(id);

		servos.EnableTorque(id, 1);
	}
	va_end(ap);
}

/*
Resets servos into facctory settings. Make sure to stop all servos before calling.
*/
void ClearServos(SMS_STS& servos, u8 numberOfServos, ...) {
	va_list ap;
	va_start(ap, numberOfServos);

	for (int i = 0; i < numberOfServos; i++) {

		const u8 id = va_arg(ap, int);

		if (!servos.EnableTorque(id, 0)) {
			Serial.println("ClearServos error, EnableTorque 1");
		}

		if (!servos.unLockEprom(id)) {
			Serial.println("ClearServos error, unLockEprom");
		}

		if (!servos.writeByte(id, SMS_STS_MODE, 0)) {
			Serial.println("ClearServos error, writeByte 1");
		}

		if (!servos.writeByte(id, SMS_STS_MIN_ANGLE_LIMIT_L, 0)) {
			Serial.println("ClearServos error, writeByte 2");
		}
		if (!servos.writeByte(id, SMS_STS_MIN_ANGLE_LIMIT_H, 0)) {
			Serial.println("ClearServos error, writeByte 3");
		}

		if (!servos.writeByte(id, SMS_STS_MAX_ANGLE_LIMIT_L, 0xFF)) {
			Serial.println("ClearServos error, writeByte 4");
		}
		if (!servos.writeByte(id, SMS_STS_MAX_ANGLE_LIMIT_H, 0x0F)) {
			Serial.println("ClearServos error, writeByte 5");
		}

		if (!servos.LockEprom(id)) {
			Serial.println("ClearServos error, LockEprom");
		}

		if (!servos.EnableTorque(id, 1)) {
			Serial.println("ClearServos error, EnableTorque 2");
		}

		if (!servos.WritePosEx(id, 0, 3072, 0)) {
			Serial.println("ClearServos error, WritePosEx");
		}
	}

	va_end(ap);
}

/*
 Simplest GoTo function, but chooses closest way to turn.
 angle => degreees (0 - 359)
 speedRPM => rotations per minute (0 - 45)
 acceleration => not a standard unit (0 - 150) (gives small delay to avoid sudden speedup)
*/
void GoToAngle(SMS_STS& servos, u8 id, u16 angle, u16 speedRPM, u8 acceleration) {
	speedRPM = constrain(speedRPM, 0, 45);
	acceleration = constrain(acceleration, 0, 150);
	angle %= 360;
	const int targetPosition = static_cast<long>(angle) * steps / 360;
	uint16_t speed = static_cast<u16>(floor(speedRPM * SPEED_UNITS_PER_RPM));

	int delta = 0;
	switch (id) {
		case VERTICAL: {
			delta = targetPosition - currentSerwoPositionVertical;
			currentSerwoPositionVertical = targetPosition;
			break;
		}
		case HORIZONTAL: {
			delta = targetPosition - currentSerwoPositionHorizontal;
			currentSerwoPositionHorizontal = targetPosition;
			break;
		}
		default: {
			Serial.println("GoToAngle id error");
		}
	}

	if (delta > steps / 2) {
		delta -= steps;
	}
	else if (delta < -steps / 2) {
		delta += steps;
	}

	if (!servos.WritePosEx(id, delta, speed, acceleration)) {
		Serial.println("GoToAngle error");
	}
}

/*
 Simplest GoTo function, but does it as fast as possible and chooses closest way to turn.
 angle => degreees (0 - 359)
*/
void GoToAngleASAP(SMS_STS& servos, u8 id, u16 angle) {
	angle %= 360;

	const int targetPosition = static_cast<long>(angle) * steps / 360;

	int delta = 0;
	switch (id) {
		case VERTICAL: {
			delta = targetPosition - currentSerwoPositionVertical;
			currentSerwoPositionVertical = targetPosition;
			break;
		}
		case HORIZONTAL: {
			delta = targetPosition - currentSerwoPositionHorizontal;
			currentSerwoPositionHorizontal = targetPosition;
			break;
		}
		default: {
			Serial.println("GoToAngleASAP id error");
		}
	}

	if (delta > steps / 2) {
		delta -= steps;
	}
	else if (delta < -steps / 2) {
		delta += steps;
	}

	if (!servos.WritePosEx(id, delta, 3073, 0)) {
		Serial.println("GoToAngleASAP error");
	}
}

/*
Returns ServoInfo structure containing information about selected servo.
*/
ServoInfo GetInfo(SMS_STS& servos, u8 id) {
	if(servos.FeedBack(id)!=-1){
		ServoInfo info;
		info.Pos = servos.ReadPos(-1);
		info.Speed = servos.ReadSpeed(-1);
		info.Load = servos.ReadLoad(-1);
		info.Voltage = servos.ReadVoltage(-1);
		info.Temper = servos.ReadTemper(-1);
		info.Move = servos.ReadMove(-1);
		info.Current = servos.ReadCurrent(-1);
		return info;
	}
	Serial.println("GetInfo error");
	return {};
}

// Luzne notatki dla innych
// ze strony 360 stopni = 4096 krokow
// https://www.waveshare.com/wiki/ST3215_Servo
// speed => kroki na skundę; 50 steps/sec≈0.732RPM; max = 3073

// trochę podejrzana stronka ale działa
// https://gitee.com/ftservo/FTServo_Linux/blob/main/examples/SMS_STS/SyncWritePos/SyncWritePos.cpp?utm_source=chatgpt.com
// acceleration => A=x*8.7deg/s^2 max = 150; im mniej tym wolniej
// A = 100 * (360/4096) = 8,7890625