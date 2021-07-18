#ifndef _GUI_DLL_H_
#define _GUI_DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif


#include "xml/xml.h"

#include <locale>
#include <string>
#include <Windows.h>


typedef enum
{
	SuccessNoActionRequired,
	OpenMenu,
	BackMenu,
	Fail
} RequestCode;

typedef struct
{
	RequestCode code;
} Request_t;


typedef Request_t (*ButtonClickHandler)(std::map<std::string, void*>*);


class IWindowHandler
{
	public:
		virtual void run() = 0;
		virtual void setScreenSizes(int, int) = 0;
		virtual void setCursorPosition(short int, short int) = 0;
		virtual void setCursorPosition() = 0;
		virtual void setCursorVisibility(bool) = 0;
		virtual void setActionHandler(std::string, ButtonClickHandler) = 0;
		virtual void setActionManager(std::map<std::string, ButtonClickHandler>) = 0;
		virtual void setColor(unsigned char, unsigned char) = 0;
		virtual void utfIsSet(bool) = 0;
		virtual std::wstring stringToWideString(const std::string&) = 0;
		virtual void clear() = 0;
};

class Window: public IWindowHandler
{
	public:
		Window(std::string);
		void run();
		void setScreenSizes(int, int);
		void setCursorPosition(short int, short int);
		void setCursorPosition();
		void setCursorVisibility(bool);
		void setActionHandler(std::string, ButtonClickHandler);
		void setActionManager(std::map<std::string, ButtonClickHandler>);
		void setColor(unsigned char, unsigned char);
		void utfIsSet(bool);
		std::wstring stringToWideString(const std::string&);
		void clear();
	private:
		short int width, height;
		short int defaultCursorX, defaultCursorY;
		short int squareOfConsole;
		char* fmt;
		bool utfIsUsed;
		std::vector<std::string> path;
		std::map<std::string, ButtonClickHandler> actionManager;
		std::map<std::string, void*> objectManager;
		COORD cursorCoord;
		CONSOLE_CURSOR_INFO consoleCursorInfo;
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		HANDLE handleStdOut;
		IXMLHandler* windowDOM;
};


#define IFactoryWindow	"CreateWindowHandle"
#define FactoryWindow	FactoryFunctionWindow
typedef IWindowHandler* (__cdecl *FactoryFunctionWindow)(const char*);

#endif
