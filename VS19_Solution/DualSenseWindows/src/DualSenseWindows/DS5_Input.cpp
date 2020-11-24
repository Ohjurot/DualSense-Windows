#include "DS5_Input.h"

void __DS5W::Input::evaluateHidInputBuffer(unsigned char* hidInBuffer, DS5W::DS5InputState* ptrInputState) {
	// Convert sticks to signed range
	ptrInputState->leftStick.x = (char)(((short)(hidInBuffer[0x00] - 128)));
	ptrInputState->leftStick.y = (char)(((short)(hidInBuffer[0x01] - 127)) * -1);
	ptrInputState->rightStick.x = (char)(((short)(hidInBuffer[0x02] - 128)));
	ptrInputState->rightStick.y = (char)(((short)(hidInBuffer[0x03] - 127)) * -1);

	// Convert trigger to unsigned range
	ptrInputState->leftTrigger = hidInBuffer[0x04];
	ptrInputState->rightTrigger = hidInBuffer[0x05];

	// Buttons
	ptrInputState->buttonsAndDpad = hidInBuffer[0x07] & 0xF0;
	ptrInputState->buttonsA = hidInBuffer[0x08];
	ptrInputState->buttonsB = hidInBuffer[0x09];

	// Dpad
	switch (hidInBuffer[0x07] & 0x0F) {
		// Up
	case 0x0:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_UP;
		break;
		// Down
	case 0x4:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_DOWN;
		break;
		// Left
	case 0x6:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_LEFT;
		break;
		// Right
	case 0x2:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_RIGHT;
		break;
		// Left Down
	case 0x5:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_LEFT | DS5W_ISTATE_DPAD_DOWN;
		break;
		// Left Up
	case 0x7:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_LEFT | DS5W_ISTATE_DPAD_UP;
		break;
		// Right Up
	case 0x1:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_RIGHT | DS5W_ISTATE_DPAD_UP;
		break;
		// Right Down
	case 0x3:
		ptrInputState->buttonsAndDpad |= DS5W_ISTATE_DPAD_RIGHT | DS5W_ISTATE_DPAD_DOWN;
		break;
	}

	// Copy accelerometer readings
	memcpy(&ptrInputState->accelerometer, &hidInBuffer[0x0F], 2 * 3);

	//TEMP: Copy gyro data (no processing currently done!)
	memcpy(&ptrInputState->gyroscope, &hidInBuffer[0x15], 2 * 3);

	// Evaluate touch state 1
	UINT32 touchpad1Raw = *(UINT32*)(&hidInBuffer[0x20]);
	ptrInputState->touchPoint1.y = (touchpad1Raw & 0xFFF00000) >> 20;
	ptrInputState->touchPoint1.x = (touchpad1Raw & 0x000FFF00) >> 8;

	// Evaluate touch state 2
	UINT32 touchpad2Raw = *(UINT32*)(&hidInBuffer[0x24]);
	ptrInputState->touchPoint2.y = (touchpad2Raw & 0xFFF00000) >> 20;
	ptrInputState->touchPoint2.x = (touchpad2Raw & 0x000FFF00) >> 8;

	// Evaluate headphone input
	ptrInputState->headPhoneConnected = hidInBuffer[0x35] & 0x01;

	// Trigger force feedback
	ptrInputState->leftTriggerFeedback = hidInBuffer[0x2A];
	ptrInputState->rightTriggerFeedback = hidInBuffer[0x29];

	// Battery
	ptrInputState->battery.chargin = (hidInBuffer[0x35] & 0x08);
	ptrInputState->battery.fullyCharged = (hidInBuffer[0x36] & 0x20);
	ptrInputState->battery.level = (hidInBuffer[0x36] & 0x0F);
}
