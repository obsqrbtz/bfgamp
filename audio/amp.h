#pragma once

#include <iostream>
#include <cstdlib>
#include "effects.h"
#include "../lib/bass/bass.h"

#ifdef _WIN32
#include "bassasio.h"
#pragma comment(lib, "bass.lib") 
#pragma comment(lib, "bassasio.lib")
#endif

static DWORD CALLBACK ApplyFx(BOOL input, DWORD channel, void* buffer, DWORD length, void* user)
{
	short gain = 200;
	DWORD c = BASS_ChannelGetData(0, buffer, length); // decode some data
	if (c == -1) c = 0; // an error, no data (could pause the channel at this point) 
	short* s = (short*)buffer;
	for (int i = 0; i < length; i++) {
		s[i] = TanhDist(s[i], gain);
	}
	return c;
}
#ifdef _WIN32
static void StartAmpASIO() {
	std::cout << "ASIO Devices info:" << std::endl;
	DWORD a = 0; int count = 0;
	BASS_ASIO_DEVICEINFO asio_info;
	for (a = 0; BASS_ASIO_GetDeviceInfo(a, &asio_info); a++)
		std::cout << "Device " << a << ") " << asio_info.name << std::endl;
	std::cout << " ________ " << std::endl;

	int asio_device = 1;

	try {
		if (!BASS_Init(asio_device, 44100, 0, 0, NULL))
			throw BASS_ErrorGetCode();
		if (!BASS_ASIO_Init(asio_device, NULL))
			throw BASS_ASIO_ErrorGetCode();
	}
	catch (int err) {
		std::cout << "Err no - " << err << std::endl;
		system("pause");
	}

	a = 0; BASS_ASIO_CHANNELINFO channel_info;
	std::cout << "inputs: " << std::endl;
	for (a = 0; BASS_ASIO_ChannelGetInfo(0, a, &channel_info); a++)
		std::cout << a << ") " << channel_info.name << " format: " << channel_info.format << std::endl;
	std::cout << "Outputs: " << std::endl;
	for (a = 0; BASS_ASIO_ChannelGetInfo(1, a, &channel_info); a++)
		std::cout << a << ") " << channel_info.name << " format: " << channel_info.format << std::endl;
	std::cout << "__________" << std::endl;

	BASS_ASIO_ChannelEnable(TRUE, 0, ApplyFx, 0);

	BASS_ASIO_ChannelEnableMirror(0, 1, 0);
	BASS_ASIO_ChannelEnableMirror(1, 1, 0);

	BASS_ASIO_Start(0, 0);

	system("pause");

	BASS_ASIO_Stop();
	BASS_ASIO_Free();
	BASS_Stop();
	BASS_Free();
}
#endif