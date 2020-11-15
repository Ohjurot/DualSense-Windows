#include "IO_BT.h"

void __DS5W::BT::evaluateHidInputBuffer(unsigned char* hidInBuffer, DS5W::DS5InputState* ptrInputState) {
	// Convert sticks to signed range
	ptrInputState->leftStick.x = (char)(((short)(hidInBuffer[0x01] - 128)));
	ptrInputState->leftStick.y = (char)(((short)(hidInBuffer[0x02] - 127)) * -1);
	ptrInputState->rightStick.x = (char)(((short)(hidInBuffer[0x03] - 128)));
	ptrInputState->rightStick.y = (char)(((short)(hidInBuffer[0x04] - 127)) * -1);

	// Convert trigger to unsigned range
	ptrInputState->leftTrigger = hidInBuffer[0x08];
	ptrInputState->rightTrigger = hidInBuffer[0x09];

	// Buttons
	ptrInputState->buttonsAndDpad = hidInBuffer[0x05] & 0xF0;
	ptrInputState->buttonsA = hidInBuffer[0x06];

	// Dpad
	switch (hidInBuffer[0x05] & 0x0F) {
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

	// Unset all (currently) not supported features
	ptrInputState->accelerometer = { 0,0,0 };
	ptrInputState->gyroscope = { 0, 0, 0 };
	ptrInputState->touchPoint1 = { 0 , 0 };
	ptrInputState->touchPoint2 = { 0 , 0 };
	ptrInputState->headPhoneConnected = false;
	ptrInputState->leftTriggerFeedback = 0;
	ptrInputState->rightTriggerFeedback = 0;
}

void __DS5W::BT::createHidOutputBuffer(unsigned char* hidOutBuffer, DS5W::DS5OutputState* ptrOutputState) {
	// Cleat all input data
	ZeroMemory(hidOutBuffer, 48);

	// Report type output
	hidOutBuffer[0x00] = 0x02;
}
