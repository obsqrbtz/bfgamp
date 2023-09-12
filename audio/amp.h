#pragma once

#include <iostream>
#include <cstdlib>
#include "effects.h"
#include "../lib/bass/bass.h"

#ifdef _WIN32
#include "../lib/bass/bassasio.h"
#pragma comment(lib, "../lib/bass/win64/bass.lib") 
#pragma comment(lib, "../lib/bass/win64/bassasio.lib")
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
/* Linux part does not work at all, keeping it here just to check meson and do not forget the macros */
#elif __linux__ 

BASS_INFO info;
HRECORD rchan;		// recording channel
HSTREAM chan;		// playback stream
int device = 1;
int rec_device = 0;
int input = 0;			// current input source
float volume = 1.0;	// volume level

#define DEFAULTRATE 44100
#define BUFF_SIZE 256

void Error(const char *es)
{
	char mes[200];
	printf(mes, "%s\n(error code: %d)", es, BASS_ErrorGetCode());
}

// STREAMPROC that pulls data from the recording
DWORD CALLBACK StreamProc(HSTREAM handle, void *buffer, DWORD length, void *user)
{
	DWORD c = BASS_ChannelGetData(rchan, buffer, length);
	return c;
}

BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	short gain = 1000;
	short* s = (short*)buffer;
	for (int i = 0; i < BUFF_SIZE; i++) {
		s[i] = TanhDist(s[i], gain);
	}
	return TRUE; // continue recording
}

static void StartAmpLinux() {
	std::cout << "Devices info:" << std::endl;
	DWORD a = 0; int count = 0;
	BASS_DEVICEINFO info;
	printf("Devices:\n");
	for (a = 0; BASS_GetDeviceInfo(a, &info); a++)
	{
		if (info.flags & BASS_DEVICE_ENABLED)
		{
			printf("%d", count);
			printf(" - ");
			printf(info.name);
			printf("\n");
        	count++;
		}
	}
	BASS_Init(device, DEFAULTRATE, 0, 0, NULL);

	int c;
	BASS_DEVICEINFO di;
	printf("Rec devices:\n");
	for (c = 0; BASS_RecordGetDeviceInfo(c, &di); c++) {
		printf("%d", c);
		printf(" - ");
		printf(di.name);
		printf("\n");
	}

	const char *name;
	printf("In:\n");
	for (c = 0; name = BASS_RecordGetInputName(c); c++) {
		printf("%d", c);
		printf(" - ");
		printf(name);
		printf("\n");
	}
	BASS_RecordSetInput(input, BASS_INPUT_ON, -1);
	BASS_RecordInit(rec_device);
	BASS_SetConfig(BASS_CONFIG_DEV_NONSTOP, 1);

	chan = BASS_StreamCreate(DEFAULTRATE, 2, BASS_SAMPLE_FLOAT, StreamProc, 0);
	BASS_ChannelSetAttribute(chan, BASS_ATTRIB_BUFFER, 0); // disable playback buffering
	BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volume); // set volume level

	BASS_SetConfig(BASS_CONFIG_REC_BUFFER, BUFF_SIZE); // don't need/want a big recording buffer

	// record without a RECORDPROC so output stream can pull data from it
	rchan = BASS_RecordStart(DEFAULTRATE, 2, BASS_SAMPLE_FLOAT, NULL, 0);

	BASS_ChannelPlay(chan, FALSE); // start the output

	char g = getchar();

	BASS_Stop();
	BASS_Free();
}
#endif