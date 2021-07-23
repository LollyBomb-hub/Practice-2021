#include "csv.h"


#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>



CSV::CSV(const wchar_t* filename, const char delimeter)
{
	this->filename = std::wstring(filename);
	this->separatedValueHelper = CreateSVHandle(delimeter);
	this->readCSV();
	return;
}


bool CSV::readCSV()
{
	this->last_id = 1;
	size_t id;
	std::wstring line;
	std::wifstream in;
	in.open(this->filename);
	if(!in)
		throw new std::runtime_error("No such file!");
	while(std::getline(in, line))
	{
		if(line.size())
		{
			std::vector<std::wstring> values = this->separatedValueHelper->GetValues(line);
			std::wistringstream convert(values[0]);
			convert >> id;
			if(this->isPresent(id))
				throw new std::logic_error("Id isn\'t unique!");
			if(id > this->last_id)
				this->last_id = id;
			this->contents[id] = values;
		}
	}
	return true;
}


std::map<size_t, std::vector<std::wstring> > CSV::getData()
{
	return this->contents;
}


std::vector<std::wstring> CSV::getById(size_t id)
{
	if(this->isPresent(id))
		return this->contents[id];
	return std::vector<std::wstring>(0);
}


bool CSV::saveCSV()
{
	return this->exportToFile(this->filename);
}


bool CSV::isPresent(size_t id)
{
	return this->contents.find(id) != this->contents.end();
}


bool CSV::appendCSV(std::vector<std::wstring> append_data)
{
	this->last_id++;
	std::wstringstream convert;
	convert << this->last_id;
	std::wstring textId;
	convert >> textId;
	append_data.insert(append_data.begin(), textId);
	this->contents[this->last_id] = append_data;
	return true;
}


bool CSV::updateRow(size_t id, std::vector<std::wstring> newData)
{
	if(!this->isPresent(id))
		return false;
	std::wstringstream convert;
	convert << id;
	std::wstring textId;
	convert >> textId;
	newData.insert(newData.begin(), textId);
	this->contents[id] = newData;
	return true;
}


bool CSV::deleteRowById(size_t id)
{
	std::map<size_t, std::vector<std::wstring> >::iterator iterator;
	for(iterator = this->contents.begin(); iterator != this->contents.end(); iterator++)
	{
		if(iterator->first == id)
		{
			this->contents.erase(iterator);
			return true;
		}
	}
	return false;
}


bool CSV::exportToFile(std::wstring filename)
{
	std::wofstream in(filename.c_str());
	if(!in)
		throw new std::runtime_error("Error opening file!");
	std::map<size_t, std::vector<std::wstring> >::iterator iterator;
	for(iterator = this->contents.begin(); iterator != this->contents.end(); iterator++)
		in << this->separatedValueHelper->JoinValues(iterator->second) << '\n';
	in.close();
	return true;
}


void CSV::close()
{
	this->contents.clear();
	return;
}


extern "C" DLLIMPORT ICSVHandler* __cdecl CreateCSVHandle(const wchar_t* filename, const char delimeter)
{
	return (ICSVHandler*)(new CSV(filename, delimeter));
}
