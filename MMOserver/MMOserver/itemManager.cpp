#include "itemManager.h"

itemsManager::itemsManager()
{
	this->reader = new xmlParser("items.xml");

	this->items = stockXML(this->reader);
	std::cout << "itemsManager initialized: " << this->items.size() << " item(s) loaded" << std::endl;
}