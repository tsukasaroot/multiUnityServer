#include "Server.h"

void Server::addRoom(Packet cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		auto searchForGuest = std::find(this->playerList.begin(), this->playerList.end(), cmd[1]);

		if (this->_client[cmd[0]]->getIfInviteSent() == true)
		{
			return;
		}

		if (searchForGuest != this->playerList.end() && this->_client[cmd[0]]->getNickName() != this->_client[cmd[1]]->getNickName())
		{
			if (this->_client[cmd[0]]->getRoom() > 0 && this->_client[cmd[1]]->getRoom() > 0)
			{
				return;
			}

			std::vector<std::string> host = { cmd[0] };
			size_t room = this->playerRoom.size() + 1;

			this->playerRoom.insert(std::pair<size_t, std::vector<std::string>>(room, host));
			this->_client[cmd[0]]->setRoom(room);
			this->_client[cmd[0]]->setHost();

			Packet array = { "C_SENDROOM_INVITATION", this->_client[cmd[0]]->getNickName(), std::to_string(this->_client[cmd[0]]->getRoom()) };
			auto toSend = packetBuilder(array);
			this->_client[cmd[1]]->clientWrite(toSend);

			Packet arrayHost = { "C_DEFINE_ROOM_HOST", std::to_string(this->_client[cmd[0]]->getRoom()) };
			toSend = packetBuilder(arrayHost);
			this->_client[cmd[0]]->clientWrite(toSend);

			this->_client[cmd[0]]->setIfInviteSent(true);

			std::cout << "invite sent by " << cmd[0] << " to " << cmd[1] << std::endl;

		}
		else
		{
			Packet array = { "C_ACCEPT_INVITATION", cmd[1], "undefined" };
			auto toSend = packetBuilder(array);
			this->_client[cmd[0]]->clientWrite(toSend);
			std::cout << "Player doesn't exist..." << std::endl;
		}
	}
}

void Server::joinRoom(Packet cmd)
{
	if (checkAll(5, cmd, &this->playerList))
	{
		// cmd[0] == host
		// cmd[1] == client
		// to refactor for better comprehension

		auto searchForHost = std::find(this->playerList.begin(), this->playerList.end(), cmd[0]);

		if (searchForHost != this->playerList.end())
		{
			if (cmd[2] == "true" && this->_client[cmd[0]]->getIfInviteSent() == true)
			{

				std::vector<std::string> array = { "C_ACCEPT_INVITATION", this->_client[cmd[1]]->getNickName(), cmd[2] };
				auto toSend = packetBuilder(array);
				this->_client[cmd[0]]->clientWrite(toSend);

				auto room = this->_client[cmd[0]]->getRoom();
				this->_client[cmd[1]]->setRoom(room);

				this->playerRoom[room].push_back(cmd[1]);

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
			Packet array = { "C_ACCEPT_INVITATION", cmd[1], "undefined" };
			auto toSend = packetBuilder(array);
			this->_client[cmd[0]]->clientWrite(toSend);
		}
	}
}

void Server::destroyRoom(Packet cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		auto room = this->_client[cmd[0]]->getRoom();

		this->_client[this->playerRoom[room][0]]->setRoom(0);
		this->_client[this->playerRoom[room][1]]->setRoom(0);
		
		Packet array = { "C_DEFINE_ROOM", "0" };
		auto toSend = packetBuilder(array);
		this->_client[this->playerRoom[room][0]]->clientWrite(toSend);
		this->_client[this->playerRoom[room][1]]->clientWrite(toSend);
	}
}

void Server::startGame(Packet cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		if (this->_client[cmd[0]]->getHost())
		{
			std::cout << "starting game..." << std::endl;

			auto guest = this->playerRoom[this->_client[cmd[0]]->getRoom()][1];

			Packet array = { "C_HOST_START_GAME", cmd[1] };
			auto toSend = packetBuilder(array);
			this->_client[guest]->clientWrite(toSend);
			this->_client[cmd[0]]->clientWrite(toSend);
		}
	}
}