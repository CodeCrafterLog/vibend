#pragma once

#include "pch.h"

#ifdef VIBEND_API

#include <stdint.h>

#ifndef CONSOLE_MAX_SIZE
// Basicly ( 2 ** 8 ) ** 2 (Is that enought?)
#define CONSOLE_MAX_SIZE 65536
#endif // !ConsoleMaxSize

#ifndef NULL
#define NULL 0
#endif // !NULL



namespace Vibend
{
	enum class VIBEND_API Format
	{
		BOLD,
		DIM,
		ITALIC,
		UNDERLINE,
		REVERSE
	};

	enum class VIBEND_API Color
	{
		BLACK,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE
	};

	// If enabled, reserves the things you print and the prints them when 'printreserve' is called
	VIBEND_API extern bool reserve;

	VIBEND_API void print(const unsigned char ch);
	VIBEND_API void print(const char* text);
	VIBEND_API void printf(const char* format, ...);
	// Clears and prints the reservations
	VIBEND_API void printreserve();
	// Clamps the text to fit inside the maxsize
	VIBEND_API void printclamp(const char* ch, uint16_t maxsize);
	// Prints all text and fills the left space with fill unsigned char
	VIBEND_API void printfull(const char* ch, const unsigned char fill, uint16_t size);

	// Saves the current position, can be loaded by 'loadpos'
	VIBEND_API void savepos();
	// Teleports to last saved position
	VIBEND_API void loadpos();
	// Teleports to the given destination
	VIBEND_API void teleport(uint16_t y, uint16_t x);
	VIBEND_API void moverows(int16_t y);
	VIBEND_API void movecols(int16_t x);
	// Moves y rows and x columns
	VIBEND_API void move(int16_t y, int16_t x);

	// Resets/Removes every style (Color, Format, etc)
	VIBEND_API void resetstyle();

	// Resets the foreground to its default
	VIBEND_API void resetforeground();
	VIBEND_API void setforeground(Color color);
	VIBEND_API void setforeground(uint8_t r, uint8_t g, uint8_t b);
	
	// Resets the background to its default
	VIBEND_API void resetbackground();
	VIBEND_API void setbackground(Color color);
	VIBEND_API void setbackground(uint8_t r, uint8_t g, uint8_t b);

	VIBEND_API void setformat(Format format);

	// Clears all the screen
	VIBEND_API void clear();
	// Clears the whole line (starting from the cursor position)
	VIBEND_API void clearline();

	// Gets the pressed character without echo
	VIBEND_API unsigned char getch(bool delay = true);
}

#endif // VIBEND_API
