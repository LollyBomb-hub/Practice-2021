#ifndef _DOM_H_

#define _DOM_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>


class IXMLHandler
{
	public:
		virtual bool addElement(IXMLHandler*);
		virtual std::string getType() = 0;
		virtual std::string getName() = 0;
		virtual std::string getText() = 0;
		virtual std::vector<IXMLHandler*> getCurrentLevel() = 0;
		virtual std::string getProperty(std::string) = 0;
		virtual IXMLHandler* getLevelByPath(IXMLHandler*, std::vector<std::string>) = 0;
		virtual IXMLHandler* operator[](const char*) = 0;
};


class DOMTree: public IXMLHandler
{
	public:
		DOMTree();
		DOMTree(std::string);
		bool addElement(IXMLHandler*);
		std::string getType();
		std::string getName();
		std::string getText();
		void setProperty(std::string, std::string);
		std::string getProperty(std::string);
		std::vector<IXMLHandler*> getCurrentLevel();
		IXMLHandler* getLevelByPath(IXMLHandler*, std::vector<std::string>);
		IXMLHandler* operator[](const char* level_name);
	private:
		std::string type;
		std::vector<IXMLHandler*> this_level_elements;
		std::map<std::string, std::string> properties;
};


#endif
