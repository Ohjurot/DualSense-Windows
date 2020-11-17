#include "IO_USB.h"

void __DS5W::USB::evaluateHidInputBuffer(unsigned char* hidInBuffer, DS5W::DS5InputState* ptrInputState) {
	// Convert sticks to signed range
	ptrInputState->leftStick.x  = (char)(((short)(hidInBuffer[0x01] - 128)));
	ptrInputState->leftStick.y  = (char)(((short)(hidInBuffer[0x02] - 127)) * -1);
	ptrInputState->rightStick.x = (char)(((short)(hidInBuffer[0x03] - 128)));
	ptrInputState->rightStick.y = (char)(((short)(hidInBuffer[0x04] - 127)) * -1);

	// Convert trigger to unsigned range
	ptrInputState->leftTrigger = hidInBuffer[0x05];
	ptrInputState->rightTrigger = hidInBuffer[0x06];

	// Buttons
	ptrInputState->buttonsAndDpad = hidInBuffer[0x08] & 0xF0;
	ptrInputState->buttonsA = hidInBuffer[0x09];
	ptrInputState->buttonsB = hidInBuffer[0x0A];

	// Dpad
	switch (hidInBuffer[0x08] & 0x0F) {
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
	memcpy(&ptrInputState->accelerometer, &hidInBuffer[0x10], 2 * 3);

	//TEMP: Copy gyro data (no processing currently done!)
	memcpy(&ptrInputState->gyroscope, &hidInBuffer[0x16], 2 * 3);

	// Evaluate touch state 1
	UINT32 touchpad1Raw = *(UINT32*)(&hidInBuffer[0x21]);
	ptrInputState->touchPoint1.x = (touchpad1Raw & 0xFFF00000) >> 20;
	ptrInputState->touchPoint1.y = (touchpad1Raw & 0x000FFF00) >> 8;

	// Evaluate touch state 2
	UINT32 touchpad2Raw = *(UINT32*)(&hidInBuffer[0x25]);
	ptrInputState->touchPoint2.x = (touchpad2Raw & 0xFFF00000) >> 20;
	ptrInputState->touchPoint2.y = (touchpad2Raw & 0x000FFF00) >> 8;

	// Evaluate headphone input
	ptrInputState->headPhoneConnected = hidInBuffer[0x36] & 0x01;

	// Trigger force feedback
	ptrInputState->leftTriggerFeedback = hidInBuffer[0x2B];
	ptrInputState->rightTriggerFeedback = hidInBuffer[0x2A];
}

void __DS5W::USB::createHidOutputBuffer(unsigned char* hidOutBuffer, DS5W::DS5OutputState* ptrOutputState) {
	// Cleat all input data
	ZeroMemory(hidOutBuffer, 48);
	
	// Report type output
	hidOutBuffer[0x00] = 0x02;

	// Feature mask
	hidOutBuffer[0x01] = 0xFF;
	hidOutBuffer[0x02] = 0xF7;

	// Rumbel motors
	hidOutBuffer[0x03] = ptrOutputState->rightRumble;
	hidOutBuffer[0x04] = ptrOutputState->leftRumble;

	// Mic led
	hidOutBuffer[0x09] = (unsigned char)ptrOutputState->microphoneLed;

	// Player led
	hidOutBuffer[0x2C] = ptrOutputState->playerLeds.bitmask;
	if (ptrOutputState->playerLeds.playerLedFade) {
		hidOutBuffer[0x2C] &= ~(0x20);
	} else {
		hidOutBuffer[0x2C] |= 0x20;
	}

	// Player led brightness
	hidOutBuffer[0x27] = 0x01;
	hidOutBuffer[0x2B] = ptrOutputState->playerLeds.brightness;

	// Lightbar
	hidOutBuffer[0x2D] = ptrOutputState->lightbar.r;
	hidOutBuffer[0x2E] = ptrOutputState->lightbar.g;
	hidOutBuffer[0x2F] = ptrOutputState->lightbar.b;

	// Adaptive Triggers
	processTrigger(&ptrOutputState->leftTriggerEffect, &hidOutBuffer[0x16]);
	processTrigger(&ptrOutputState->rightTriggerEffect, &hidOutBuffer[0x0B]);

	// TODO: Haptic (maybe in diffrent "audio" function)
}

void __DS5W::USB::processTrigger(DS5W::TriggerEffect* ptrEffect, unsigned char* buffer) {
	// Switch on effect
	switch (ptrEffect->effectType) {
		// Continious
		case DS5W::TriggerEffectType::ContinuousResitance:
			// Mode
			buffer[0x00] = 0x01;
			// Parameters
			buffer[0x01] = ptrEffect->Continuous.startPosition;
			buffer[0x02] = ptrEffect->Continuous.force;

			break;

		// Section
		case DS5W::TriggerEffectType::SectionResitance:
			// Mode
			buffer[0x00] = 0x02;
			// Parameters
			buffer[0x01] = ptrEffect->Continuous.startPosition;
			buffer[0x02] = ptrEffect->Continuous.force;

			break;

		// EffectEx
		case DS5W::TriggerEffectType::EffectEx:
			// Mode
			buffer[0x00] = 0x02 | 0x20 | 0x04;
			// Parameters
			buffer[0x01] = 0xFF - ptrEffect->EffectEx.startPosition;
			// Keep flag
			if (ptrEffect->EffectEx.keepEffect) {
				buffer[0x02] = 0x02;
			}
			// Forces
			buffer[0x04] = ptrEffect->EffectEx.beginForce;
			buffer[0x05] = ptrEffect->EffectEx.middleForce;
			buffer[0x06] = ptrEffect->EffectEx.endForce;
			// Frequency
			buffer[0x09] = max(1, ptrEffect->EffectEx.frequency / 2);

			break;

		// Calibrate
		case DS5W::TriggerEffectType::Calibrate:
			// Mode 
			buffer[0x00] = 0xFC;

			break;

		// No resistance / default
		case DS5W::TriggerEffectType::NoResitance:
			__fallthrough;
		default:
			// All zero
			buffer[0x00] = 0x00;
			buffer[0x01] = 0x00;
			buffer[0x02] = 0x00;

			break;
	}		
}
