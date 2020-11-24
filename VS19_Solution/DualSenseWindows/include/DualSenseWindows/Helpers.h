/*
	Helpers.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#include <DualSenseWindows/DSW_Api.h>
#include <DualSenseWindows/DS5State.h>

namespace DS5W {
	/// <summary>
	/// Convert from 3-Color RGB normalized float to DS5W::Color
	/// </summary>
	/// <param name="r">Red channel</param>
	/// <param name="g">Green channel</param>
	/// <param name="b">Blue channel</param>
	/// <returns>DS5W::Color</returns>
	DS5W_API DS5W::Color color_R32G32B32_FLOAT(float r, float g, float b);
	
	/// <summary>
	/// Convert from 4-Color RGBA normalized  float to DS5W::Color
	/// </summary>
	/// <param name="r">Red channel</param>
	/// <param name="g">Green channel</param>
	/// <param name="b">Blue channel</param>
	/// <param name="a">Alpha channel</param>
	/// <returns>DS5W::Color</returns>
	DS5W_API DS5W::Color color_R32G32B32A32_FLOAT(float r, float g, float b, float a);

	/// <summary>
	/// Convert from 4-Color RGBA byte / UChar to DS5W::Color
	/// </summary>
	/// <param name="r">Red channel</param>
	/// <param name="g">Green channel</param>
	/// <param name="b">Blue channel</param>
	/// <param name="a">Alpha channel</param>
	/// <returns>DS5W::Color</returns>
	DS5W_API DS5W::Color color_R8G8B8A8_UCHAR(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	/// <summary>
	/// Convert from 4-Color RGB byte / UChar to DS5W::Color while using a normalized float for alpha chanel
	/// </summary>
	/// <param name="r">Red channel</param>
	/// <param name="g">Green channel</param>
	/// <param name="b">Blue channel</param>
	/// <param name="a">Alpha channel</param>
	/// <returns>DS5W::Color</returns>
	DS5W_API DS5W::Color color_R8G8B8_UCHAR_A32_FLOAT(unsigned char r, unsigned char g, unsigned char b, float a);
}
