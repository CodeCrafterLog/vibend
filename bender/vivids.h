#pragma once

#include "pch.h"

#ifdef VIBEND_API

#include <stdint.h>
#include <vector>
#include "rawbender.h"
#include <memory>

namespace Vibend
{
	// A simple rect struct
	struct VIBEND_API Box
	{
		uint16_t x;
		uint16_t y;
		uint16_t w;
		uint16_t h;

		Box();
		Box(uint16_t left, uint16_t top, uint16_t width, uint16_t height);

		Box inner();
		Box outer();
	};

	// A simple way of drawing a box
	class VIBEND_API Panel
	{
	public:
		Box box;
		unsigned char toprow, botrow, leftcol, rightcol, topleftcorner,
			toprightcorner, botleftcorner, botrightcorner, fill;

		Panel(Box box);

		Panel(Box box, unsigned char row, unsigned char col, unsigned char corners);

		Panel(Box box, unsigned char row, unsigned char col, unsigned char corners, unsigned char fill);

		Panel(Box box, unsigned char row, unsigned char col, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner);

		Panel(Box box, unsigned char row, unsigned char col, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner, unsigned char fill);

		Panel(Box box, unsigned char toprow, unsigned char botrow, unsigned char leftcol, unsigned char rightcol
			, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner);

		Panel(Box box, unsigned char toprow, unsigned char botrow, unsigned char leftcol, unsigned char rightcol
			, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner, unsigned char fill);

		void drawborder();
		void drawcenter();
		// Draws both the border and center
		void draw();
	};

	// A progress bar that can be use at loading screens
	class VIBEND_API ProgressBar
	{
	public:
		Box box;
		unsigned char fill, head, placeholder;
		// Progress is between 0 (0%) up to 1 (100%)
		float progress;
		// The function to be called after teleporting before drawing
		void(*before)();
		// The function to be called after drawing
		void(*after)();

		ProgressBar(Box box, unsigned char fill);
		ProgressBar(Box box, unsigned char fill, unsigned char head);
		ProgressBar(Box box, unsigned char fill, unsigned char head, unsigned char placeholder);
		// Tip: You can use '[]() {Vibend::setforeground(Vibend::Color::GREEN);}' as before and 
		// '[]() {Vibend::resetforeground();}' for after to make the bar colored
		ProgressBar(Box box, unsigned char fill, unsigned char head, unsigned char placeholder, void(*before)(), void(*after)());

		void draw();

		// TODO Implement percentage display (None, Left, Mid, Right)
	};

	// This is a class that can be used for multi-line texts (That use \n) or Auto wrapping
	class VIBEND_API TextBox
	{
	public:
		char* text;
		Box box;
		std::vector<uint16_t>* breaks;
		uint16_t lines;
		// Ther character to put instead of \n
		unsigned char enter = ' ';

		TextBox(Box box, const char* text);
		~TextBox();

		// Checks and updates the number of breaks and lines
		void checkbreaks();
		void draw(uint16_t startline = 0);
	};

	// TODO Find a better name
	class VIBEND_API ItemSelect
	{
	public:
		static constexpr uint8_t SELECT = 0;
		static constexpr uint8_t PREVIOUS = 1;
		static constexpr uint8_t NEXT = 2;
		static constexpr uint8_t NONE = -1;

		Box box;
		std::vector<const char*>* items;
		Color hoverforeground, hoverbackground, normalforeground, normalbackground;

		// If enabled you jump back to 0th index after moving to last index, also reversed
		bool wrapping;
		uint16_t current;

		ItemSelect(Box box, std::vector<const char*>* items);
		ItemSelect(Box box, std::vector<const char*>* items, bool wrapping);
		ItemSelect(Box box, std::vector<const char*>* items, Color normalforeground, Color normalbackground, Color hoverforeground, Color hoverbackground);
		ItemSelect(Box box, std::vector<const char*>* items, Color normalforeground, Color normalbackground, Color hoverforeground, Color hoverbackground, bool wrapping);

		void drawall();

		void hover(uint16_t idx);
		// TODO Find a better name
		void dishover(uint16_t idx);
		void previous();
		void next();
		uint8_t refresh(bool delay = true);
	};

	// TODO implement saver
	// TODO Implement Entity : a class with a 2d array of chars that can print them all (So useless)
}

#endif // VIBEND_API