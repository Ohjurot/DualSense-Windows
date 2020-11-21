/*
	DS_CRC32.h is part of DualSenseWindows
	https://github.com/Ohjurot/DualSense-Windows

	Contributors of this file:
	11.2020 Ludwig Füchsl

	Licensed under the MIT License (To be found in repository root directory)
*/
#pragma once

#include <DualSenseWindows/DSW_Api.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/DS5State.h>

#include <Windows.h>

namespace __DS5W {
	/// <summary>
	/// Pre gennerated CRC hashing for DS5
	/// </summary>
	class CRC32 {
	private:
		/// <summary>
		/// Fast lookup precalculated byte crc hashes
		/// </summary>
		const static UINT32 hashTable[256];

		/// <summary>
		/// Start seed for crc hash
		/// </summary>
		const static UINT32 crcSeed;


	public:
		/// <summary>
		/// Compute the CRC32 Hash
		/// </summary>
		/// <param name="buffer">Input buffer</param>
		/// <param name="len">Length of buffer</param>
		/// <returns>Computed crc value</returns>
		static UINT32 compute(unsigned char* buffer, size_t len);
	};
}