#ifndef NPCSPAWNER_H_
#define NPCSPAWNER_H_

#include "xmlParser.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader);

class npcSpawner {
public:
	npcSpawner();
private:
	xmlParser *reader;
	std::map<std::string, std::vector<std::string>> npcs;
};

#endif