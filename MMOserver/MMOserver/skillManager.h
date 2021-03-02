#ifndef SKILLMANAGER_H_
#define SKILLMANAGER_H_

#include "xmlParser.h"


#include <windows.h>

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <time.h>

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader);

class skillManager
{
public:
	skillManager();
	~skillManager() {};
private:
	xmlParser* reader;
	std::map<std::string, std::vector<std::string>> skills;
};

#endif