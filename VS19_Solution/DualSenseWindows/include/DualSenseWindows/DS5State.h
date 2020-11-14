/*
	DS5State.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#define DS5W_ISTATE_BTX_SQUARE 0x10
#define DS5W_ISTATE_BTX_CROSS 0x20
#define DS5W_ISTATE_BTX_CIRCLE 0x40
#define DS5W_ISTATE_BTX_TRIANGLE 0x80
#define DS5W_ISTATE_DPAD_LEFT 0x01
#define DS5W_ISTATE_DPAD_DOWN 0x02
#define DS5W_ISTATE_DPAD_RIGHT 0x04
#define DS5W_ISTATE_DPAD_UP 0x08

#define DS5W_ISTATE_BTN_A_LEFT_BUMPER 0x01
#define DS5W_ISTATE_BTN_A_RIGHT_BUMPER 0x02
#define DS5W_ISTATE_BTN_A_LEFT_TRIGGER 0x04
#define DS5W_ISTATE_BTN_A_RIGHT_TRIGGER 0x08
#define DS5W_ISTATE_BTN_A_SELECT 0x10
#define DS5W_ISTATE_BTN_A_MENUE 0x20
#define DS5W_ISTATE_BTN_A_LEFT_STICK 0x40
#define DS5W_ISTATE_BTN_A_RIGHT_STICK 0x80

#define DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO 0x01
#define DS5W_ISTATE_BTN_B_PAD_BUTTON 0x02
#define DS5W_ISTATE_BTN_B_MIC_BUTTON 0x04

namespace DS5W {

	/// <summary>
	/// Analog stick
	/// </summary>
	typedef struct _AnalogStick {
		/// <summary>
		/// X Position of stick (0 = Center)
		/// </summary>
		char x;

		/// <summary>
		/// Y Posistion of stick (0 = Center)
		/// </summary>
		char y;
	} AnalogStick;

	/// <summary>
	/// 3 Component vector
	/// </summary>
	typedef struct _Vec3 {
		short x;
		short y;
		short z;
	} Vector3, Vec3;

	/// <summary>
	/// Touchpad state
	/// </summary>
	typedef struct _Touch {
		/// <summary>
		/// X positon of finger (~ 0 - 2000)
		/// </summary>
		unsigned int x;

		/// <summary>
		/// Y position of finger (~ 0 - 2048)
		/// </summary>
		unsigned int y;
	} Touch;

	/// <summary>
	/// Input state of the controler
	/// </summary>
	typedef struct _DS5InputState {
		/// <summary>
		/// Position of left stick
		/// </summary>
		AnalogStick leftStick;

		/// <summary>
		/// Posisiton of right stick
		/// </summary>
		AnalogStick rightStick;

		/// <summary>
		/// Left trigger position
		/// </summary>
		unsigned char leftTrigger;

		/// <summary>
		/// Right trigger position
		/// </summary>
		unsigned char rightTrigger;

		/// <summary>
		/// Buttons and dpad bitmask DS5W_ISTATE_BTX_?? and DS5W_ISTATE_DPAD_?? indices check with if(buttonsAndDpad & DS5W_ISTATE_DPAD_??)...
		/// </summary>
		unsigned char buttonsAndDpad;

		/// <summary>
		/// Button bitmask A (DS5W_ISTATE_BTN_A_??)
		/// </summary>
		unsigned char buttonsA;

		/// <summary>
		/// Button bitmask B (DS5W_ISTATE_BTN_B_??)
		/// </summary>
		unsigned char buttonsB;

		/// <summary>
		/// Accelerometer
		/// </summary>
		Vector3 accelerometer;

		/// <summary>
		/// Gyroscope  (Currently only raw values will be dispayed! Probably needs calibration (Will be done within the lib in the future))
		/// </summary>
		Vector3 gyroscope;

		/// <summary>
		/// First touch point
		/// </summary>
		Touch touchPoint1;

		/// <summary>
		/// Second touch point
		/// </summary>
		Touch touchPoint2;

		/// <summary>
		/// Indicates the connection of headphone
		/// </summary>
		bool headPhoneConnected;
	} DS5InputState;
}