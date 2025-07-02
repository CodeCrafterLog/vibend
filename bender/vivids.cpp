#include "pch.h"
#include "vivids.h"
#include "rawbender.h"
#include <string>
#include <stdexcept>

namespace Vibend
{
	#pragma region Box
	Box::Box() : x(0), y(0), w(0), h(0) { }

	Box::Box(uint16_t left, uint16_t top, uint16_t width, uint16_t height)
		: x(left), y(top), w(width), h(height) { }

	Box Box::inner()
	{
		return Box(x + 1, y + 1, w - 2, h - 2);
	}

	Box Box::outer()
	{
		return Box(x - 1, y - 1, w + 2, h + 2);
	}
	#pragma endregion

	#pragma region Panel
	Panel::Panel(Box box)
		: Panel(box, '-', '-', '|', '|', '+', '+', '+', '+', ' ') { }

	Panel::Panel(Box box, unsigned char row, unsigned char col, unsigned char corners)
		: Panel(box, row, row, col, col, corners, corners, corners, corners, ' ') { }

	Panel::Panel(Box box, unsigned char row, unsigned char col, unsigned char corners, unsigned char fill)
		: Panel(box, row, row, col, col, corners, corners, corners, corners, fill) { }

	Panel::Panel(Box box, unsigned char row, unsigned char col, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner)
		: Panel(box, row, row, col, col, topleftcorner, toprightcorner, botleftcorner, botrightcorner, ' ') { }

	Panel::Panel(Box box, unsigned char row, unsigned char col, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner, unsigned char fill)
		: Panel(box, row, row, col, col, topleftcorner, toprightcorner, botleftcorner, botrightcorner, fill) { }

	Panel::Panel(Box box, unsigned char toprow, unsigned char botrow, unsigned char leftcol, unsigned char rightcol
		, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner)
		: Panel(box, toprow, botrow, leftcol, rightcol, topleftcorner, toprightcorner, botleftcorner, botrightcorner, ' ') { }

	Panel::Panel(Box box, unsigned char toprow, unsigned char botrow, unsigned char leftcol, unsigned char rightcol
		, unsigned char topleftcorner, unsigned char toprightcorner, unsigned char botleftcorner, unsigned char botrightcorner, unsigned char fill)
	{
		this->box = box;
		this->toprow = toprow;
		this->botrow = botrow;
		this->leftcol = leftcol;
		this->rightcol = rightcol;
		this->topleftcorner = topleftcorner;
		this->toprightcorner = toprightcorner;
		this->botleftcorner = botleftcorner;
		this->botrightcorner = botrightcorner;
		this->fill = fill;
	}

	void Panel::drawborder()
	{
		if (box.w == 0 || box.h == 0)
			return;

		teleport(box.y, box.x);
		print(topleftcorner);
		if (box.w > 2)
		{
			std::string toprowstr(box.w - 2, toprow);
			print(toprowstr.c_str());
		}
		if (box.w > 1)
		{
			print(toprightcorner);
		}

		for (uint16_t i = 1; i < box.h - 1; i++)
		{
			teleport(box.y + i, box.x);
			print(leftcol);
			teleport(box.y + i, box.x + box.w - 1);
			print(rightcol);
		}

		teleport(box.y + box.h - 1, box.x);
		print(botleftcorner);
		if (box.w > 2)
		{
			std::string botrowstr(box.w - 2, botrow);
			print(botrowstr.c_str());
		}
		if (box.w > 1) {
			print(botrightcorner);
		}

	}

	void Panel::drawcenter()
	{
		if (box.w < 3 || box.h < 3)
			return;

		std::string fillline(box.w - 2, fill);

		for (uint16_t i = 1; i < box.h - 1; i++)
		{
			teleport(box.y + i, box.x + 1);
			print(fillline.c_str());
		}
	}
	
	void Panel::draw()
	{
		drawborder();
		drawcenter();
	}
	#pragma endregion

	#pragma region ProgressBar
	ProgressBar::ProgressBar(Box box, unsigned char fill)
		: ProgressBar(box, fill, '\0', ' ', nullptr, nullptr) { }

	ProgressBar::ProgressBar(Box box, unsigned char fill, unsigned char head)
		: ProgressBar(box, fill, head, ' ', nullptr, nullptr) { }

	ProgressBar::ProgressBar(Box box, unsigned char fill, unsigned char head, unsigned char placeholder)
		: ProgressBar(box, fill, head, placeholder, nullptr, nullptr) { }

	ProgressBar::ProgressBar(Box box, unsigned char fill, unsigned char head, unsigned char placeholder, void(*before)(), void(*after)())
	{
		this->progress = 0;
		this->box = box;
		this->fill = fill;
		this->head = head;
		this->placeholder = placeholder;
		this->before = before;
		this->after = after;
	}

	void ProgressBar::draw()
	{
		progress = min(max(0, progress), 1);

		if (box.w + box.h == 0)
			return;

		uint16_t fillamount = (uint16_t)(box.w * progress);

		std::string filled(fillamount, fill);

		std::string line;
		if (fillamount == box.w || head == '\0') {
			line = filled;
		}
		else {
			std::string left(box.w - fillamount - 1, placeholder);
			line = filled + (char)head + left;
		}

		teleport(box.y, box.x);

		if (before != nullptr)
			before();

		for (uint16_t i = 0; i < box.h; i++)
		{
			print(line.c_str());
			move(1, -box.w);
		}

		if (after != nullptr)
			after();
	}
	#pragma endregion

	#pragma region TextBox
	TextBox::TextBox(Box box, const char* text)
	{
		this->box = box;
		this->text = new char[strlen(text) + 1];  // Allocate memory
		strcpy_s(this->text, strlen(text) + 1, text);  // Copy contents
		

		this->breaks = new std::vector<uint16_t>();

		checkbreaks();
	}

