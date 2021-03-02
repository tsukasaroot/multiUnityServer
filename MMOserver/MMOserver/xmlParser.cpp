#include "xmlParser.h"

std::map<std::string, std::string> checkXML(std::vector<std::string> xmlStruct)
{
	std::vector<std::string> parent;
	std::map<std::string, std::string> parents;
	std::map<std::string, std::string> XMLobject;

	for (auto it = xmlStruct.begin(); it != xmlStruct.end(); it++)
	{
		if ((*it)[0] == '<' && (*it)[it->size() - 1] == '>' && (*it)[it->size() - 2] == '/') // Check orphan tag
		{
			if (XMLobject.find(parent[parent.size() - 1]) == XMLobject.end())
				XMLobject.insert(std::pair<std::string, std::string>(parent[parent.size() - 1], *it));
			else
				XMLobject[parent[parent.size() - 1]].append(*it);
		}
		else if ((*it)[0] == '<' && (*it)[it->size() - 1] == '>' && (*it)[1] != '/') // Check a parent tag
		{
			if (parents.find(*it) != parents.end())
			{
				std::cerr << "Parent tag already opened " << *it << std::endl;
				exit(1);
			}
			parent.push_back(*it);
			parents.insert(std::pair<std::string, std::string>(*it, *it));
		}
		else if ((*it)[0] == '<' && (*it)[1] == '/' && (*it)[it->size() - 1] == '>') // Check a closing parent tag
		{
			it->erase(std::remove(it->begin(), it->end(), '/'), it->end());
			if (parents.size() > 0)
			{
				if (parents.find(*it) == parents.end())
				{
					std::cerr << "Parent tags seems inaccurate " << parents[*it] << " " << *it << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cerr << "No parents for this closing tag " << *it << std::endl;
				exit(1);
			}
			parent.erase(parent.end() - 1);
		}
		else
		{
			std::cerr << "XML tags not properly closed at " << *it << std::endl;
			exit(1);
		}
	}

	return XMLobject;
}

std::vector<std::string> xmlParser::cleanData(std::string data)
{
	std::vector<std::string> functions;
	std::string token;
	size_t pos = 0;

	data.erase(std::remove_if(data.begin(), data.end(), OneOf("/<")), data.end());
	std::replace(data.begin(), data.end(), '>', ' ');

	while ((pos = (size_t)data.find(' ')) != std::string::npos)
	{
		std::string token = data.substr(0, data.find(' '));

		if (!token.empty())
			functions.push_back(token);
		data.erase(0, pos + 1);
		token.clear();
	}
	return functions;
}

std::string xmlParser::cleanParent(std::string data)
{
	data.erase(std::remove_if(data.begin(), data.end(), OneOf("/<>")), data.end());
//	std::replace(data.begin(), data.end(), '>', ' ');

	return data;
}

xmlParser::xmlParser(const char *const path)
{
	std::fstream fs;
	std::string buffer;
	fs.open(path, std::fstream::in);
	std::vector<std::string> xmlStruct;

	if (!fs)
	{
		std::cerr << "File " << path << " not found" << std::endl;
		exit(1);
	}

	while (std::getline(fs, buffer))
	{
		buffer.erase(std::remove(buffer.begin(), buffer.end(), '\t'), buffer.end());
		xmlStruct.push_back(buffer);
		buffer.clear();
	}
	fs.close();
	Data = checkXML(xmlStruct);
}