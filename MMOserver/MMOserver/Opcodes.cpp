#include "Server.h"

void Server::Opcodesinitialize()
{
	this->reader = new xmlParser("opcodes.xml");
	auto functions = this->reader->cleanData(reader->Data["<functions>"]);
	std::string name = "&Server::login";

	list.insert(std::pair<std::string, opcodes>("S_LOGIN", &Server::login));
	list.insert(std::pair<std::string, opcodes>("S_LOGOUT", &Server::logout));
	list.insert(std::pair<std::string, opcodes>("S_SAVEDATA", &Server::savePlayerData));
	list.insert(std::pair<std::string, opcodes>("S_SENDCLIENTDATA", &Server::sendClientData));
	list.insert(std::pair<std::string, opcodes>("S_PRIVATEMESSAGE", &Server::privateMessage));
	list.insert(std::pair<std::string, opcodes>("S_GLOBALMESSAGE", &Server::globalMessage));
	list.insert(std::pair<std::string, opcodes>("S_SENDALLCLIENTSNAME", &Server::sendAllClientsName));
	list.insert(std::pair<std::string, opcodes>("S_SENDPROFILE", &Server::sendProfile));
	list.insert(std::pair<std::string, opcodes>("S_GETCURRENTPOSITION", &Server::getPosition));

	std::cout << "OPCodes initialized: " << list.size() << " OPCodes loaded" << std::endl;

	this->items = new itemsManager();
	this->npcSpawn = new npcSpawner();
	this->messages = new messageManager();
	this->skills = new skillManager();
}

void Server::processOpcodes(std::vector<std::string> opcodes, std::string ip)
{
	for (auto& cmd : opcodes)
	{
		std::string token = cmd.substr(0, cmd.find(':'));
		cmd.erase(0, token.length() + 1);
		std::vector<std::string> args = formatString(cmd);

		if (this->list.find(token) != this->list.end())
		{
			(this->*list[token])(args);
		}
		else
		{
			std::cerr << "Invalid opcode sent from: " << ip << std::endl;
		}
		args.clear();
	}
}