#include <iostream>
#include <vector>
#include <windows.h>

#include "include/loader.h"

int main(void)
{
	std::vector<HINSTANCE> libraries;
	loadDLLFunction(Factory(CSV), CreateCSV, CSVLibraryHandle, "./CSV/CSV.dll", IFactory(CSV), libraries)
	loadDLLFunction(Factory(SV), CreateSV, SVLibraryHandle, "./SV/sv.dll", IFactory(SV), libraries)
	checkLibraries(libraries);
	ICSVHandler* csv;
	ISVHandler* sv;
	csv = CreateCSV("test.csv");
	sv = CreateSV(";");
	std::vector<std::string> test_values;
	test_values.push_back("Works");
	test_values.push_back("with");
	test_values.push_back("dll!");
	csv->write_csv(sv->JoinValues(test_values));
	freeLibraries(libraries);
	return 0;
}
