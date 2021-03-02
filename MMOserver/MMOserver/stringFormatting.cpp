#include "SQLmanager.h"

std::vector<std::string> formatString(std::string line)
{
	std::vector<std::string> buildedCommands;
	std::string token;

	while (line.find(':') != std::string::npos)
	{
		token = line.substr(0, line.find(':'));
		line.erase(0, token.length() + 1);
		buildedCommands.push_back(token);
		token.clear();
	}
	buildedCommands.push_back(line);
	return buildedCommands;
}