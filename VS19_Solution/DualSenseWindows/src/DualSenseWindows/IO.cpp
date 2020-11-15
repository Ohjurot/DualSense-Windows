/*
	IO.cpp is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#include <DualSenseWindows/IO.h>
#include <DualSenseWindows/IO_BT.h>
#include <DualSenseWindows/IO_USB.h>

#define NOMINMAX

#include <Windows.h>
#include <malloc.h>

#include <initguid.h>
#include <Hidclass.h>
#include <SetupAPI.h>
#include <hidsdi.h>

DS5W_API DS5W_ReturnValue DS5W::enumDevices(void* ptrBuffer, unsigned int inArrLength, bool pointerToArray, unsigned int* requiredLength) {
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
	size_t devIndex = 0;
	SP_DEVINFO_DATA hidDiInfo;
	hidDiInfo.cbSize = sizeof(SP_DEVINFO_DATA);
	while (SetupDiEnumDeviceInfo(hidDiHandle, devIndex, &hidDiInfo)) {
		
		// Enumerate over all hid device interfaces
		size_t ifIndex = 0;
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
				if (vendorId == 0x054C && productId == 0x0CE6) {
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
	unsigned short inputReportLength = 0;
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		// The bluetooth input report is 78 Bytes long
		inputReportLength = 78;
	}
	else {
		// The usb input report is 64 Bytes long
		inputReportLength = 64;
	}

	// TODO: Replace with proper memory allocation routine
	ptrContext->_internal.hidBuffer = (unsigned char*)calloc(inputReportLength, sizeof(unsigned char));
	
	// Return OK
	return DS5W_OK;
}

DS5W_API void DS5W::freeDeviceContext(DS5W::DeviceContext* ptrContext) {
	// Unset handle
	if (ptrContext->_internal.deviceHandle) {
		CloseHandle(ptrContext->_internal.deviceHandle);
		ptrContext->_internal.deviceHandle = NULL;
	}
	
	// Unset bool
	ptrContext->_internal.connected = false;

	// Unset string
	ptrContext->_internal.devicePath[0] = 0x0;

	//Plug that leak
	free(ptrContext->_internal.hidBuffer);
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
	}
	else {
		// The usb input report is 64 Bytes long
		inputReportLength = 64;
	}

	// Get device input
	ptrContext->_internal.hidBuffer[0] = 0x01;
	if (!ReadFile(ptrContext->_internal.deviceHandle, ptrContext->_internal.hidBuffer, inputReportLength, NULL, NULL)) {
		// Close handle and set error state
		CloseHandle(ptrContext->_internal.deviceHandle);
		ptrContext->_internal.deviceHandle = NULL;
		ptrContext->_internal.connected = false;

		// Return error
		return DS5W_E_DEVICE_REMOVED;
	}

	// Get pointer for easy access
	unsigned char* buffer = ptrContext->_internal.hidBuffer;

	// Evaluete input buffer
	if (ptrContext->_internal.connection == DS5W::DeviceConnection::BT) {
		// Call bluetooth evaluator if connection is qual to BT
		__DS5W::BT::evaluateHidInputBuffer(buffer, ptrInputState);
	} else {
		// Else it is USB so call its evaluator
		__DS5W::USB::evaluateHidInputBuffer(buffer, ptrInputState);
	}
	

	// Return ok
	return DS5W_OK;
}