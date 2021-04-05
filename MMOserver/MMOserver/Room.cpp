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

		if (this->_client[this->playerRoom[room][0]]->getHost())
			this->_client[this->playerRoom[room][0]]->setHost();
		if (this->_client[this->playerRoom[room][1]]->getHost())
			this->_client[this->playerRoom[room][1]]->setHost();

		delete(&this->playerRoom[room]);
		this->playerRoom.erase(room);
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

void Server::playerReady(Packet cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		auto host = this->playerRoom[this->_client[cmd[0]]->getRoom()][0];
		auto guest = this->playerRoom[this->_client[cmd[0]]->getRoom()][1];

		if (host == cmd[0] && !this->_client[host]->getIsReady())
		{
			this->_client[host]->setIsReady();
			std::cout << host << " is ready" << std::endl;
		}
		else if (guest == cmd[0] && !this->_client[guest]->getIsReady())
		{
			this->_client[guest]->setIsReady();
			std::cout << guest << " is ready" << std::endl;
		}

		if (this->_client[guest]->getIsReady() && this->_client[host]->getIsReady())
		{
			this->_client[host]->setIsOnCountdown(3);
			this->_client[guest]->setIsOnCountdown(3);

			std::cout << "Countdown starting" << std::endl;
		}
	}
}

void Server::raceEndForOnePlayer(Packet cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		this->_client[cmd[0]]->setTimerDiff();
		auto result = this->_client[cmd[0]]->getTimerDiff();

		Packet array = { "C_TIME", std::to_string(result) };
		auto toSend = packetBuilder(array);
		this->_client[cmd[0]]->clientWrite(toSend);

		std::string values = "'" + cmd[0] + "'" +  ",'" + cmd[1] +"'," + "'" + std::to_string(result) + "'";

		this->dataBase->insert("users", "name,map,score", values);
	}
}

void Server::playerleaveRace(Packet cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		Packet array = { "C_LEAVE", cmd[1] };
		auto toSend = packetBuilder(array);

		auto room = this->_client[cmd[0]]->getRoom();

		std::string player1 = this->playerRoom[room][0];
		std::string player2 = this->playerRoom[room][1];

		this->_client[player1]->clientWrite(toSend);
		this->_client[player2]->clientWrite(toSend);

		array = { "C_DEFINE_ROOM", "0" };
		auto toSendDefineRoom = packetBuilder(array);

		this->_client[player1]->setRoom(0);
		this->_client[player1]->clientWrite(toSendDefineRoom);
		this->_client[player1]->setInRace();

		this->_client[player2]->setRoom(0);
		this->_client[player2]->clientWrite(toSendDefineRoom);
		this->_client[player2]->setInRace();

		if (this->_client[player2]->getHost())
			this->_client[player2]->setHost();
		if (this->_client[player1]->getHost())
			this->_client[player1]->setHost();

		this->_client[player1]->setIsReady();
		this->_client[player2]->setIsReady();
	
		this->playerRoom.erase(room);
	}
}