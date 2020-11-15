/*
	IO_USB.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#include <Windows.h>

#include <DualSenseWindows/DSW_Api.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/DS5State.h>

#include <DualSenseWindows/IO_TriggerFx.h>

namespace __DS5W {
	namespace USB {
		/// <summary>
		/// Interprete the hid returned buffer as a usb buffer
		/// </summary>
		/// <param name="hidInBuffer">Input buffer</param>
		/// <param name="ptrInputState">Input state to be set</param>
		void evaluateHidInputBuffer(unsigned char* hidInBuffer, DS5W::DS5InputState* ptrInputState);

		/// <summary>
		/// Creates the hid output buffer for USB
		/// </summary>
		/// <param name="hidOutBuffer">HID Output buffer</param>
		/// <param name="ptrOutputState">Pointer to state to read from</param>
		void createHidOutputBuffer(unsigned char* hidOutBuffer, DS5W::DS5OutputState* ptrOutputState);
	}
}
