/*
    DS5State.h is part of DualSenseWindows
    https://github.com/Ohjurot/DualSense-Windows

    Contributors of this file:
    11.2020 Ludwig Füchsl

    Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#include <cstdint>

// Button and D-pad bitmasks
#define DS5W_ISTATE_BTX_SQUARE 0x10
#define DS5W_ISTATE_BTX_CROSS 0x20
#define DS5W_ISTATE_BTX_CIRCLE 0x40
#define DS5W_ISTATE_BTX_TRIANGLE 0x80
#define DS5W_ISTATE_DPAD_LEFT 0x01
#define DS5W_ISTATE_DPAD_DOWN 0x02
#define DS5W_ISTATE_DPAD_RIGHT 0x04
#define DS5W_ISTATE_DPAD_UP 0x08

// Button A bitmasks
#define DS5W_ISTATE_BTN_A_LEFT_BUMPER 0x01
#define DS5W_ISTATE_BTN_A_RIGHT_BUMPER 0x02
#define DS5W_ISTATE_BTN_A_LEFT_TRIGGER 0x04
#define DS5W_ISTATE_BTN_A_RIGHT_TRIGGER 0x08
#define DS5W_ISTATE_BTN_A_SELECT 0x10
#define DS5W_ISTATE_BTN_A_MENU 0x20
#define DS5W_ISTATE_BTN_A_LEFT_STICK 0x40
#define DS5W_ISTATE_BTN_A_RIGHT_STICK 0x80

// Button B bitmasks
#define DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO 0x01
#define DS5W_ISTATE_BTN_B_PAD_BUTTON 0x02
#define DS5W_ISTATE_BTN_B_MIC_BUTTON 0x04

// Player LED bitmasks
#define DS5W_OSTATE_PLAYER_LED_LEFT 0x01
#define DS5W_OSTATE_PLAYER_LED_MIDDLE_LEFT 0x02
#define DS5W_OSTATE_PLAYER_LED_MIDDLE 0x04
#define DS5W_OSTATE_PLAYER_LED_MIDDLE_RIGHT 0x08
#define DS5W_OSTATE_PLAYER_LED_RIGHT 0x10

namespace DS5W {

    struct AnalogStick {
        int8_t x;  // X Position of stick (0 = Center)
        int8_t y;  // Y Position of stick (0 = Center)
    };

    struct Vector3 {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    struct Touch {
        uint32_t x;       // X position of finger (~ 0 - 2000)
        uint32_t y;       // Y position of finger (~ 0 - 2048)
        bool down;        // Touch is down
        uint8_t id;       // 7-bit ID for touch
    };

    struct Battery {
        bool charging;
        bool fullyCharged;
        uint8_t level;    // Battery charge level 0x0 to 0xA
    };

    enum class MicLed : uint8_t {
        OFF = 0x00,
        ON = 0x01,
        PULSE = 0x02,
    };

    enum class TriggerEffectType : uint8_t {
        NoResistance = 0x00,
        ContinuousResistance = 0x01,
        SectionResistance = 0x02,
        EffectEx = 0x26,
        Calibrate = 0xFC,
    };

    struct TriggerEffect {
        TriggerEffectType effectType;
        union {
            uint8_t _u1_raw[6];
            struct {
                uint8_t startPosition;
                uint8_t force;
                uint8_t _pad[4];
            } Continuous;
            struct {
                uint8_t startPosition;
                uint8_t endPosition;
                uint8_t _pad[4];
            } Section;
            struct {
                uint8_t startPosition;
                bool keepEffect;
                uint8_t beginForce;
                uint8_t middleForce;
                uint8_t endForce;
                uint8_t frequency;
            } EffectEx;
        };
    };

    enum class LedBrightness : uint8_t {
        LOW = 0x02,
        MEDIUM = 0x01,
        HIGH = 0x00,
    };

    struct PlayerLeds {
        uint8_t bitmask;
        bool playerLedFade;
        LedBrightness brightness;
    };

    struct DS5InputState {
        AnalogStick leftStick;
        AnalogStick rightStick;
        uint8_t leftTrigger;
        uint8_t rightTrigger;
        uint8_t buttonsAndDpad;
        uint8_t buttonsA;
        uint8_t buttonsB;
        Vector3 accelerometer;
        Vector3 gyroscope;
        Touch touchPoint1;
        Touch touchPoint2;
        Battery battery;
        bool headPhoneConnected;
        uint8_t leftTriggerFeedback;
        uint8_t rightTriggerFeedback;
    };

    struct DS5OutputState {
        uint8_t leftRumble;
        uint8_t rightRumble;
        MicLed microphoneLed;
        bool disableLeds;
        PlayerLeds playerLeds;
        Color lightbar;
        TriggerEffect leftTriggerEffect;
        TriggerEffect rightTriggerEffect;
    };

} // namespace DS5W
