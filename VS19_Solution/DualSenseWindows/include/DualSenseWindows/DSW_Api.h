/*
    DSW_Api.h is part of DualSenseWindows
    https://github.com/Ohjurot/DualSense-Windows

    Contributors of this file:
    11.2020 Ludwig Füchsl

    Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#if defined(DS5W_BUILD_DLL)
    #define DS5W_API __declspec(dllexport)
#elif defined(DS5W_BUILD_LIB) || defined(DS5W_USE_LIB)
    #define DS5W_API
#else
    #define DS5W_API __declspec(dllimport)
#endif

#define DS5W_SUCCESS(expr) ((expr) == DS5W_ReturnValue::OK)
#define DS5W_FAILED(expr) ((expr) != DS5W_ReturnValue::OK)

#define DS5W_OK DS5W_ReturnValue::OK
#define DS5W_E_UNKNOWN DS5W_ReturnValue::E_UNKNOWN
#define DS5W_E_INSUFFICIENT_BUFFER DS5W_ReturnValue::E_INSUFFICIENT_BUFFER
#define DS5W_E_EXTERNAL_WINAPI DS5W_ReturnValue::E_EXTERNAL_WINAPI
#define DS5W_E_STACK_OVERFLOW DS5W_ReturnValue::E_STACK_OVERFLOW
#define DS5W_E_INVALID_ARGS DS5W_ReturnValue::E_INVALID_ARGS
#define DS5W_E_CURRENTLY_NOT_SUPPORTED DS5W_ReturnValue::E_CURRENTLY_NOT_SUPPORTED
#define DS5W_E_DEVICE_REMOVED DS5W_ReturnValue::E_DEVICE_REMOVED
#define DS5W_E_BT_COM DS5W_ReturnValue::E_BT_COM

/// <summary>
/// Enum for return values
/// </summary>
typedef enum class DS5W_ReturnValue : unsigned int {
    /// <summary>
    /// Operation completed without an error
    /// </summary>
    OK = 0,

    /// <summary>
    /// Operation encountered an unknown error
    /// </summary>
    E_UNKNOWN = 1,

    /// <summary>
    /// The user supplied buffer is too small
    /// </summary>
    E_INSUFFICIENT_BUFFER = 2,

    /// <summary>
    /// External unexpected WinAPI error (please report as issue if you get this error!)
    /// </summary>
    E_EXTERNAL_WINAPI = 3,

    /// <summary>
    /// Not enough memory on the stack
    /// </summary>
    E_STACK_OVERFLOW = 4,

    /// <summary>
    /// Invalid arguments
    /// </summary>
    E_INVALID_ARGS = 5,

    /// <summary>
    /// This feature is currently not supported
    /// </summary>
    E_CURRENTLY_NOT_SUPPORTED = 6,

    /// <summary>
    /// Device was disconnected
    /// </summary>
    E_DEVICE_REMOVED = 7,

    /// <summary>
    /// Bluetooth communication error
    /// </summary>
    E_BT_COM = 8,

} DS5W_RV;
