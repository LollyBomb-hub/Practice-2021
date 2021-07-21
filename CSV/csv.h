#ifndef _CSV_DLL_H_
#define _CSV_DLL_H_

#ifndef DLLIMPORT
	#if BUILDING_DLL
		#define DLLIMPORT __declspec(dllexport)
	#else
		#define DLLIMPORT __declspec(dllimport)
	#endif
#endif


#include "SV/sv.h"

#include <string>
#include <vector>
#include <map>


class ICSVHandler
{
	public:
		virtual bool readCSV() = 0;
		virtual std::map<size_t, std::vector<std::wstring> > getData() = 0;
		virtual std::vector<std::wstring> getById(size_t) = 0;
		virtual bool saveCSV() = 0;
		virtual bool isPresent(size_t) = 0;
		virtual bool appendCSV(std::vector<std::wstring>) = 0;
		virtual bool updateRow(size_t, std::vector<std::wstring>) = 0;
		virtual bool deleteRowById(size_t) = 0;
		virtual bool exportToFile(std::wstring) = 0;
		virtual void close() = 0;
};

class CSV: public ICSVHandler
{
	public:
		CSV(const wchar_t*, const char*);
		bool readCSV();
		std::map<size_t, std::vector<std::wstring> > getData();
		std::vector<std::wstring> getById(size_t);
		bool saveCSV();
		bool isPresent(size_t);
		bool appendCSV(std::vector<std::wstring>);
		bool updateRow(size_t, std::vector<std::wstring>);
		bool deleteRowById(size_t);
		bool exportToFile(std::wstring);
		void close();
		
	private:
		size_t last_id;
		std::wstring filename;
		std::map<size_t, std::vector<std::wstring> > contents;
		ISVHandler* separatedValueHelper;
};

typedef ICSVHandler* (__cdecl *FactoryFunctionCSV)(const wchar_t*, const char*);

#define IFactoryCSV "CreateCSVHandle"
#define FactoryCSV FactoryFunctionCSV

extern "C" DLLIMPORT ICSVHandler* __cdecl CreateCSVHandle(const wchar_t* filename, const char* delimeter);

#endif
