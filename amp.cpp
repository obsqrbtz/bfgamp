#include "audio/amp.h"

int main() {
#ifdef _WIN32
	StartAmpASIO();
#endif
	int a, count = 0;
	BASS_DEVICEINFO info;
	printf("Devices:\n");
	for (a = 0; BASS_GetDeviceInfo(a, &info); a++)
	{
		if (info.flags & BASS_DEVICE_ENABLED)
		{
			printf(info.name);
			printf("\n");
        	count++;
		}
	}
	return 0;
}