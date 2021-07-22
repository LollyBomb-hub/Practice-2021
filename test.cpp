#include <conio.h>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <io.h>
#include <limits>
#include <map>
#include <sstream>
#include <vector>
#include <windows.h>


#include "include/loader.h"


bool utf16 = false;
Factory(Window) CreateGUI;
Factory(CSV) CreateCSV;
IWindowHandler* window;


void clearWcinBuffer()
{
	while(std::wcin.peek() == L'\n')
		std::wcin.get();
	return;
}


size_t toInt(std::wstring str)
{
	size_t result;
	std::wistringstream helper(str);
	helper >> result;
	return result;
}

bool checkEnteredData(std::string type, std::wstring value, ICSVHandler* approriateHandler)
{
	if(type == "price")
		return toInt(value) > 0;
	if(type == "id")
		return approriateHandler->isPresent(toInt(value));
	if(type == "season")
	{
		if(value == L"весна" || value == L"лето" || value == L"осень")
				return true;
	}
	return false;
}

Request_t openFile(std::map<std::string, void*>* objects)
{
	Request_t result;
	result.code = SuccessNoActionRequired;
	window->clear();
	window->setCursorPosition(0, 0);
	std::wstring filename;
	std::wcout << L"Введите полный путь к файлу или имя файла, если он расположен рядом с программой. Файл при этом должен существовать!\n>>> ";
	clearWcinBuffer();
	std::getline(std::wcin, filename);
	ICSVHandler* csv = CreateCSV(filename.c_str(), ';');
	std::pair<std::string, void*> toInsert("DataFile", (void*)csv);
	objects->insert(toInsert);
	return result;
}

Request_t saveFile(std::map<std::string, void*>* objects)
{
	Request_t result;
	if(!(*objects)["DataFile"])
	{
		result.code = Fail;
		return result;
	}
	if(((ICSVHandler*)objects[0]["DataFile"])->saveCSV())
		result.code = SuccessNoActionRequired;
	return result;
}

Request_t closeFile(std::map<std::string, void*>* objects)
{
	Request_t result;
	if((*objects)["DataFile"])
	{
		((ICSVHandler*)objects[0]["DataFile"])->close();
		objects[0]["DataFile"] = NULL;
	}
	result.code = SuccessNoActionRequired;
	return result;
}

Request_t printFile(std::map<std::string, void*>* objects)
{
	Request_t result;
	result.code = Fail;
	if(!(*objects)["DataFile"] || !objects[0]["FormFile"] || !objects[0]["SortFile"])
		return result;
	result.code = SuccessNoActionRequired;
	window->clear();
	window->setCursorPosition(0, 0);
	std::map<size_t, std::vector<std::wstring> > data = ((ICSVHandler*)objects[0]["DataFile"])->getData();
	std::map<size_t, std::vector<std::wstring> > forms = ((ICSVHandler*)objects[0]["FormFile"])->getData();
	std::map<size_t, std::vector<std::wstring> > sorts = ((ICSVHandler*)objects[0]["SortFile"])->getData();
	std::map<size_t, std::vector<std::wstring> >::iterator iterator;
	for(iterator = data.begin(); iterator != data.end(); iterator++)
	{
		std::wcout << iterator->second[0] << ' ';
		if(!checkEnteredData("id", iterator->second[1], (ICSVHandler*)objects[0]["FormFile"]))
		{
			std::wcout << "\nError! No such index in forms db!\n";
			throw new std::logic_error("No such id!");
		}
		std::wcout << forms[toInt(iterator->second[1])][1] << ' ';
		if(!checkEnteredData("id", iterator->second[2], (ICSVHandler*)objects[0]["SortFile"]))
		{
			std::wcout << "\nError! No such index in sorts db!\n";
			throw new std::logic_error("No such id!");
		}
		std::wcout << sorts[toInt(iterator->second[2])][1] << ' ';
		std::wcout << iterator->second[3] << ' ';
		std::wcout << iterator->second[4] << '\n';
	}
	getch();
	return result;
}

