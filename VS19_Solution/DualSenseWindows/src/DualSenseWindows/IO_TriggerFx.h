/*
	IO_BT.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#include <DualSenseWindows/DSW_Api.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/TriggerFX.h>


#include <Windows.h>

namespace __DS5W {
	/// <summary>
	/// Function to set required output buffer data for a trigger fx
	/// </summary>
	/// <param name="ptrTriggerFxBuffer">Pointer to begin of left or right trigger effect</param>
	/// <param name="ptrTriggerFx">Pointer to trigger effect to use</param>
	/// <returns>If was suuccesfull</returns>
	bool setTriggerEffect(unsigned char* ptrTriggerFxBuffer, DS5W::TriggerFX* ptrTriggerFx);
}