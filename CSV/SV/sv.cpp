#include "sv.h"

SeparatedValues::SeparatedValues(const char* delimeter)
{
	this->delimeter = std::string(delimeter);
	return;
}


std::string SeparatedValues::JoinValues(std::vector<std::string> values)
{
	if(!values.size())
		return std::string("");
	std::string result("");
	for(size_t i = 0; i < values.size(); i++)
		result += values[i] + this->delimeter;
	return result;
}


std::vector<std::string> SeparatedValues::GetValues(std::string value)
{
	if(value.size() == 0 || value.size() < this->delimeter.size() || this->delimeter.size() == 0)
		return std::vector<std::string>(0);
	const size_t length_of_delimeter = this->delimeter.size();
	size_t position = value.find(this->delimeter);
	while(position != std::string::npos)
	{
		value.replace(position, length_of_delimeter, " ");
		position = value.find(this->delimeter, position);
	}
	std::vector<std::string> result;
	std::istringstream from(value);
	while(!from.eof())
	{
		std::string buffer;
		from >> buffer;
		if(buffer.size())
			result.push_back(buffer);
	}
	return result;
}


std::wstring SeparatedValues::JoinValues(std::vector<std::wstring> values)
{
	if(!values.size())
		return std::wstring(L"");
	std::wstring delim = std::wstring(this->delimeter.begin(), this->delimeter.end());
	std::wstring result(L"");
	for(size_t i = 0; i < values.size(); i++)
		result += values[i] + delim;
	return result;
}


std::vector<std::wstring> SeparatedValues::GetValues(std::wstring value)
{
	if(value.size() == 0 || value.size() < this->delimeter.size() || this->delimeter.size() == 0)
		return std::vector<std::wstring>(0);
	std::wstring delim = std::wstring(this->delimeter.begin(), this->delimeter.end());
	const size_t length_of_delimeter = delim.size();
	size_t position = value.find(delim);
	while(position != std::wstring::npos)
	{
		value.replace(position, length_of_delimeter, L" ");
		position = value.find(delim, position);
	}
	std::vector<std::wstring> result;
	std::wistringstream from(value);
	while(!from.eof())
	{
		std::wstring buffer;
		from >> buffer;
		if(buffer.size())
			result.push_back(buffer);
	}
	return result;
}


extern "C" DLLIMPORT ISVHandler* __cdecl CreateSVHandle(const char* delimeter)
{
	return (ISVHandler*)(new SeparatedValues(delimeter));
}
