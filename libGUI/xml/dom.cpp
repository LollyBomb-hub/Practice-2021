#include "dom.h"


// DOMTree methods definitions


DOMTree::DOMTree()
{
	this->type = std::string("ROOT");
	this->this_level_elements = std::vector<IXMLHandler*>(0);
	this->setProperty("name", "ROOT");
	return;
}


DOMTree::DOMTree(std::string type)
{
	this->type = type;
	this->this_level_elements = std::vector<IXMLHandler*>(0);
	return;
}


bool DOMTree::addElement(IXMLHandler* element)
{
	this->this_level_elements.push_back(element);
	return true;
}


std::string DOMTree::getType()
{
	return this->type;
}


std::string DOMTree::getName()
{
	return this->getProperty("name");
}


std::string DOMTree::getText()
{
	return this->getProperty("text");
}


std::vector<IXMLHandler*> DOMTree::getCurrentLevel()
{
	return this->this_level_elements;
}


IXMLHandler* DOMTree::getLevelByPath(IXMLHandler* tree, std::vector<std::string> path)
{
	if(tree->getName() == path[0])
	{
		path.erase(path.begin());
		if(path.size() == 0)
		{
			return tree;
		}
		else if(path.size() > 0)
		{
			return tree->getLevelByPath(tree, path);
		}
	}
	if(path.size() == 0)
		return tree;
	for(IXMLHandler* element: tree->getCurrentLevel())
	{
		if(element->getName() == path[0])
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


IXMLHandler* DOMTree::operator[](const char* level_name){
	std::string check(level_name);
	for(IXMLHandler* el: this->this_level_elements)
	{
		if(el->getName() == check)
			return el;
	}
	return new DOMTree("Error");
}
