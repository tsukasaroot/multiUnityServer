#include "Server.h"

void Server::addRoom(std::vector<std::string> cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		auto searchForGuest = std::find(this->playerList.begin(), this->playerList.end(), cmd[1]);

		if (this->_client[cmd[0]]->getIfInviteSent() == true)
		{
			return;
		}

		if (this->_client[cmd[0]]->getRoom() > 0 && this->_client[cmd[1]]->getRoom() > 0)
		{
			return;
		}

		if (searchForGuest != this->playerList.end() && this->_client[cmd[0]]->getNickName() != this->_client[cmd[1]]->getNickName())
		{
			std::vector<std::string> host = { cmd[0] };
			size_t size = this->playerRoom.size() + 1;

			this->playerRoom.push_back(std::make_pair(size, host));
			this->_client[cmd[0]]->setRoom(this->playerRoom.size());

			std::vector<std::string> array = { "C_SENDROOM_INVITATION", this->_client[cmd[0]]->getNickName(), std::to_string(this->_client[cmd[0]]->getRoom()) };
			auto toSend = packetBuilder(array);
			this->_client[cmd[1]]->clientWrite(toSend);

			this->_client[cmd[0]]->setIfInviteSent(true);

			std::cout << "invite sent by " << cmd[0] << " to " << cmd[1] << std::endl;

		}
		else
		{
			std::vector<std::string> array = { "C_ACCEPT_INVITATION", cmd[1], "undefined" };
			auto toSend = packetBuilder(array);
			this->_client[cmd[0]]->clientWrite(toSend);
			std::cout << "Player doesn't exist..." << std::endl;
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
			if (cmd[2] == "true" && this->_client[cmd[0]]->getIfInviteSent() == true)
			{
				std::vector<std::string> player = { cmd[0] };

				std::vector<std::string> array = { "C_ACCEPT_INVITATION", this->_client[cmd[1]]->getNickName(), cmd[2] };
				auto toSend = packetBuilder(array);
				this->_client[cmd[0]]->clientWrite(toSend);

				auto room = this->_client[cmd[1]]->getRoom();
				this->_client[cmd[0]]->setRoom(room);

				this->playerRoom.push_back(std::make_pair(room, player));

				this->_client[cmd[1]]->setIfInviteSent(false);
				this->_client[cmd[0]]->setIfInviteSent(false);

				std::cout << "accepted by..." << this->_client[cmd[1]]->getNickName() << " host request..." << this->_client[cmd[0]]->getNickName() << std::endl;
			}
			else if (cmd[2] == "false")
			{
				std::vector<std::string> array = { "C_ACCEPT_INVITATION", this->_client[cmd[1]]->getNickName(), cmd[2] };
				auto toSend = packetBuilder(array);
				this->_client[cmd[0]]->clientWrite(toSend);

				this->_client[cmd[1]]->setRoom(0);
				this->_client[cmd[0]]->setRoom(0);
				this->_client[cmd[1]]->setIfInviteSent(false);
				this->_client[cmd[0]]->setIfInviteSent(false);

				std::cout << "declined by..." << this->_client[cmd[1]]->getNickName() << " host request..." << this->_client[cmd[0]]->getNickName() << std::endl;
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

void Server::destroyRoom(std::vector<std::string> cmd)
{
	/*auto room = this->_client[cmd[0]]->getRoom();
	this->_client[cmd[0]]->setRoom(0);
	this->playerRoom[room];*/
}