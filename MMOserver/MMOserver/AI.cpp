#include "Server.h"

void Server::createAI()
{
	auto reader = new xmlParser("items.xml");

	auto npcs = stockXML(reader);
	std::cout << "AI initialized: " << npcs.size() << " npc(s) loaded" << std::endl;
}