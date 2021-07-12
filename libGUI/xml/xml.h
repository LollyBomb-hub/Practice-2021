#ifndef _XML_DLL_H_
#define _XML_DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif


#include "dom.h"


#define IFactoryXML "CreateXMLHandle"
#define FactoryXML	FactoryFunctionXML
typedef IXMLHandler* (__cdecl *FactoryFunctionXML)(const char*);


#endif
