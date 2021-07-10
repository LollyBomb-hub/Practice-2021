#include "csv.h"


#include <iostream>
#include <fstream>


CSV::CSV(std::string filename)
{
	this->filename = std::string(filename.c_str());
	return;
}


CSV::CSV(const char* filename)
{
	this->filename = std::string(filename);
	return;
}


std::string CSV::read_csv()
{
	std::ifstream in(this->filename.c_str());
	in.seekg(0, std::ios::end);
	size_t size = in.tellg();
	std::string contents(size, ' ');
	in.seekg(0);
	in.read(&contents[0], size);
	in.close();
	return std::string(contents.c_str());
}


bool CSV::write_csv(const char* data)
{
	std::ofstream out(this->filename.c_str());
	if(!out.is_open())
		return false; // Smth went wrong
	out << data;
	out.close();
	return true; // Success
}


bool CSV::write_csv(std::string data)
{
	return this->write_csv(data.c_str());
}


bool CSV::append_csv(const char* append_data)
{
	std::ofstream out(this->filename.c_str(), std::ios_base::app);
	if(!out.is_open())
		return false;
	out << append_data;
	out.close();
	return true;
}


bool CSV::append_csv(std::string append_data)
{
	return this->append_csv(append_data.c_str());
}


void DLLIMPORT DestroyCSVHandle(CSV* handle)
{
	(void)handle;
	return;
}


extern "C" __declspec(dllexport) ICSVHandler* __cdecl CreateCSVHandle(const char* filename)
{
	return (ICSVHandler*)(new CSV(filename));
}
