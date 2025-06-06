/*

This is vibend
It stands for vivid bend. Which means to bend console so it would be vivid.
It is heavily inspired by ncurses but it has more content

Currently (5/3/2025) this library is so weak.
It has some basic mechanizms (including printing, ANSI Escape Codes, etc).
And also some vivid classes (including progress bar, panel, text box, etc).
But it doesn't has support for linux and wide unsigned char.
So it is under development...

*/

#pragma once

#include "pch.h"

#ifdef VIBEND_API

#ifndef VIBEND_VERSION
#define VIBEND_VERSION 25002
#endif // !VIBEND_VERSION


#include "bender/rawbender.h"
#include "bender/vivids.h"

// TODO Implement the Wide mode (Ahhh, this is hard; Basicly remaking all of the functions and variables with their wide mode)

namespace Vibend
{
	// The width of the screen (Use checksize to update)
	VIBEND_API extern uint16_t screenwidth;
	// The height of the screen (Use checksize to update)
	VIBEND_API extern uint16_t screenheight;
	// Cursor visibility
	VIBEND_API extern bool visible;

	// Sets the window, enables ansi code
	VIBEND_API uint8_t init();
	// Prints the version (to test if the module is installed)
	VIBEND_API void version();
	// Updates/Calculates the width and height of the screen
	VIBEND_API void checksize();
	// Hides/Shows the console cursor
	VIBEND_API void setcursor(bool isvisible);

	namespace Literals
	{
		VIBEND_API uint16_t operator"" _vw(unsigned long long n);
		VIBEND_API uint16_t operator"" _vh(unsigned long long n);
	}
}

#endif // VIBEND_API