Request_t exportFile(std::map<std::string, void*>* objects)
{
	Request_t result;
	result.code = Fail;
	if(!(*objects)["DataFile"] || !objects[0]["FormFile"] || !objects[0]["SortFile"])
		return result;
	result.code = SuccessNoActionRequired;
	window->clear();
	window->setCursorPosition(0, 0);
	std::wstring filename;
	std::wcout << L"Введите полный путь к файлу, в который надо экспортировать данные. Прав доступа должно хватать!\n>>> ";
	clearWcinBuffer();
	std::getline(std::wcin, filename);
	std::wofstream toFile(filename.c_str());
	std::map<size_t, std::vector<std::wstring> > data = ((ICSVHandler*)objects[0]["DataFile"])->getData();
	std::map<size_t, std::vector<std::wstring> > forms = ((ICSVHandler*)objects[0]["FormFile"])->getData();
	std::map<size_t, std::vector<std::wstring> > sorts = ((ICSVHandler*)objects[0]["SortFile"])->getData();
	std::map<size_t, std::vector<std::wstring> >::iterator iterator;
	for(iterator = data.begin(); iterator != data.end(); iterator++)
	{
		toFile << iterator->second[0] << ' ';
		if(!checkEnteredData("id", iterator->second[1], (ICSVHandler*)objects[0]["FormFile"]))
		{
			toFile.close();
			std::wcout << "\nError! No such index in sorts db!\n";
			throw new std::logic_error("No such id!");
		}
		toFile << forms[toInt(iterator->second[1])][1] << ' ';
		if(!checkEnteredData("id", iterator->second[2], (ICSVHandler*)objects[0]["SortFile"]))
		{
			toFile.close();
			std::wcout << "\nError! No such index in sorts db!\n";
			throw new std::logic_error("No such id!");
		}
		toFile << sorts[toInt(iterator->second[2])][1] << ' ';
		toFile << iterator->second[3] << ' ';
		toFile << iterator->second[4] << '\n';
	}
	toFile.close();
	return result;
}

Request_t addRowToFile(std::map<std::string, void*>* objects)
{
	Request_t result;
	result.code = Fail;
	if(!((*objects)["DataFile"]) && !objects[0]["FormFile"] && !objects[0]["SortFile"])
		return result;
	window->clear();
	window->setCursorPosition(0, 0);
	std::vector<std::wstring> values(4);
	std::wcout << L"Введите id вида для записи к занесению: ";
	std::wcin >> values[0];
	if(!checkEnteredData("id", values[0], (ICSVHandler*)objects[0]["FormFile"]))
		return result;
	std::wcout << L"Введите id сорта для записи к занесению: ";
	std::wcin >> values[1];
	if(!checkEnteredData("id", values[1], (ICSVHandler*)objects[0]["SortFile"]))
		return result;
	std::wcout << L"Введите сезон для записи к занесению: ";
	std::wcin >> values[2];
	if(!checkEnteredData("season", values[2], NULL))
		return result;
	std::wcout << L"Введите цену для записи к занесению: ";
	std::wcin >> values[3];
	if(!checkEnteredData("price", values[3], NULL))
		return result;
	((ICSVHandler*)objects[0]["DataFile"])->appendCSV(values);
	return result;
}

Request_t editRowInFile(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = Fail;
	if(!objects[0]["DataFile"])
		return result;
	std::wstring id;
	std::wcout << L"Введите id записи к редактированию: ";
	std::wcin >> id;
	if(!checkEnteredData("id", id, (ICSVHandler*)objects[0]["DataFile"]))
		return result;
	std::vector<std::wstring> values(4);
	std::wcout << L"Введите новый id вида для записи: ";
	std::wcin >> values[0];
	if(!checkEnteredData("id", values[0], (ICSVHandler*)objects[0]["FormFile"]))
		return result;
	std::wcout << L"Введите новый id сорта для записи: ";
	std::wcin >> values[1];
	if(!checkEnteredData("id", values[1], (ICSVHandler*)objects[0]["SortFile"]))
		return result;
	std::wcout << L"Введите новый сезон для записи: ";
	std::wcin >> values[2];
	if(!checkEnteredData("season", values[2], NULL))
		return result;
	std::wcout << L"Введите новую цену для записи: ";
	std::wcin >> values[3];
	if(!checkEnteredData("price", values[3], NULL))
		return result;
	((ICSVHandler*)objects[0]["DataFile"])->updateRow(toInt(id), values);
	return result;
}

