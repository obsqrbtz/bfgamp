#include "audio/amp.h"

int main() {
#ifdef _WIN32
	StartAmpASIO();
#elif __linux__ 
	StartAmpLinux();
#endif
}