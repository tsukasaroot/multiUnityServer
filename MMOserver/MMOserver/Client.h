#ifndef CLIENT_H_
#define CLIENT_H_

#include "inventoryManager.h"
#include "SQLmanager.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <time.h>

typedef std::vector<std::string> Packet;
std::string packetBuilder(Packet requests);

class Client {
public:
	Client() {};
	~Client() {};
	void closeClient();
	void initClient(std::string client, std::string nickName, SOCKET serverRCV, SQLManager* db, std::string port);
	void initClient(std::map<std::string, std::string> cmd);

	void clientWrite(std::string msg);

	void queryClientPosition();

	double getX();
	double getY();
	double getZ();
	int getRoom();
	bool getIfInviteSent();
	bool getHost();
	bool getIsReady();
	int getIfOnCountdown();
	double getTimerDiff();
	bool getInRace();

	void setTimerDiff();
	void setTimerBegin();
	void setPositionQuery(std::vector<std::string> cmd);
	void saveClientToDatabase();
	void setRoom(int room);
	void setIfInviteSent(bool invite);
	void setHost();
	void setIsReady();
	void setIsOnCountdown(int c);
	void setInRace();

	std::string getAll();
	std::string getProfile();

	int getAbnormalities();
	SOCKET getClientSocket();
	std::string getClientAddress();
	std::string getNickName();
private:
	int port = 16384;
	std::string clientAddress;
	std::string nickName;
	std::string ip;
	double x = 0, y = 0, z = 0, HP = 0, currency = 0, exp = 0, attack = 0, critRate = 0,
		critP = 0, defense = 0;
	int MP = 0, RE = 0, clientClass = 0, positionQuery = 0, region = 0, level = 0, client_id = 0;
	bool isAlive = true;

	int abnormal = 0, modifier = 1;
	double movementTolerance = 6.1;
	int inRoomNumber = 0;
	bool invitationSent = false;
	bool isHost = false, isReady = false, inRace = false;
	int onCountdown = 0;

	inventoryManager* inventory;
	SQLManager* dataBase;

	SOCKET _client = 0;

	time_t begin;
	float result;

	WSADATA initialisation_win32; // Variable permettant de r�cup�rer la structure d'information sur l'initialisation
	int error = 0, tempo = 0, bytes = 0;
	char buffer[4024] = ""; // Tampon contenant les donn�es re�ues ou envoy�es
	SOCKADDR_IN ipep; // D�claration de la structure des informations li� au serveur
};

#endif