Request_t deleteRowFromFile(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = Fail;
	if(!objects[0]["DataFile"])
		return result;
	size_t id;
	std::wcout << L"Введите id записи к удалению: ";
	std::wcin >> id;
	bool flag = ((ICSVHandler*)objects[0]["DataFile"])->deleteRowById(id);
	if(flag)
		result.code = SuccessNoActionRequired;
	return result;
}

Request_t chooseManual(std::map<std::string, void*>* objects)
{
	Request_t result;
	result.code = Fail;
	window->clear();
	window->setCursorPosition(0, 0);
	window->setCursorVisibility(true);
	unsigned short type;
	std::wstring filename;
	std::wcout << L"Введите тип справочника(1 - вид, 2 - сорт): ";
	std::wcin >> type;
	std::wcout << L"Введите полный путь к файлу или имя файла, если он расположен рядом с программой. Файл при этом должен существовать!\n>>> ";
	clearWcinBuffer();
	std::getline(std::wcin, filename);
	ICSVHandler* csv = CreateCSV(filename.c_str(), ';');
	switch(type)
	{
		case 1:
			objects[0]["FormFile"] = (void*)csv;
			break;
		case 2:
			objects[0]["SortFile"] = (void*)csv;
			break;
		default:
			return result;
	}
	result.code = SuccessNoActionRequired;
	return result;
}

Request_t printManual(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = Fail;
	if(!objects[0]["FormFile"] || !objects[0]["SortFile"])
		return result;
	window->clear();
	window->setCursorPosition(0, 0);
	unsigned short type;
	std::wcout << L"Введите тип справочника(1 - вид, 2 - сорт): ";
	std::wcin >> type;
	std::map<size_t, std::vector<std::wstring> > data;
	switch(type)
	{
		case 1:
			data = ((ICSVHandler*)objects[0]["FormFile"])->getData();
			break;
		case 2:
			data = ((ICSVHandler*)objects[0]["SortFile"])->getData();
			break;
		default:
			return result;
	}
	std::map<size_t, std::vector<std::wstring> >::iterator iterator;
	for(iterator = data.begin(); iterator != data.end(); iterator++)
		std::wcout << iterator->first << L' ' << iterator->second[1] << L'\n';
	if(getch() == 224)
		getch();
	return result;
}

Request_t addRowToManual(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = Fail;
	if(!objects[0]["FormFile"] && !objects[0]["SortFile"])
		return result;
	window->clear();
	window->setCursorPosition(0, 0);
	unsigned short type;
	std::wcout << L"Введите тип справочника(1 - вид, 2 - сорт): ";
	std::wcin >> type;
	std::vector<std::wstring> values(1);
	std::wcout << L"Введите название ";
	switch(type)
	{
		case 1:
			std::wcout << L"вида: ";
			break;
		case 2:
			std::wcout << L"сорта: ";
			break;
		default:
			return result;
	}
	clearWcinBuffer();
	std::getline(std::wcin, values[0]);
	switch(type)
	{
		case 1:
			((ICSVHandler*)objects[0]["FormFile"])->appendCSV(values);
			break;
		case 2:
			((ICSVHandler*)objects[0]["SortFile"])->appendCSV(values);
			break;
		default:
			return result;
	}
	result.code = SuccessNoActionRequired;
	return result;
}

