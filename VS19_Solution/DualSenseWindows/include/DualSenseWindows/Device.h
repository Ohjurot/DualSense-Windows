/*
    Device.h is part of DualSenseWindows
    https://github.com/Ohjurot/DualSense-Windows

    Contributors of this file:
    11.2020 Ludwig Füchsl

    Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

namespace DS5W {
    /// <summary>
    /// Enum for device connection type
    /// </summary>
    enum class DeviceConnection : unsigned char {
        /// <summary>
        /// Controller is connected via USB
        /// </summary>
        USB = 0,

        /// <summary>
        /// Controller is connected via Bluetooth
        /// </summary>
        BT = 1,
    };

    /// <summary>
    /// Struct for storing device enum info during device discovery
    /// </summary>
    struct DeviceEnumInfo {
        /// <summary>
        /// Encapsulate data in struct to (at least try to) prevent user from modifying the context
        /// </summary>
        struct {
            /// <summary>
            /// Path to the discovered device
            /// </summary>
            wchar_t path[260];

            /// <summary>
            /// Connection type of the discovered device
            /// </summary>
            DeviceConnection connection;
        } _internal;
    };

    /// <summary>
    /// Device context
    /// </summary>
    struct DeviceContext {
        /// <summary>
        /// Encapsulate data in struct to (at least try to) prevent user from modifying the context
        /// </summary>
        struct {
            /// <summary>
            /// Path to the device
            /// </summary>
            wchar_t devicePath[260];

            /// <summary>
            /// Handle to the open device
            /// </summary>
            void* deviceHandle;

            /// <summary>
            /// Connection of the device
            /// </summary>
            DeviceConnection connection;

            /// <summary>
            /// Current state of connection
            /// </summary>
            bool connected;

            /// <summary>
            /// HID Input buffer (will be allocated by the context init function)
            /// </summary>
            unsigned char hidBuffer[547];
        } _internal;
    };
}
