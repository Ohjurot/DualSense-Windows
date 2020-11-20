/*
	DS5_Output.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#include <DualSenseWindows/DSW_Api.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/DS5State.h>

#include <Windows.h>

namespace __DS5W {
	namespace Output {
		/// <summary>
		/// Creates the hid output buffer
		/// </summary>
		/// <param name="hidOutBuffer">HID Output buffer</param>
		/// <param name="ptrOutputState">Pointer to state to read from</param>
		void createHidOutputBuffer(unsigned char* hidOutBuffer, DS5W::DS5OutputState* ptrOutputState);

		/// <summary>
		/// Process trigger
		/// </summary>
		/// <param name="ptrEffect">Pointer to effect to be applied</param>
		/// <param name="buffer">Buffer for trigger parameters</param>
		void processTrigger(DS5W::TriggerEffect* ptrEffect, unsigned char* buffer);
	}
}