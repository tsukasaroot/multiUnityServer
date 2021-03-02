#include "server.h"

void Server::sendAllClientsName(std::vector<std::string> cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		std::string currentClient = cmd[0];
		std::string listName = "C_GETALLCLIENTSNAME";

		for (auto it = this->_client.begin(); it != this->_client.end(); it++)
		{
			listName = listName + ':' + it->second->getNickName();
		}
		this->_client[currentClient]->clientWrite(listName);
	}
}

void Server::sendClientData(std::vector<std::string> cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		std::string nickName = cmd[0];

		auto dataToSend = "C_GETCLIENTDATA:" + this->_client[nickName]->getAll();
		this->_client[nickName]->clientWrite(dataToSend);
	}
}

void Server::sendProfile(std::vector<std::string> cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		std::string nickName = cmd[0];

		auto dataToSend = "C_GETPROFILE:" + this->_client[nickName]->getNickName() + ':' + this->_client[nickName]->getAll();
		this->_client[nickName]->clientWrite(dataToSend);
	}
}

void Server::getPosition(std::vector<std::string> cmd)
{
	if (checkAll(6, cmd, &this->playerList))
	{
		std::string nickName = cmd[0];

		this->_client[nickName]->setPositionQuery(cmd);
		if (this->_client[nickName]->getAbnormalities() > this->abnormalitiesTolerance)
		{
			std::vector<std::string> data;
			data.push_back(nickName);
			data.push_back(cmd[cmd.size() - 1]);
			logout(data);
		}
	}
}