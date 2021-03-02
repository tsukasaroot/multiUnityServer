#include "inventoryManager.h"

inventoryManager::inventoryManager(SQLManager *db)
{
	this->dataBase = db;
}

void inventoryManager::addItem(std::vector<int> item)
{

}

std::vector<int> inventoryManager::getClientInventory(std::string client)
{
	std::vector<std::string> fields;
	std::vector<std::string> columnName;
	std::vector<std::string> where;

	fields.push_back("item_id");
	fields.push_back("player_id");
	columnName.push_back("item_id");
	columnName.push_back("player_id");
	where.push_back("1");
	where.push_back("1");

	this->dataBase->get("inventory", fields, columnName, where);
	std::cout << "inventoryManager initialized: " << this->items.size() << " inventories loaded" << std::endl;
}

void inventoryManager::saveClientInventory(std::string cmd)
{

}