Request_t editRowInManual(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = Fail;
	if(!objects[0]["FormFile"] && !objects[0]["SortFile"])
		return result;
	window->clear();
	window->setCursorPosition(0, 0);
	unsigned short type;
	std::wcout << L"Введите тип справочника(1 - вид, 2 - сорт): ";
	std::wcin >> type;
	size_t id;
	std::vector<std::wstring> values(1);
	std::wcout << L"Введите id записи ";
	std::wcin >> id;
	std::wcout << L"Введите название ";
	switch(type)
	{
		case 1:
			std::wcout << L"вида: ";
			break;
		case 2:
			std::wcout << L"сорта: ";
			break;
		default:
			return result;
	}
	clearWcinBuffer();
	std::getline(std::wcin, values[0]);
	switch(type)
	{
		case 1:
			((ICSVHandler*)objects[0]["FormFile"])->updateRow(id, values);
			break;
		case 2:
			((ICSVHandler*)objects[0]["SortFile"])->updateRow(id, values);
			break;
		default:
			return result;
	}
	result.code = SuccessNoActionRequired;
	return result;
}

Request_t deleteRowFromManual(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = Fail;
	if(!objects[0]["FormFile"] && !objects[0]["SortFile"])
		return result;
	window->clear();
	window->setCursorPosition(0, 0);
	unsigned short type;
	size_t id;
	std::wcout << L"Введите тип справочника(1 - вид, 2 - сорт): ";
	std::wcin >> type;
	std::wcout << L"Введите id записи к удалению: ";
	std::wcin >> id;
	switch(type)
	{
		case 1:
			((ICSVHandler*)objects[0]["FormFile"])->deleteRowById(id);
			break;
		case 2:
			((ICSVHandler*)objects[0]["SortFile"])->deleteRowById(id);
			break;
		default:
			return result;
	}
	result.code = SuccessNoActionRequired;
	return result;
}

Request_t about(std::map<std::string, void*>* objects)
{
	(void)objects;
	Request_t result;
	result.code = SuccessNoActionRequired;
	window->clear();
	window->setCursorPosition(0, 0);
	std::wcout << L"Выполнено Артёмом Романовичем Пестеревым!";
	getch();
	return result;
}

Request_t exit(std::map<std::string, void*>* objects)
{
	Request_t result;
	std::map<std::string, void*>::iterator iterator;
	for(iterator = objects->begin(); iterator != objects->end(); iterator++)
	{
		if(iterator->first == "DataFile")
			((ICSVHandler*)iterator->second)->saveCSV();
		if(iterator->first == "FormFile")
			((ICSVHandler*)iterator->second)->saveCSV();
		if(iterator->first == "SortFile")
			((ICSVHandler*)iterator->second)->saveCSV();
	}
	result.code = SuccessNoActionRequired;
	exit(0);
	return result;
}

int main(void)
{
	
	utf16 ^= true;
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin),  _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	// ***Loading all dlls***

	// This peace of code loads Factory Functions for each dll
	loadDLLFunction(CreateCSV, CSV, "./CSV/CSV.dll")
	loadDLLFunction(CreateGUI, Window, "./libGUI/libGUI.dll")
	// Action Manager setup;

	std::map<std::string, ButtonClickHandler> actions;
	actions.insert(std::pair<std::string, ButtonClickHandler>("OpenFile", openFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("SaveFile", saveFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("CloseFile", closeFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("AddRowToFile", addRowToFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("DeleteRowFromFile", deleteRowFromFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("EditRowInFile", editRowInFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("PrintFile", printFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("ExportFile", exportFile));
	actions.insert(std::pair<std::string, ButtonClickHandler>("ChooseManual", chooseManual));
	actions.insert(std::pair<std::string, ButtonClickHandler>("PrintManual", printManual));
	actions.insert(std::pair<std::string, ButtonClickHandler>("EditRowInManual", editRowInManual));
	actions.insert(std::pair<std::string, ButtonClickHandler>("AddRowToManual", addRowToManual));
	actions.insert(std::pair<std::string, ButtonClickHandler>("DeleteRowFromManual", deleteRowFromManual));
	actions.insert(std::pair<std::string, ButtonClickHandler>("About", about));
	actions.insert(std::pair<std::string, ButtonClickHandler>("Exit", exit));
	
	// Sets up window from xml configuration file
	window = CreateGUI("interface.xml");
	window->utfIsSet(utf16);
	window->setActionManager(actions);
	window->run();
	return 0;
}
