#include "pch.h"
#include "vibend.h"
#include <string>

namespace Vibend
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole;
	uint16_t screenwidth = 0;
	uint16_t screenheight = 0;
	bool visible = false;

	uint8_t init()
	{
		// Get console handle
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE) {
			// Failed to get console handle
			return 1;
		}

		// Enable Virtual Terminal Processing
		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) {
			// Failed to get console mode
			return 2;
		}

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode)) {
			// Failed to set console mode
			return 3;
		}

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		checksize();

		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		visible = cursorInfo.bVisible;

		return 0;
	}

	void version()
	{
		printf("Vibend lib %i", VIBEND_VERSION);
	}

	void checksize()
	{
		if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
			screenwidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			screenheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			setcursor(visible);
		}
	}

	void setcursor(bool isvisible)
	{
		visible = isvisible;
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = visible;
		SetConsoleCursorInfo(hConsole, &cursorInfo);
	}

	namespace Literals
	{
		uint16_t operator"" _vw(unsigned long long n) {
			return (uint16_t)n * screenwidth / 100;
		}

		uint16_t operator"" _vh(unsigned long long n) {
			return (uint16_t)n * screenheight / 100;
		}
	}
}
