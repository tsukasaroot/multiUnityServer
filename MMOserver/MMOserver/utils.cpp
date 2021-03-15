#include "Server.h"

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader)
{
	std::map<std::string, std::vector<std::string>> object;

	for (auto it = reader->Data.begin(); it != reader->Data.end(); it++)
	{
		auto parent = reader->cleanParent(it->first);
		auto functions = reader->cleanData(it->second);

		object.insert(std::pair<std::string, std::vector<std::string>>(parent, functions));
	}
	return object;
}

bool checkAll(int size, std::vector<std::string> cmd, std::vector<std::string>* playerList)
{
	if (cmd.size() == size)
	{
		std::string nickName = cmd[0];
		auto it = std::find(playerList->begin(), playerList->end(), nickName);
		if (it != playerList->end())
			return true;
		else
			std::cerr << "Error: " << nickName << " this player don't exist, command denied." << std::endl;
	}
	return false;
}

std::string packetBuilder(Packet requests)
{
	std::string result;

	for (auto it = requests.begin(); it != requests.end(); it++)
	{
		result += *it;
		if (it + 1 != requests.end())
			result += ':';
	}
	return result;
}