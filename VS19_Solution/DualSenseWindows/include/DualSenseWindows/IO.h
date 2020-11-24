/*
	DualSenseWindows API
	https://github.com/Ohjurot/DualSense-Windows

	MIT License

	Copyright (c) 2020 Ludwig Füchsl

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/
#pragma once

#include <DualSenseWindows/DSW_Api.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/DS5State.h>

namespace DS5W {
	/// <summary>
	/// Enumerate all ds5 deviced connected to the computer
	/// </summary>
	/// <param name="ptrBuffer">Pointer to begin of array of DeviceEnumInfo objects / DeviceEnumInfo pointers</param>
	/// <param name="inArrLength">Length of imput array</param>
	/// <param name="pointerToArray">(Optional) true: DeviceEnumInfo pointer is the pointer to an array of DeviceEnumInfo objects. false: DeviceEnumInfo pointer is a pointer to DeviceEnumInfo pointers to DeviceEnumInfo objects</param>
	/// <param name="requiredLength">(Optional) pointer to uint witch recives the required total length</param>
	/// <returns>DS5W Return value</returns>
	DS5W_API DS5W_ReturnValue enumDevices(void* ptrBuffer, unsigned int inArrLength, unsigned int* ptrLength, bool pointerToArray = true);

	/// <summary>
	/// Initializes a DeviceContext from its enum infos
	/// </summary>
	/// <param name="ptrEnumInfo">Pointer to enum object to create device from</param>
	/// <param name="ptrContext">Pointer to context to create to</param>
	/// <returns>If creation was successfull</returns>
	DS5W_API DS5W_ReturnValue initDeviceContext(DS5W::DeviceEnumInfo* ptrEnumInfo, DS5W::DeviceContext* ptrContext);

	/// <summary>
	/// Free the device conntext
	/// </summary>
	/// <param name="ptrContext">Pointer to context</param>
	DS5W_API void freeDeviceContext(DS5W::DeviceContext* ptrContext);

	/// <summary>
	/// Try to reconnect a removed device
	/// </summary>
	/// <param name="ptrContext">Context to reconnect on</param>
	/// <returns>Result</returns>
	DS5W_API DS5W_ReturnValue reconnectDevice(DS5W::DeviceContext* ptrContext);

	/// <summary>
	/// Get device input state
	/// </summary>
	/// <param name="ptrContext">Pointer to context</param>
	/// <param name="ptrInputState">Pointer to input state</param>
	/// <returns>Result of call</returns>
	DS5W_API DS5W_ReturnValue getDeviceInputState(DS5W::DeviceContext* ptrContext, DS5W::DS5InputState* ptrInputState);

	/// <summary>
	/// Set the device output state
	/// </summary>
	/// <param name="ptrContext">Pointer to context</param>
	/// <param name="ptrOutputState">Pointer to output state to be set</param>
	/// <returns>Result of call</returns>
	DS5W_API DS5W_ReturnValue setDeviceOutputState(DS5W::DeviceContext* ptrContext, DS5W::DS5OutputState* ptrOutputState);
}
