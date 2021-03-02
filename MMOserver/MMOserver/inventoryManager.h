#ifndef INVENTORYMANAGER_H_
#define INVENTORYMANAGER_H_

#include "xmlParser.h"
#include "SQLmanager.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader);

class inventoryManager {
public:
	inventoryManager(SQLManager* db);
	void addItem(std::vector<int> item);
	void saveClientInventory(std::string cmd);
	std::vector<int> getClientInventory(std::string client);
private:
	SQLManager* dataBase;
	xmlParser *reader;
	std::map<std::string, std::vector<std::string>> items;
};

#endif