	TextBox::~TextBox()
	{
		delete breaks;
		delete text;
	}

	void addword(uint16_t& x, uint16_t size, uint16_t maxsize, std::vector<uint16_t>* words)
	{
		if (x + size <= maxsize) {
			x += size;
			return;
		}

		words->push_back(x);
		if (size < maxsize) {
			x = size;
			return;
		}

		uint16_t need = size / maxsize;
		for (uint16_t i = 0; i < need; i++) {
			words->push_back(maxsize);
		}

		x = size - (need * maxsize);
	}

	void TextBox::checkbreaks()
	{
		if (text == nullptr)
			return;

		if (breaks == nullptr)
			return;

		breaks->clear();

		uint16_t x = 0;
		uint16_t wordsize = 0;

		for (char* ch = text; *ch != '\0'; ch++) {
			wordsize++;

			if (*ch == '\n') {
				addword(x, wordsize, box.w, breaks);
				breaks->push_back(x);
				wordsize = 0;
				x = 0;
				continue;
			}

			if (*ch != ' ')
				continue;

			addword(x, wordsize, box.w, breaks);
			wordsize = 0;
		}


		if (wordsize)
			addword(x, wordsize, box.w, breaks);

		// The number of lines is dependent on the break points
		// But there might be an extra line at the end so we add one if x is not 0
		lines = (uint16_t)breaks->size() + (x ? 1 : 0);
	}

	void TextBox::draw(uint16_t startline)
	{
		// Stop if there is not enough space
		if (box.w == 0 || box.h == 0)
			return;

		// Check if start line is valid
		if (startline >= lines)
			throw std::invalid_argument("The start line is larger than the height of text");

		char* cur = text;
		uint16_t y = box.y;

		// Continue until we get to the start line
		for (auto it = breaks->cbegin(); it != breaks->cbegin() + startline; ++it) {
			cur += *it;
		}
		
		for (auto it = breaks->cbegin() + startline; it != breaks->cend(); ++it) {
			uint16_t end = *it;
			teleport(y++, box.x);
			for (int x = 0; x < end; x++) {
				if (*cur == '\n')
					print(enter);
				else
					print(*cur);

				cur++;
			}

			// Stop if we are out of bound
			if (y - box.y > box.h - 1) {
				return;
			}
		}

		teleport(y, box.x);
		print(cur);
	}
	#pragma endregion

	#pragma region SelectItem
	ItemSelect::ItemSelect(Box box, std::vector<const char*>* items)
		: ItemSelect(box, items, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, true) { }

	ItemSelect::ItemSelect(Box box, std::vector<const char*>* items, bool wrapping)
		: ItemSelect(box, items, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, wrapping) { }

	ItemSelect::ItemSelect(Box box, std::vector<const char*>* items, Color normalforeground, Color normalbackground, Color hoverforeground, Color hoverbackground)
		: ItemSelect(box, items, normalforeground, normalbackground, hoverforeground, hoverbackground, true) { }

	ItemSelect::ItemSelect(Box box, std::vector<const char*>* items, Color normalforeground, Color normalbackground, Color hoverforeground, Color hoverbackground, bool wrapping)
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");

		this->box = box;
		this->items = items;
		this->normalforeground = normalforeground;
		this->normalbackground = normalbackground;
		this->hoverforeground = hoverforeground;
		this->hoverbackground = hoverbackground;
		this->wrapping = wrapping;
		this->current = 0;
	}

	void ItemSelect::drawall()
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");

		if (!box.h || !box.w || !items->size())
			return;

		for (uint16_t i = 0; i < items->size(); i++)
		{
			teleport(box.y + i, box.x);
			printfull(items->at(i), ' ', box.w);
		}
	}

	void ItemSelect::hover(uint16_t idx)
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");
		
		if (!box.h || !box.w || !items->size())
			return;

		if (idx >= items->size())
			throw std::invalid_argument("The 'idx' is out of range");

		teleport(box.y + idx, box.x);

		setforeground(hoverforeground);
		setbackground(hoverbackground);

		printfull(items->at(idx), ' ', box.w);

		resetforeground();
		resetbackground();
	}

	void ItemSelect::dishover(uint16_t idx)
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");
		
		if (!box.h || !box.w || !items->size())
			return;

		if (idx >= items->size())
			throw std::invalid_argument("The 'idx' is out of range");

		teleport(box.y + idx, box.x);

		setforeground(normalforeground);
		setbackground(normalbackground);

		printfull(items->at(idx), ' ', box.w);

		resetforeground();
		resetbackground();
	}

	void ItemSelect::previous()
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");
		
		if (current == 0 && !wrapping)
			return;

		if (items->size() < 2)
			return;

		dishover(current--);
		if (current == (uint16_t)-1)
			current = (uint16_t)items->size() -1;
		hover(current);
	}

	void ItemSelect::next()
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");

		if (current == items->size() -1 && !wrapping)
			return;

		if (items->size() < 2)
			return;

		dishover(current++);
		if (current == items->size())
			current = 0;
		hover(current);
	}

	uint8_t ItemSelect::refresh(bool delay)
	{
		if (items == nullptr)
			throw std::runtime_error("'items' cannot be nullptr");

		unsigned char ch = getch(delay);

		if (ch == 13)
			return SELECT;

		if (ch != 224)
			return NONE;

		// Get second char
		ch = getch();
		switch (ch)
		{
		case 'H':
			previous();
			return PREVIOUS;
			
		case 'P':
			next();
			return NEXT;

		default:
			break;
		}

		return NONE;
	}
	#pragma endregion
}
