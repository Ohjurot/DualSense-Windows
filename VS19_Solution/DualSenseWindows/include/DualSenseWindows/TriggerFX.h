/*
	TriggerFX.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#define DS5W_TRIGGER_FXTYPE_POS 1

namespace DS5W {
	/// <summary>
	/// Prototype of trigger fx
	/// </summary>
	typedef struct _TriggerFX {
		/// <summary>
		/// Id code of fx type
		/// </summary>
		unsigned char type;
	} TriggerFX;

	/// <summary>
	/// Effect for moving the trigger to a position
	/// </summary>
	typedef struct _TriggerFX_Pos : TriggerFX {
		/// <summary>
		/// Start position of force
		/// </summary>
		unsigned int position;
	} TriggerFX_Pos;
}