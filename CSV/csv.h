#ifndef _CSV_DLL_H_
#define _CSV_DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

#include <string>

class ICSVHandler
{
	public:
		virtual std::string read_csv() = 0;
		virtual bool write_csv(std::string) = 0;
		virtual bool append_csv(std::string) = 0;
		virtual bool write_csv(const char*) = 0;
		virtual bool append_csv(const char*) = 0;
};

class CSV: public ICSVHandler
{
	public:
		CSV(const char*);
		CSV(std::string);
		std::string read_csv();
		bool write_csv(std::string);
		bool write_csv(const char*);
		bool append_csv(std::string);
		bool append_csv(const char*);
		
	private:
		std::string filename;
};

#define IFactoryCSV "CreateCSVHandle"
#define FactoryCSV	FactoryFunctionCSV
typedef ICSVHandler* (__cdecl *FactoryFunctionCSV)(const char*);

#endif
