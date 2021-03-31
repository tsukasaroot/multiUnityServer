#include "Server.h"

void Server::logout(std::vector<std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string nickName = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), nickName);

		if (it != this->playerList.end())
		{
			size_t room = 0;

			if (room = this->_client[nickName]->getRoom() > 0)
			{
				auto room = this->_client[cmd[0]]->getRoom();

				auto searchForHost = std::find(this->playerList.begin(), this->playerList.end(), this->playerRoom[room][0]);
				auto searchForGuest = std::find(this->playerList.begin(), this->playerList.end(), this->playerRoom[room][1]);

				std::string player2;
				std::string player1;

				if (searchForHost != this->playerList.end())
					player2 = this->playerRoom[room][1];
				if (searchForGuest != this->playerList.end())
					player1 = this->playerRoom[room][0];

				Packet array = { "C_DEFINE_ROOM", "0" };
				auto toSend = packetBuilder(array);

				array = { "C_LEAVE", "MainMenu" };
				auto toSendToQuit = packetBuilder(array);

				if (searchForHost != this->playerList.end())
				{
					this->_client[player1]->setRoom(0);
					this->_client[player1]->clientWrite(toSend);
					this->_client[player1]->clientWrite(toSendToQuit);
					this->_client[player1]->setInRace();
				}

				if (searchForGuest != this->playerList.end())
				{
					this->_client[player2]->setRoom(0);
					this->_client[player2]->clientWrite(toSend);
					this->_client[player2]->clientWrite(toSendToQuit);
					this->_client[player2]->setInRace();
				}
				this->playerRoom.erase(room);
			}

			this->playerList.erase(it);
			this->_client[nickName]->closeClient();
			delete(this->_client[nickName]);
			this->_client.erase(nickName);
			return;
		}
		std::cerr << "Error: " << nickName << " player already logged out, logout command denied." << std::endl;
		return;
	}
	else
	{
		std::cout << "Not enough argument for this function" << std::endl;
	}
}

void Server::login(std::vector<std::string> cmd)
{
	if (cmd.size() == 4)
	{
		std::string login = cmd[0];
		std::string password = cmd[1];
		auto result = this->dataBase->checkLogin(login);

		if (std::find(this->playerList.begin(), this->playerList.end(), login) != this->playerList.end())
		{
			std::cerr << "Error: " << login << " player already logged, connection denied." << std::endl;
			return;
		}
		if (login == result["name"] && password == result["password"])
		{
			this->playerList.push_back(login);
			this->_client.insert(std::pair<std::string, Client*>(login, new Client()));
			_client[login]->initClient(cmd[2], result["name"], this->serverRCV, this->dataBase, cmd[3]);

			auto datas = this->dataBase->initPlayer(login);
			//_client[login]->initClient(datas);
			return;
		}
	}
	else
	{
		std::cout << "Not enough argument for this function" << std::endl;
	}
}

void Server::savePlayerData(std::vector<std::string> cmd)
{
	std::vector<std::pair<std::string, std::string>> values;

	values.push_back(std::make_pair("email", "jorouco@free.fr"));
	dataBase->update("test", "name", "users", values);
}