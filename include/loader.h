#ifndef LOADER_H

#define LOADER_H

#include <windows.h>

#include "../CSV/csv.h"
#include "../libGUI/libGUI.h"

#define IFactory(value)	IFactory##value
#define Factory(value)	Factory##value

#define loadDLLFunction(var_name, library, library_path)	HINSTANCE library = LoadLibraryA(library_path);\
																				if(GetLastError())\
																				{\
																					std::cout << "Loading " << #library << " failed with err_code: " << GetLastError() << '\n';\
																					exit(0);\
																				}\
																				var_name = (Factory(library))(void*)(GetProcAddress(library, IFactory(library)));\
																				if(GetLastError())\
																				{\
																					std::cout << "Loading " << #var_name;\
																					std::cout << " by its name ( ";\
																					std::cout << IFactory(library) << " )";\
																					std::cout << " failed with err_code: " << GetLastError();\
																					exit(0);\
																				}\

#define freeLibraries(libraries)	for(size_t index_of_library = 0; index_of_library < libraries.size(); index_of_library++)\
										FreeLibrary(libraries[index_of_library]);

#define checkLibraries(libraries)	for(size_t index_of_library = 0; index_of_library < libraries.size(); index_of_library++)\
									if(!libraries[index_of_library])\
									{\
										std::cout << "Not found library at index " << index_of_library << '\n';\
										freeLibraries(libraries)\
										exit(0);\
									}	

#define assertDLLFunction(function_var_name, libraries)	if(!function_var_name)\
														{\
															freeLibraries(libraries)\
															exit(0);\
														}

#endif
