#include "csv.h"


#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>



CSV::CSV(const char* filename, const char* delimeter)
{
	this->filename = std::string(filename);
	this->separatedValueHelper = CreateSVHandle(delimeter);
	this->readCSV();
	return;
}


bool CSV::readCSV()
{
	this->last_id = 1;
	size_t id;
	std::string line;
	std::ifstream in(this->filename.c_str());
	if(!in)
		throw new std::runtime_error("No such file!");
	while(std::getline(in, line))
	{
		std::vector<std::string> values = this->separatedValueHelper->GetValues(line);
		std::istringstream convert(values[0]);
		convert >> id;
		if(this->isPresent(id))
			throw new std::logic_error("Id isn\'t unique!");
		if(id > this->last_id)
			this->last_id = id;
		this->contents[id] = values;
	}
	return true;
}


std::map<size_t, std::vector<std::string> > CSV::getData()
{
	return this->contents;
}


std::vector<std::string> CSV::getById(size_t id)
{
	return this->contents[id];
}


bool CSV::saveCSV()
{
	return this->exportToFile(this->filename);
}


bool CSV::isPresent(size_t id)
{
	return this->contents[id].size() > 0;
}


bool CSV::appendCSV(std::vector<std::string> append_data)
{
	this->last_id++;
	std::stringstream convert;
	convert << this->last_id;
	std::string textId;
	convert >> textId;
	append_data.insert(append_data.begin(), textId);
	this->contents[this->last_id] = append_data;
	return true;
}


bool CSV::updateRow(size_t id, std::vector<std::string> newData)
{
	if(!this->contents[id].size())
		return false;
	this->contents[id] = newData;
	return true;
}


bool CSV::deleteRowById(size_t id)
{
	std::map<size_t, std::vector<std::string> >::iterator iterator;
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


bool CSV::exportToFile(std::string filename)
{
	std::ofstream in(filename.c_str());
	if(!in)
		throw new std::runtime_error("Error opening file!");
	std::map<size_t, std::vector<std::string> >::iterator iterator;
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


extern "C" DLLIMPORT ICSVHandler* __cdecl CreateCSVHandle(const char* filename, const char* delimeter)
{
	return (ICSVHandler*)(new CSV(filename, delimeter));
}
