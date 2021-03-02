#include "npcSpawner.h"

npcSpawner::npcSpawner()
{
	this->reader = new xmlParser("npcs.xml");

	npcs = stockXML(this->reader);
	std::cout << "npcSpawner initialized: " << this->npcs.size() << " npc(s) loaded" << std::endl;
}