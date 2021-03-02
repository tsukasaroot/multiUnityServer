#include "server.h"

void Server::privateMessage(std::vector<std::string> cmd)
{
	if (checkAll(5, cmd, &this->playerList))
	{
		std::string nickName = cmd[0];
		Client* sender = this->_client[cmd[0]];
		Client* receiver = this->_client[cmd[1]];

		this->messages->sendPrivateMessage(sender, receiver, cmd[2]);
	}
}

void Server::globalMessage(std::vector<std::string> cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		std::string nickName = cmd[0];
		Client* sender = this->_client[cmd[0]];

		this->messages->sendGlobalMessage(sender, cmd[1], this->_client);
	}
}