#include "xml.h"
#include "constants.h"


#include <clocale>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>


TYPES parseLine(std::string line, DOMTree** dom)
{
	std::smatch sm;
	std::regex_search(line, sm, regexpr_type);
	std::string type = sm[2].str();
	if(sm[1].str().size() == 1)
	{
		*dom = new DOMTree(type);
		return CLOSING_ELEMENT;
	}
	if(type.size() == 0)
		return DATA;
	*dom = new DOMTree(type);
	while(std::regex_search(line, sm, regexpr_properties))
	{
		dom[0]->setProperty(sm[1].str(), sm[2].str());
		line = sm.suffix();
	}
	return ELEMENT_DEFINITION;
}


void printTree(IXMLHandler* tree, size_t count)
{
	std::cout << std::string(count, '\t') << tree->getName() << '\n';
	for(IXMLHandler* element: tree->getCurrentLevel())
		printTree(element, count + 1);
	return;
}


DOMTree* parseXML(const char* filename)
{
	std::stack<std::string> opened;
	DOMTree* xml = new DOMTree();
	std::vector<std::string> path;
	path.push_back(xml->getName());
	setlocale(LC_ALL, "rus");
	std::ifstream in(filename);
	size_t i = 1;
	while(!in.eof())
	{
		TYPES flag;
		DOMTree* dom = new DOMTree();
		std::string buffer;
		getline(in, buffer);
		flag = parseLine(buffer, &dom);
		if(flag == ELEMENT_DEFINITION)
		{
			xml->getLevelByPath(xml, path)->addElement(dom);
			path.push_back(dom->getName());
			opened.push(dom->getType());
		}
		else if(flag == CLOSING_ELEMENT)
		{
			if(opened.top() != dom->getType())
			{
				std::ostringstream err_message;
				err_message << opened.top() + "(Opened) != " + dom->getType() + "(Closing statement) at line: ";
				err_message << i;
				throw std::runtime_error(err_message.str());
			}
			if(path.size() == 0)
				throw std::runtime_error("Different count of closed and opened elements!");
			opened.pop();
			path.pop_back();
		}
		i++;
	}
	if(path.size() != 1)
		throw new std::logic_error("XML file corrupted!");
	return xml;
}


extern "C" __declspec(dllexport) IXMLHandler* __cdecl CreateXMLHandle(const char* filename)
{
	return (IXMLHandler*)(parseXML(filename));
}
