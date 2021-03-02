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

	this->x = std::stod(cmd["x"]);
	this->y = std::stod(cmd["y"]);
	this->z = std::stod(cmd["z"]);
	this->currency = std::stod(cmd["currency"]);
	this->exp = std::stod(cmd["exp"]);
	this->HP = std::stod(cmd["hp"]);
	this->MP = std::stoi(cmd["mp"]);
	this->attack = std::stod(cmd["attack"]);
	this->critRate = std::stod(cmd["critRate"]);
	this->critP = std::stod(cmd["critP"]);
	this->defense = std::stod(cmd["defense"]);
	this->clientClass = std::stoi(cmd["class"]);
	this->level = std::stoi(cmd["level"]);
	this->region = std::stoi(cmd["region"]);
	this->isAlive = std::stoi(cmd["isAlive"]);
	this->RE = std::stoi(cmd["re"]);
	this->client_id = std::stoi(cmd["client_id"]);

	std::vector<std::string> array = { "C_LOGIN_DATA", this->nickName, 
		cmd["x"], cmd["y"], cmd["z"], cmd["currency"], cmd["exp"], cmd["hp"], cmd["mp"], cmd["attack"], cmd["critRate"], cmd["critP"], cmd["defense"],
		cmd["class"], cmd["level"], cmd["region"], cmd["re"],
		cmd["isAlive"]
	};

	data = packetBuilder(array);
	this->clientWrite(data);
}

void Client::closeClient()
{
	saveClientToDatabase();
	std::cout << "Client logged out: " << this->nickName << std::endl;
}

void Client::clientWrite(std::string msg)
{
	this->ipep.sin_family = AF_INET;
	this->ipep.sin_addr.s_addr = inet_addr(this->clientAddress.c_str()); // Indique l'adresse IP du client qui a �t� push
	this->ipep.sin_port = htons(16384);
	msg = msg + "0x12" + '\n';
	strcpy_s(this->buffer, msg.c_str());
	this->bytes = sendto(this->_client, this->buffer, strlen(this->buffer), 0, (struct sockaddr*)&this->ipep, sizeof(this->ipep));
	if (this->bytes == SOCKET_ERROR)
		std::cout << "Can't send data: " << WSAGetLastError() << std::endl;
}

void Client::saveClientToDatabase()
{
	std::vector<std::pair<std::string, std::string>> values;

	values.push_back(std::make_pair((std::string)"x", std::to_string(this->x)));
	values.push_back(std::make_pair((std::string)"y", std::to_string(this->y)));
	values.push_back(std::make_pair((std::string)"z", std::to_string(this->z)));
	values.push_back(std::make_pair((std::string)"region", std::to_string(this->region)));
	values.push_back(std::make_pair((std::string)"currency", std::to_string(this->currency)));
	values.push_back(std::make_pair((std::string)"class", std::to_string(this->clientClass)));
	values.push_back(std::make_pair((std::string)"exp", std::to_string(this->exp)));
	values.push_back(std::make_pair((std::string)"isAlive", std::to_string(this->isAlive)));

	this->dataBase->update(this->nickName, "name", "users", values);
	values.clear();

	values.push_back(std::make_pair((std::string)"hp", std::to_string(this->HP)));
	values.push_back(std::make_pair((std::string)"mp", std::to_string(this->MP)));
	values.push_back(std::make_pair((std::string)"attack", std::to_string(this->attack)));
	values.push_back(std::make_pair((std::string)"critRate", std::to_string(this->critRate)));
	values.push_back(std::make_pair((std::string)"critP", std::to_string(this->critP)));
	values.push_back(std::make_pair((std::string)"defense", std::to_string(this->defense)));
	values.push_back(std::make_pair((std::string)"re", std::to_string(this->RE)));
	this->dataBase->update(std::to_string(this->client_id), "user_id", "currentplayerstats", values);
}

/*
** Setters method
*/

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