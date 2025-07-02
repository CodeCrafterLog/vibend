#include "pch.h"
#include "rawbender.h"
#include <iostream>
#include <stdexcept>
#include <stdarg.h>
#include <stdio.h>
#include <conio.h>

namespace Vibend
{
    bool reserve = false;
    // Make a reservation array
    char reserved[CONSOLE_MAX_SIZE];

    void print(const unsigned char ch)
    {
        printf("%c", ch);
    }

    void print(const char* text)
    {
        if (!text)
            throw std::invalid_argument("Cannot print a nullptr");

        std::cout << text;
    }

    void printf(const char* format, ...)
    {
        if (!format)
            throw std::invalid_argument("Cannot print a nullptr");

        va_list args;
        va_start(args, format);

        if (reserve) {
            // Save print in the reservations
            size_t offset = strlen(reserved);
            vsnprintf(reserved + offset, CONSOLE_MAX_SIZE - offset, format, args);
        } else {
            // Print it
            vprintf(format, args);
        }
        va_end(args);
    }

    void printreserve()
    {
        std::cout << reserved;
        // Clear the reservations
        memset(reserved, 0, CONSOLE_MAX_SIZE);
    }

    void printclamp(const char* ch, uint16_t maxsize)
    {
        for (uint16_t i = 0; *ch && i < maxsize; i++) {
            print(*ch);
            ch++;
        }
    }

    void printfull(const char* ch, const unsigned char fill, uint16_t size)
    {
        printclamp(ch, size);
        int16_t left = size - (uint16_t)std::strlen(ch);
        if (left <= 0)
            return;
        
        print(std::string(left, fill).c_str());
    }

    void savepos()
    {
        print("\x1b[s");
    }

    void loadpos()
    {
        print("\x1b[u");
    }
    
    void teleport(uint16_t y, uint16_t x)
    {
        printf("\x1b[%i;%iH", y, x);
    }

    void moverows(int16_t y)
    {
        if (y == 0)
            return;

        if (y < 0)
            printf("\x1b[%iA", -y);

        else
            printf("\x1b[%iB", y);
    }
    
    void movecols(int16_t x)
    {
        if (x == 0)
            return;

        if (x > 0)
            printf("\x1b[%iC", x);

        else
            printf("\x1b[%iD", -x);
    }

    void move(int16_t y, int16_t x)
    {
        moverows(y);
        movecols(x);
    }

    void setformat(Format format)
    {
        printf("\x1b[%im", format == Format::REVERSE ? 7 : static_cast<int>(format) + 1);
    }

    void resetstyle()
    {
        print("\x1b[0m");
    }

    void resetforeground()
    {
        print("\x1b[39m");
    }

    void setforeground(Color color)
    {
        printf("\x1b[%im", static_cast<int>(color) + 30);
    }

    void setforeground(uint8_t r, uint8_t g, uint8_t b)
    {
        printf("\x1b[38;2;%i;%i;%im", r, g, b);
    }

    void resetbackground()
    {
        print("\x1b[49m");
    }

    void setbackground(Color color)
    {
        printf("\x1b[%im", static_cast<int>(color) + 40);
    }

    void setbackground(uint8_t r, uint8_t g, uint8_t b)
    {
        printf("\x1b[48;2;%i;%i;%im", r, g, b);
    }
    
    void clear()
    {
        print("\x1b[2J");
    }
    
    void clearline()
    {
        print("\x1b[K");
    }
    
    unsigned char getch(bool delay)
    {
        if (delay || _kbhit())
            return _getch(); // Read the key

        return NULL;
    }
}