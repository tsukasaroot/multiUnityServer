#ifndef ITEMS_H_
#define ITEMS_H_

#include "xmlParser.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader);

class itemsManager {
public:
	itemsManager();
private:
	xmlParser* reader;
	std::map<std::string, std::vector<std::string>> items;
};

#endif