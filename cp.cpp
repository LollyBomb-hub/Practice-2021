#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <sstream>
#include <codecvt>
#include <windows.h>


int main(void)
{
	std::wofstream out(L"test.txt");
	out.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
	std::wstring data = L"Data / Данные";
	std::wcout << data;
	if(out.is_open())
	{
		out << data;
		std::wcout << "\nReady!";
	}
	return 0;
}
