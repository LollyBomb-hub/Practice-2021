#ifndef _CONSTANTS_H_

#define _CONSTANTS_H_

#include <regex>

typedef enum
{
	DATA,
	ELEMENT_DEFINITION,
	CLOSING_ELEMENT
} TYPES;

static const std::regex regexpr_type("<([/]?)([a-z]*)");
static const std::regex regexpr_properties("([a-zA-Z]*)=[\"\']?([a-zA-Zà-ÿÀ-ß0-9\\s]*)[\"\']?");

#endif
