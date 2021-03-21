#include "Client.h"

void Client::initClient(std::string ip, std::string nickName, SOCKET serverRCV, SQLManager *db, std::string port)
{
	std::cout << "New client logging: " << nickName << std::endl;

	this->dataBase = db;
	this->inventory = new inventoryManager(db);
	this->_client = serverRCV;
	this->clientAddress = ip;
	this->nickName = nickName;
	this->port = std::stoi(port);

	auto reader = new xmlParser("config/clientConfig.xml");
	auto config = stockXML(reader);

	this->movementTolerance = std::stof(config["movementTolerance"][0]);

	clientWrite("C_LOGIN:" + nickName);
}

void Client::initClient(std::map<std::string, std::string> cmd)
{
	std::string data;

	this->client_id = std::stoi(cmd["client_id"]);
}

void Client::closeClient()
{
	//saveClientToDatabase();
	std::cout << "Client logged out: " << this->nickName << std::endl;
}

void Client::clientWrite(std::string msg)
{
	this->ipep.sin_family = AF_INET;
	this->ipep.sin_addr.s_addr = inet_addr(this->clientAddress.c_str()); // Indique l'adresse IP du client qui a été push
	this->ipep.sin_port = htons(this->port);
	msg = msg + "0x12" + '\n';
	strcpy_s(this->buffer, msg.c_str());
	this->bytes = sendto(this->_client, this->buffer, strlen(this->buffer), 0, (struct sockaddr*)&this->ipep, sizeof(this->ipep));
	if (this->bytes == SOCKET_ERROR)
		std::cout << "Can't send data: " << WSAGetLastError() << std::endl;
}

void Client::saveClientToDatabase()
{
	std::vector<std::pair<std::string, std::string>> values;
}

/*
** Setters method
*/

void Client::setIsOnCountdown(int c)
{
	this->onCountdown = c;
}

void Client::setIsReady()
{
	this->isReady = !this->isReady;
}

void Client::setHost()
{
	this->isHost = !this->isHost;
}

void Client::setIfInviteSent(bool invite)
{
	this->invitationSent = invite;
}

void Client::setRoom(int room)
{
	this->inRoomNumber = room;
}

void Client::setPositionQuery(std::vector<std::string> cmd)
{
	this->positionQuery--;

	auto x = std::stod(cmd[1]);
	auto y = std::stod(cmd[2]);
	auto z = std::stod(cmd[3]);

	double xChecker = abs(x - this->x);
	double yChecker = abs(z - this->z);


	if (xChecker > this->movementTolerance)
	{
		this->abnormal++;
		clientWrite("C_CORRECTPOSITION:x:" + std::to_string(this->x));
	}
	else
	{
		this->x = x;
	}

	if (yChecker > this->movementTolerance)
	{
		this->abnormal++;
		clientWrite("C_CORRECTPOSITION:z:" + std::to_string(this->z));
	}
	else
	{
		this->z = z;
	}
}

/*
** Getters method
*/

int Client::getIfOnCountdown()
{
	return this->onCountdown;
}

bool Client::getIsReady()
{
	return this->isReady;
}

bool Client::getHost()
{
	return this->isHost;
}

bool Client::getIfInviteSent()
{
	return this->invitationSent;
}

int Client::getRoom()
{
	return this->inRoomNumber;
}

int Client::getAbnormalities()
{
	return this->abnormal;
}

SOCKET Client::getClientSocket()
{
	return this->_client;
}

std::string Client::getClientAddress()
{
	return this->clientAddress;
}

std::string Client::getNickName()
{
	return this->nickName;
}

double Client::getX()
{
	return this->x;
}

double Client::getY()
{
	return this->y;
}

double Client::getZ()
{
	return this->z;
}

std::string Client::getProfile()
{
	std::string toSend = this->nickName + ':' + getAll();

	return toSend;
}

std::string Client::getAll()
{
	std::string toSend;

	toSend = std::to_string(this->MP) + ':' + std::to_string(this->HP) + ':' + std::to_string(this->RE) + ':' + std::to_string(this->currency) + ':' + std::to_string(this->clientClass) + ':' +
		std::to_string(this->x) + ':' + std::to_string(this->y) + ':' + std::to_string(this->z);
	return toSend;
}