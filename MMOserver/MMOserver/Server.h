#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include "SQLmanager.h"
#include "xmlParser.h"
#include "npcSpawner.h"
#include "itemManager.h"
#include "messageManager.h"
#include "skillManager.h"

/*
* Check if opcode contain the right amount of args and if a valid player exist
* 
* @param size of the command, the command itself and the player list contained in class Server
* @return a boolean depending if all test succeed
*/

typedef std::vector<std::string> Packet;

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader);
bool checkAll(int size, std::vector<std::string> cmd, std::vector<std::string>* playerList);

class Server {
public:
	Server();
	void processOpcodes(std::vector<std::string> opcodes, std::string ip);
	//void clientChecks();
	void saveWorld();
	void createAI();

	SOCKET getSocket();
	struct timeval getTimeVal();
	SOCKADDR_IN getIpep();
	void sendCountDown();
private:
	std::map<std::string, std::vector<std::string>> getServerConfig();

	int serverPort = 16384;
	struct timeval read_timeout;
	WSADATA initialisation_win32;
	int error;
	SOCKET serverRCV;
	SOCKADDR_IN ipep;

	/**
	* Log the client server-side.
	*
	* @param Take a cmd string that contain IP, nickname and password.
	* @return no return values.
	*/
	void login(std::vector<std::string> cmd);
	/**
	* Logout the client server-side.
	*
	* @param Take a cmd string that contain IP, nickname.
	* @return no return values.
	*/
	void logout(std::vector<std::string> cmd);
	/**
	* Save players data server-side.
	*
	* @param Take a cmd string that contain all clients informations.
	* @return no return values.
	*/
	void savePlayerData(std::vector<std::string> cmd);
	/**
	* Get Client Data server-side.
	*
	* @param Take a cmd string that contain clients info we want and send info back.
	* @return no return values.
	*/
	void sendClientData(std::vector<std::string> cmd);
	/**
	* Send a private message to a connected client
	* 
	* @param Take the current client that wants to send the message, the second client that will receive it, and finally the string
	* @return no return values
	*/
	void privateMessage(std::vector<std::string> cmd);
	/**
	* Send a global message to all connected client
	*
	* @param Take the current client that wants to send the message and finally the string
	* @return no return values
	*/
	void globalMessage(std::vector<std::string> cmd);
	/**
	* Send to the asking client all the connected clients name
	*
	* @param Take the current client that wants the list
	* @return no return values
	*/
	void sendAllClientsName(std::vector<std::string> cmd);

	void sendProfile(std::vector<std::string> cmd);

	void getPosition(std::vector<std::string> cmd);

	void addRoom(Packet cmd);

	void joinRoom(Packet cmd);

	void destroyRoom(Packet cmd);

	void startGame(Packet cmd);

	void playerReady(Packet cmd);

	void playerMovement(Packet cmd);

	void Opcodesinitialize();
	void closeServer();

	typedef void(Server::* opcodes)(std::vector<std::string>);

	std::map<std::string, opcodes> list;
	std::vector<std::string> playerList;

	skillManager* skills;
	messageManager* messages;
	itemsManager* items;
	npcSpawner* npcSpawn;
	xmlParser* reader;
	std::map<std::string, Client*> _client;
	std::map<std::string, std::string> Movements;
	SQLManager *dataBase;

	int abnormalitiesTolerance = 10;
	std::map<size_t, std::vector<std::string>> playerRoom;
	
	bool debug = false;
};

#endif