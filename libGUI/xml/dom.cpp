#include "dom.h"


// DOMTree methods definitions


DOMTree::DOMTree()
{
	this->level = std::string("ROOT");
	this->this_level_elements = std::vector<IXMLHandler*>(0);
	this->setProperty("name", "ROOT");
	return;
}


DOMTree::DOMTree(std::string level)
{
	this->level = level;
	this->this_level_elements = std::vector<IXMLHandler*>(0);
	return;
}


bool DOMTree::addElement(IXMLHandler* element)
{
	this->this_level_elements.push_back(element);
	return true;
}


std::string DOMTree::getName()
{
	return this->level;
}


std::vector<IXMLHandler*> DOMTree::getCurrentLevel()
{
	return this->this_level_elements;
}


IXMLHandler* DOMTree::getLevelByPath(IXMLHandler* tree, std::vector<std::string> path)
{
	//std::cout << tree->getName() << ' ' << tree->getProperty("name") << '\n';
	if(tree->getProperty("name") == path[0])
	{
		path.erase(path.begin());
		if(path.size() == 0)
		{
			return tree;
		}
		else if(path.size() > 0)
		{
			return DOMTree::getLevelByPath(tree, path);
		}
	}
	if(path.size() == 0)
		return tree;
	for(IXMLHandler* element: tree->getCurrentLevel())
	{
		if(element->getProperty("name") == path[0])
		{
			path.erase(path.begin());
			if(path.size() == 0)
			{
				return element;
			}
			else if(path.size() > 0)
			{
				return DOMTree::getLevelByPath(element, path);
			}
		}
	}
	return NULL; // Not found
}


void DOMTree::setProperty(std::string key, std::string value)
{
	this->properties[key] = value;
	return;
}


std::string DOMTree::getProperty(std::string key)
{
	return this->properties[key];
}


void DOMTree::print()
{
	std::cout << "Deleting object " << this->getName() << '\n';
	std::cout << "With properties:\n";
	for(auto& element: this->properties)
		std::cout << element.first << ' ' << element.second << '\n';
}


IXMLHandler* DOMTree::operator[](const char* level_name){
	std::string check(level_name);
	for(IXMLHandler* el: this->this_level_elements)
	{
		if(el->getProperty("name") == check)
			return el;
	}
	return new DOMTree("Error");
}
