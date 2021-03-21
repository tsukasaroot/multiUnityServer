#include "Server.h"

std::map<std::string, std::vector<std::string>> Server::getServerConfig()
{
	auto reader = new xmlParser("config/serverConfig.xml");
	auto config = stockXML(reader);

	this->abnormalitiesTolerance = std::stoi(config["abnormalitiesTolerance"][0]);
	//this->debug = std::stob

	return config;
}

Server::Server()
{
	dataBase = new SQLManager();

	auto config = getServerConfig();

	this->serverPort = std::stoi(config["port"][0]);

	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;

	error = WSAStartup(MAKEWORD(2, 2), &initialisation_win32);
	if (error != 0)
		std::cout << "Can't initialize Winsock : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << std::endl << "WSAStartup : OK" << std::endl;

	this->serverRCV = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->serverRCV == INVALID_SOCKET)
		std::cout << "Can't initialiaze socket : " << WSAGetLastError() << std::endl;
	else
		std::cout << "socket : OK" << std::endl;

	ipep.sin_family = AF_INET;
	ipep.sin_addr.s_addr = INADDR_ANY;
	ipep.sin_port = htons(this->serverPort);
	error = bind(this->serverRCV, (struct sockaddr*)&ipep, sizeof(ipep));
	if (error != 0)
		std::cout << "Can't listen to this port : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "Bind : OK" << std::endl;
	std::cout << std::endl;
	
	// Will initialiaze the different XMLs needed for the game
	Opcodesinitialize();
	std::cout << std::endl << "$ Main server ready $" << std::endl << "Listening on: " << this->serverPort << std::endl << std::endl;
}

void Server::closeServer()
{
	error = closesocket(this->serverRCV);
	if (error != 0)
		std::cout << "Socket can't be freed : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "close socket : OK" << std::endl;
	 
	error = WSACleanup();
	if (error != 0)
		std::cout << "Winsock can't be freed : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "WSACleanup  : OK" << std::endl;
	exit(0);
}

void Server::sendCountDown()
{
	std::vector<size_t> roomDone;

	for (auto it = this->_client.begin(); it != this->_client.end(); it++)
	{
		size_t room;
		int countdown = it->second->getIfOnCountdown();
		room = it->second->getRoom();

		auto searchForRoom = std::find(roomDone.begin(), roomDone.end(), room);
		
		if (searchForRoom == roomDone.end())
		{
			roomDone.push_back(room);
			if (countdown > 0 && countdown < 10)
			{
				auto player1 = this->playerRoom[room][0];
				auto player2 = this->playerRoom[room][1];

				Packet array = { "C_COUNTDOWN_START", std::to_string(countdown) };
				auto toSend = packetBuilder(array);
				this->_client[player1]->clientWrite(toSend);
				this->_client[player2]->clientWrite(toSend);

				std::cout << "sending " << countdown << " to " << player1 << " " << player2 << std::endl;

				countdown--;

				if (countdown == 0)
					countdown = 10;

				this->_client[player1]->setIsOnCountdown(countdown);
				this->_client[player2]->setIsOnCountdown(countdown);
			}
			else if (countdown == 10)
			{
				Packet array = { "C_START" };
				auto toSend = packetBuilder(array);

				room = it->second->getRoom();

				auto player1 = this->playerRoom[room][0];
				auto player2 = this->playerRoom[room][1];

				this->_client[player1]->clientWrite(toSend);
				this->_client[player2]->clientWrite(toSend);

				countdown = 0;

				this->_client[player1]->setIsOnCountdown(countdown);
				this->_client[player2]->setIsOnCountdown(countdown);
			}
		}
	}
}

void Server::saveWorld()
{
	for (auto it = this->_client.begin(); it != this->_client.end(); it++)
	{
		it->second->saveClientToDatabase();
	}
}

SOCKET Server::getSocket()
{
	return this->serverRCV;
}

struct timeval Server::getTimeVal()
{
	return this->read_timeout;
}

SOCKADDR_IN Server::getIpep()
{
	return this->ipep;
}