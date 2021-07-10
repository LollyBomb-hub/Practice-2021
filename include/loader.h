#ifndef LOADER_H

#define LOADER_H

#include <windows.h>

#include "../CSV/csv.h"
#include "../SV/sv.h"

#define IFactory(value)	IFactory##value
#define Factory(value)	Factory##value

#define loadDLLFunction(type_cast, var_name, library_var_name, library_path, proc_name, libraries_container)	HINSTANCE library_var_name = LoadLibrary(library_path);\
																												libraries_container.push_back(library_var_name);\
																												type_cast var_name = reinterpret_cast<type_cast>(GetProcAddress(library_var_name, proc_name));

#define freeLibraries(libraries)	for(size_t index_of_library = 0; index_of_library < libraries.size(); index_of_library++)\
										FreeLibrary(libraries[index_of_library]);

#define checkLibraries(libraries)	for(size_t index_of_library = 0; index_of_library < libraries.size(); index_of_library++)\
									if(!libraries[index_of_library])\
									{\
										freeLibraries(libraries)\
										exit(0);\
									}	

#define assertDLLFunction(function_var_name, libraries)	if(!function_var_name)\
														{\
															freeLibraries(libraries)\
															exit(0);\
														}

#endif
