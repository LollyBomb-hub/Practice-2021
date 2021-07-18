#include <clocale>
#include <conio.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>


#include "libGUI.h"
#include "common_object.h"


#define ARROW_UP	0x48
#define ARROW_DOWN	0x50
#define HOME		0x47
#define END			0x4F
#define ENTER		0x0D
#define ESC			0x1B


void Window::setScreenSizes(int x, int y)
{
	if(this->handleStdOut == INVALID_HANDLE_VALUE)
		return;

	COORD coord;
    coord.X = x;
    coord.Y = y;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = x-1;
	Rect.Right = y-1;

	SetConsoleScreenBufferSize(this->handleStdOut, coord);
	SetConsoleWindowInfo(this->handleStdOut, TRUE, &Rect);
	return;
}


void Window::setCursorPosition(short int x, short int y)
{
	COORD pos = {x,y};
	SetConsoleCursorPosition(this->handleStdOut, pos);
	return;
}


void Window::setCursorPosition()
{
	SetConsoleCursorPosition(this->handleStdOut, this->cursorCoord);
	return;
}


void Window::setCursorVisibility(bool visibility)
{
	GetConsoleCursorInfo(this->handleStdOut, &this->consoleCursorInfo);
	consoleCursorInfo.bVisible = visibility;
	SetConsoleCursorInfo(this->handleStdOut, &this->consoleCursorInfo);
	return;
}


void Window::setColor(unsigned char text, unsigned char background)
{
	SetConsoleTextAttribute(this->handleStdOut, (WORD)((background << 4) | text));
	return;
}


void Window::setActionHandler(std::string actionID, ButtonClickHandler handler)
{
	this->actionManager[actionID] = handler;
	return;
}


void Window::setActionManager(std::map<std::string, ButtonClickHandler> newManager)
{
	this->actionManager = newManager;
	return;
}


void Window::utfIsSet(bool isSet)
{
	this->utfIsUsed = isSet;
	return;
}


std::wstring Window::stringToWideString( const std::string& str )
{
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t> >(wstm.getloc());
	for(size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
    return wstm.str();
}


void Window::clear()
{
	this->setCursorPosition(0, 0);
	DWORD written;
	FillConsoleOutputCharacter(this->handleStdOut, ' ', this->squareOfConsole,
								{0, 0}, &written);
	FillConsoleOutputAttribute(this->handleStdOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
								this->squareOfConsole,
								{0, 0}, &written);
	usleep(20000);
	this->setCursorPosition();
	return;
}


Window::Window(std::string filename)
{
	this->handleStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if(this->handleStdOut == INVALID_HANDLE_VALUE)
		throw new std::runtime_error("Error getting console descriptor!");
    if(GetConsoleScreenBufferInfo(this->handleStdOut, &this->console_info))
	{
		this->width = this->console_info.srWindow.Right - this->console_info.srWindow.Left + 1;
		this->height = this->console_info.srWindow.Bottom - this->console_info.srWindow.Top + 1;
	}
	else
		throw new std::runtime_error("Error getting console sizes!");
	this->windowDOM = parseXML(filename.c_str());
	if(this->windowDOM->getCurrentLevel().size() == 0)
		throw new std::logic_error("No elements!");
	this->path = std::vector<std::string>(1, "MainWindow");
	this->setScreenSizes(this->width, this->height);
	this->defaultCursorX = 20;
	this->defaultCursorY = 0;
	this->cursorCoord = {this->defaultCursorX, this->defaultCursorY};
	this->setCursorVisibility(false);
	this->setCursorPosition();
	this->squareOfConsole = this->console_info.dwSize.X * this->console_info.dwSize.Y;
	return;
}


void Window::run()
{
	this->fmt = (char*)malloc(this->width - 2*this->defaultCursorX);
	strcpy(fmt, " %2d) %-");
	if(this->utfIsUsed)
		sprintf(fmt + 8, "%dls ", this->width - 2*this->defaultCursorX - 5);
	else
		sprintf(fmt + 8, "%ds ", this->width - 2*this->defaultCursorX - 5);
	int current_selected = 0;
	IXMLHandler* CurrentDom = this->windowDOM->getLevelByPath(this->windowDOM, this->path);
	if(!CurrentDom)
		throw new std::runtime_error("Menu is nulled!");
	if(CurrentDom->getType() != "menu")
		throw new std::logic_error("No menu defined!");
	std::vector<IXMLHandler*> level = CurrentDom->getCurrentLevel();
	while(true)
	{
		clear();
		for(size_t i = 0; i < level.size(); i++)
		{
			this->setCursorPosition(this->cursorCoord.X, this->cursorCoord.Y + i + 1);
			if((unsigned int)current_selected == i)
				this->setColor(15, 1);
			else
				this->setColor(15, 7);
			std::string menuelement = level[i]->getText();
			if(this->utfIsUsed)
				wprintf(this->stringToWideString(this->fmt).c_str(), (unsigned short)(i + 1), this->stringToWideString(menuelement.c_str()).c_str());
			else
				printf(this->fmt, (unsigned char)(i + 1), menuelement.c_str());
		}
		unsigned int code = getch();
		if(code == 224)
			code = getch();
		switch(code)
		{
			case ARROW_UP:
				current_selected--;
				if(current_selected < 0)
					current_selected = (int)level.size() - 1;
				break;
			case ARROW_DOWN:
				current_selected++;
				if((unsigned int)current_selected >= level.size())
					current_selected = 0;
				break;
			case HOME:
				current_selected = 0;
				break;
			case END:
				current_selected = (int)level.size() - 1;
				break;
			case ENTER:
				if(level[current_selected]->getType() == "menu")
				{
					path.push_back(level[current_selected]->getName());
					this->run();
				}
				else if(level[current_selected]->getType() == "menuelement")
				{
					std::string action = level[current_selected]->getProperty("action");
					if(action.size() > 0)
					{
						if(level[current_selected]->getProperty("isDefault") == "true")
						{
							if(action == "Back")
							{
								this->path.pop_back();
								return;
							}
							if(action == "Exit")
							{
								this->setColor(7, 0);
								exit(0);
							}
						}
						if(this->actionManager[action] == NULL)
						{
							throw new std::runtime_error("No action found for button");
						}
						else
						{
							this->setColor(7, 0);
							this->setCursorPosition(0, 20);
							this->actionManager[action](&this->objectManager);
						}
					}
				}
				break;
			case ESC:
				if(this->path.size() == 1)
				{
					this->setColor(7, 0);
					exit(0);
				}
				else
				{
					this->path.pop_back();
					return;
				}
		}
		this->cursorCoord = {this->defaultCursorX, this->defaultCursorY};
	}
	return;
}


extern "C" __declspec(dllexport) IWindowHandler* __cdecl CreateWindowHandle(const char* filename)
{
	return (IWindowHandler*)(new Window(std::string(filename)));
}
