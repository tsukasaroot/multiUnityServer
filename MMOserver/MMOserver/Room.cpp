#include "Server.h"

void Server::addRoom(std::vector<std::string> cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		auto searchForGuest = std::find(this->playerList.begin(), this->playerList.end(), cmd[1]);

		if (searchForGuest != this->playerList.end())
		{
			std::vector<std::string> host = { cmd[0] };
			size_t size = this->playerRoom.size() + 1;

			this->playerRoom.push_back(std::make_pair(size, host));
			this->_client[cmd[0]]->setRoom(this->playerRoom.size());

			std::vector<std::string> array = { "C_SENDROOM_INVITATION", this->_client[cmd[0]]->getNickName() };
			auto toSend = packetBuilder(array);
			this->_client[cmd[1]]->clientWrite(toSend);

			this->_client[cmd[0]]->setIfInviteSent(true);
		}
		else
		{
			std::vector<std::string> array = { "C_ACCEPT_INVITATION", cmd[1], "undefined" };
			auto toSend = packetBuilder(array);
			this->_client[cmd[0]]->clientWrite(toSend);
		}
	}
}

void Server::joinRoom(std::vector<std::string> cmd)
{
	if (checkAll(5, cmd, &this->playerList))
	{
		auto searchForHost = std::find(this->playerList.begin(), this->playerList.end(), cmd[0]);

		if (searchForHost != this->playerList.end())
		{
			if (cmd[2] == "true" && this->_client[cmd[1]]->getIfInviteSent())
			{
				std::vector<std::string> array = { "C_ACCEPT_INVITATION", this->_client[cmd[1]]->getNickName(), cmd[2] };
				auto toSend = packetBuilder(array);
				this->_client[cmd[0]]->clientWrite(toSend);

				auto room = this->_client[cmd[1]]->getRoom();
				this->_client[cmd[0]]->setRoom(room);

				this->_client[cmd[0]]->setIfInviteSent(false);
			}
			else if (cmd[2] == "false")
			{
				std::vector<std::string> array = { "C_ACCEPT_INVITATION", this->_client[cmd[1]]->getNickName(), cmd[2] };
				auto toSend = packetBuilder(array);
				this->_client[cmd[0]]->clientWrite(toSend);

				this->_client[cmd[1]]->setRoom(0);

				this->_client[cmd[1]]->setIfInviteSent(false);
			}
		}
		else
		{
			std::vector<std::string> array = { "C_ACCEPT_INVITATION", cmd[1], "undefined" };
			auto toSend = packetBuilder(array);
			this->_client[cmd[0]]->clientWrite(toSend);
		}
	}
}