/*
	IO.cpp is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/

#include <DualSenseWindows/IO.h>
#include <DualSenseWindows/DS_CRC32.h>
#include <DualSenseWindows/DS5_Input.h>
#include <DualSenseWindows/DS5_Output.h>

#define NOMINMAX

#include <Windows.h>
#include <malloc.h>

#include <initguid.h>
#include <Hidclass.h>
#include <SetupAPI.h>
#include <hidsdi.h>

#define VENDOR_ID_SONY		0x054C
#define DEVICE_ID_DS5		0x0CE6
#define DEVICE_ID_DS5_EDGE 	0x0DF2

DS5W_API DS5W_ReturnValue DS5W::enumDevices(void* ptrBuffer, unsigned int inArrLength, unsigned int* requiredLength, bool pointerToArray) {
	// Check for invalid non expected buffer
	if (inArrLength && !ptrBuffer) {
		inArrLength = 0;
	}

	// Get all hid devices from devs
	HANDLE hidDiHandle = SetupDiGetClassDevs(&GUID_DEVINTERFACE_HID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (!hidDiHandle || (hidDiHandle == INVALID_HANDLE_VALUE)) {
		return DS5W_E_EXTERNAL_WINAPI;
	}

	// Index into input array
	unsigned int inputArrIndex = 0;
	bool inputArrOverflow = false;

	// Enumerate over hid device
	DWORD devIndex = 0;
	SP_DEVINFO_DATA hidDiInfo;
	hidDiInfo.cbSize = sizeof(SP_DEVINFO_DATA);
	while (SetupDiEnumDeviceInfo(hidDiHandle, devIndex, &hidDiInfo)) {
		
		// Enumerate over all hid device interfaces
		DWORD ifIndex = 0;
		SP_DEVICE_INTERFACE_DATA ifDiInfo;
		ifDiInfo.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		while (SetupDiEnumDeviceInterfaces(hidDiHandle, &hidDiInfo, &GUID_DEVINTERFACE_HID, ifIndex, &ifDiInfo)) {

			// Query device path size
			DWORD requiredSize = 0;
			SetupDiGetDeviceInterfaceDetailW(hidDiHandle, &ifDiInfo, NULL, 0, &requiredSize, NULL);

			// Check size
			if (requiredSize > (260 * sizeof(wchar_t))) {
				SetupDiDestroyDeviceInfoList(hidDiHandle);
				return DS5W_E_EXTERNAL_WINAPI;
			}

			// Allocate memory for path on the stack
			SP_DEVICE_INTERFACE_DETAIL_DATA_W* devicePath = (SP_DEVICE_INTERFACE_DETAIL_DATA_W*)_malloca(requiredSize);
			if (!devicePath) {
				SetupDiDestroyDeviceInfoList(hidDiHandle);
				return DS5W_E_STACK_OVERFLOW;
			}
			
			// Get device path
			devicePath->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
			SetupDiGetDeviceInterfaceDetailW(hidDiHandle, &ifDiInfo, devicePath, requiredSize, NULL, NULL);

			// Check if input array has space
			// Check if device is reachable
			HANDLE deviceHandle = CreateFileW(devicePath->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

			// Check if device is reachable
			if (deviceHandle && (deviceHandle != INVALID_HANDLE_VALUE)) {
				// Get vendor and product id
				unsigned int vendorId = 0;
				unsigned int productId = 0;
				HIDD_ATTRIBUTES deviceAttributes;
				if (HidD_GetAttributes(deviceHandle, &deviceAttributes)) {
					vendorId = deviceAttributes.VendorID;
					productId = deviceAttributes.ProductID;
				}

				// Check if ids match
				if (vendorId == VENDOR_ID_SONY && (productId == DEVICE_ID_DS5 || productId == DEVICE_ID_DS5_EDGE)) {
					// Get pointer to target
					DS5W::DeviceEnumInfo* ptrInfo = nullptr;
					if (inputArrIndex < inArrLength) {
						if (pointerToArray) {
							ptrInfo = &(((DS5W::DeviceEnumInfo*)ptrBuffer)[inputArrIndex]);
						}
						else {
							ptrInfo = (((DS5W::DeviceEnumInfo**)ptrBuffer)[inputArrIndex]);
						}
					}

					// Copy path
					if (ptrInfo) {
						wcscpy_s(ptrInfo->_internal.path, 260, (const wchar_t*)devicePath->DevicePath);
					}

					// Get preparsed data
					PHIDP_PREPARSED_DATA ppd;
					if (HidD_GetPreparsedData(deviceHandle, &ppd)) {
						// Get device capcbilitys
						HIDP_CAPS deviceCaps;
						if (HidP_GetCaps(ppd, &deviceCaps) == HIDP_STATUS_SUCCESS) {
							// Check for device connection type
							if (ptrInfo) {
								// Check if controller matches USB specifications
								if (deviceCaps.InputReportByteLength == 64) {
									ptrInfo->_internal.connection = DS5W::DeviceConnection::USB;

									// Device found and valid -> Inrement index
									inputArrIndex++;
								}
								// Check if controler matches BT specifications
								else if(deviceCaps.InputReportByteLength == 78) {
									ptrInfo->_internal.connection = DS5W::DeviceConnection::BT;

									// Device found and valid -> Inrement index
									inputArrIndex++;
								}
							}
						}

						// Free preparsed data
						HidD_FreePreparsedData(ppd);
					}
				}

				// Close device
				CloseHandle(deviceHandle);
			}

			// Increment index
			ifIndex++;

			// Free device from stack
			_freea(devicePath);
		}

		// Increment index
		devIndex++;
	}

	// Close device enum list
	SetupDiDestroyDeviceInfoList(hidDiHandle);
	
	// Set required size if exists
	if (requiredLength) {
		*requiredLength = inputArrIndex;
	}

	// Check if array was suficient
	if (inputArrIndex <= inArrLength) {
		return DS5W_OK;
	}
	// Else return error
	else {
		return DS5W_E_INSUFFICIENT_BUFFER;
	}
}

DS5W_API DS5W_ReturnValue DS5W::initDeviceContext(DS5W::DeviceEnumInfo* ptrEnumInfo, DS5W::DeviceContext* ptrContext) {
	// Check if pointers are valid
	if (!ptrEnumInfo || !ptrContext) {
		return DS5W_E_INVALID_ARGS;
	}

	// Check len
	if (wcslen(ptrEnumInfo->_internal.path) == 0) {
		return DS5W_E_INVALID_ARGS;
	}

	// Connect to device
	HANDLE deviceHandle = CreateFileW(ptrEnumInfo->_internal.path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (!deviceHandle || (deviceHandle == INVALID_HANDLE_VALUE)) {
		return DS5W_E_DEVICE_REMOVED;
	}

	// Write to conext
	ptrContext->_internal.connected = true;
	ptrContext->_internal.connection = ptrEnumInfo->_internal.connection;
	ptrContext->_internal.deviceHandle = deviceHandle;
	wcscpy_s(ptrContext->_internal.devicePath, 260, ptrEnumInfo->_internal.path);

	// Get input report length
	unsigned short reportLength = 0;
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		// Start BT by reading feature report 5
		unsigned char fBuffer[64];
		fBuffer[0] = 0x05;
		if (!HidD_GetFeature(deviceHandle, fBuffer, 64)) {
			return DS5W_E_BT_COM;
		}
		
		// The bluetooth input report is 78 Bytes long
		reportLength = 547;
	}
	else {
		// The usb input report is 64 Bytes long
		reportLength = 64;
	}
	
	// Return OK
	return DS5W_OK;
}

DS5W_API void DS5W::freeDeviceContext(DS5W::DeviceContext* ptrContext) {
	// Check if handle is existing
	if (ptrContext->_internal.deviceHandle) {
		// Send zero output report to disable all onging outputs
		DS5W::DS5OutputState os;
		ZeroMemory(&os, sizeof(DS5W::DS5OutputState));
		os.leftTriggerEffect.effectType = TriggerEffectType::NoResitance;
		os.rightTriggerEffect.effectType = TriggerEffectType::NoResitance;
		os.disableLeds = true;

		DS5W::setDeviceOutputState(ptrContext, &os);

		// Close handle
		CloseHandle(ptrContext->_internal.deviceHandle);
		ptrContext->_internal.deviceHandle = NULL;
	}
	
	// Unset bool
	ptrContext->_internal.connected = false;

	// Unset string
	ptrContext->_internal.devicePath[0] = 0x0;
}

DS5W_API DS5W_ReturnValue DS5W::reconnectDevice(DS5W::DeviceContext* ptrContext) {	
	// Check len
	if (wcslen(ptrContext->_internal.devicePath) == 0) {
		return DS5W_E_INVALID_ARGS;
	}

	// Connect to device
	HANDLE deviceHandle = CreateFileW(ptrContext->_internal.devicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (!deviceHandle || (deviceHandle == INVALID_HANDLE_VALUE)) {
		return DS5W_E_DEVICE_REMOVED;
	}

	// Write to conext
	ptrContext->_internal.connected = true;
	ptrContext->_internal.deviceHandle = deviceHandle;

	// Return ok
	return DS5W_OK;
}

DS5W_API DS5W_ReturnValue DS5W::getDeviceInputState(DS5W::DeviceContext* ptrContext, DS5W::DS5InputState* ptrInputState) {
	// Check pointer
	if (!ptrContext || !ptrInputState) {
		return DS5W_E_INVALID_ARGS;
	}

	// Check for connection
	if (!ptrContext->_internal.connected) {
		return DS5W_E_DEVICE_REMOVED;
	}

	// Get the most recent package
	HidD_FlushQueue(ptrContext->_internal.deviceHandle);

	// Get input report length
	unsigned short inputReportLength = 0;
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		// The bluetooth input report is 78 Bytes long
		inputReportLength = 78;
		ptrContext->_internal.hidBuffer[0] = 0x31;
	}
	else {
		// The usb input report is 64 Bytes long
		inputReportLength = 64;
		ptrContext->_internal.hidBuffer[0] = 0x01;
	}

	// Get device input
	if (!ReadFile(ptrContext->_internal.deviceHandle, ptrContext->_internal.hidBuffer, inputReportLength, NULL, NULL)) {
		// Close handle and set error state
		CloseHandle(ptrContext->_internal.deviceHandle);
		ptrContext->_internal.deviceHandle = NULL;
		ptrContext->_internal.connected = false;

		// Return error
		return DS5W_E_DEVICE_REMOVED;
	}

	// Evaluete input buffer
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		// Call bluetooth evaluator if connection is qual to BT
		__DS5W::Input::evaluateHidInputBuffer(&ptrContext->_internal.hidBuffer[2], ptrInputState);
	} else {
		// Else it is USB so call its evaluator
		__DS5W::Input::evaluateHidInputBuffer(&ptrContext->_internal.hidBuffer[1], ptrInputState);
	}
	
	// Return ok
	return DS5W_OK;
}

DS5W_API DS5W_ReturnValue DS5W::setDeviceOutputState(DS5W::DeviceContext* ptrContext, DS5W::DS5OutputState* ptrOutputState) {
	// Check pointer
	if (!ptrContext || !ptrOutputState) {
		return DS5W_E_INVALID_ARGS;
	}

	// Check for connection
	if (!ptrContext->_internal.connected) {
		return DS5W_E_DEVICE_REMOVED;
	}

	// Get otuput report length
	unsigned short outputReportLength = 0;
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		// The bluetooth input report is 547 Bytes long
		outputReportLength = 547;
	}
	else {
		// The usb input report is 48 Bytes long
		outputReportLength = 48;
	}

	// Cleat all input data
	ZeroMemory(ptrContext->_internal.hidBuffer, outputReportLength);

	// Build output buffer
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		//return DS5W_E_CURRENTLY_NOT_SUPPORTED;
		// Report type
		ptrContext->_internal.hidBuffer[0x00] = 0x31;
		ptrContext->_internal.hidBuffer[0x01] = 0x02;
		__DS5W::Output::createHidOutputBuffer(&ptrContext->_internal.hidBuffer[2], ptrOutputState);

		// Hash
		const UINT32 crcChecksum = __DS5W::CRC32::compute(ptrContext->_internal.hidBuffer, 74);

		ptrContext->_internal.hidBuffer[0x4A] = (unsigned char)((crcChecksum & 0x000000FF) >> 0UL);
		ptrContext->_internal.hidBuffer[0x4B] = (unsigned char)((crcChecksum & 0x0000FF00) >> 8UL);
		ptrContext->_internal.hidBuffer[0x4C] = (unsigned char)((crcChecksum & 0x00FF0000) >> 16UL);
		ptrContext->_internal.hidBuffer[0x4D] = (unsigned char)((crcChecksum & 0xFF000000) >> 24UL);
		
	}
	else {
		// Report type
		ptrContext->_internal.hidBuffer[0x00] = 0x02;

		// Else it is USB so call its evaluator
		__DS5W::Output::createHidOutputBuffer(&ptrContext->_internal.hidBuffer[1], ptrOutputState);
	}

	// Write to controller
	if (!WriteFile(ptrContext->_internal.deviceHandle, ptrContext->_internal.hidBuffer, outputReportLength, NULL, NULL)) {
		// Close handle and set error state
		CloseHandle(ptrContext->_internal.deviceHandle);
		ptrContext->_internal.deviceHandle = NULL;
		ptrContext->_internal.connected = false;

		// Return error
		return DS5W_E_DEVICE_REMOVED;
	}

	// OK 
	return DS5W_OK;
}
