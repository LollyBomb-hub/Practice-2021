#include "sv.h"


SeparatedValues::SeparatedValues(const char delimeter)
{
	this->delimeter = std::string(1, delimeter);
	this->wdelimeter = std::wstring(this->delimeter.begin(), this->delimeter.end());
	this->regexpr = std::regex("([^" + this->delimeter + "\n]+)");
	this->wregexpr = std::wregex(L"([^" + this->wdelimeter + L"\n]+)");
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
	std::vector<std::string> result;
	std::smatch sm;
	while(std::regex_search(value, sm, this->regexpr))
	{
		result.push_back(sm[1].str());
		value = sm.suffix();
	}
	return result;
}


std::wstring SeparatedValues::JoinValues(std::vector<std::wstring> values)
{
	if(!values.size())
		return std::wstring(L"");
	std::wstring result(L"");
	for(size_t i = 0; i < values.size(); i++)
		result += values[i] + this->wdelimeter;
	return result;
}


std::vector<std::wstring> SeparatedValues::GetValues(std::wstring value)
{
	if(value.size() == 0 || value.size() < this->delimeter.size() || this->delimeter.size() == 0)
		return std::vector<std::wstring>(0);
	std::vector<std::wstring> result;
	std::wsmatch wsm;
	while(std::regex_search(value, wsm, this->wregexpr))
	{
		result.push_back(wsm[1].str());
		value = wsm.suffix();
	}
	return result;
}


extern "C" DLLIMPORT ISVHandler* __cdecl CreateSVHandle(const char delimeter)
{
	return (ISVHandler*)(new SeparatedValues(delimeter));
}
