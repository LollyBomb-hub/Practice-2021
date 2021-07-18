#ifndef _SV_DLL_H_
#define _SV_DLL_H_

#ifndef DLLIMPORT
	#if BUILDING_DLL
		#define DLLIMPORT __declspec(dllexport)
	#else
		#define DLLIMPORT __declspec(dllimport)
	#endif
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ISVHandler
{
	public:
		virtual std::string JoinValues(std::vector<std::string>) = 0;
		virtual std::vector<std::string> GetValues(std::string) = 0;
};

class SeparatedValues: public ISVHandler
{
	public:
		SeparatedValues(const char*);
		std::string JoinValues(std::vector<std::string>);
		std::vector<std::string> GetValues(std::string);
	private:
		std::string delimeter;
};


#define IFactorySV	"CreateSVHandle"
#define FactorySV	FactoryFunctionSV
typedef ISVHandler* (__cdecl *FactoryFunctionSV)(const char*);

extern "C" DLLIMPORT ISVHandler* __cdecl CreateSVHandle(const char* delimeter);

#endif
