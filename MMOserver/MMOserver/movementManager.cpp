#include "Server.h"

void Server::playerMovement(Packet cmd)
{
	if (checkAll(6, cmd, &this->playerList))
	{
		auto getX = cmd[1];
		auto getY = cmd[2];
		auto getZ = cmd[3];
		auto searchForPlayer1 = std::find(this->playerList.begin(), this->playerList.end(), cmd[0]);

		if (searchForPlayer1 == this->playerList.end() || this->_client[cmd[0]]->getRoom() == 0)
		{
			return;
		}

		auto room = this->_client[cmd[0]]->getRoom();
		std::string secondPlayer = this->playerRoom[room][1];
		auto searchForPlayer2 = std::find(this->playerList.begin(), this->playerList.end(), secondPlayer);

		if (searchForPlayer2 == this->playerList.end() || this->_client[secondPlayer]->getRoom() == 0)
		{
			return;
		}
		
		if (cmd[0] == this->playerRoom[room][0])
		{
			secondPlayer = this->playerRoom[room][1];
		}
		else
		{
			secondPlayer = this->playerRoom[room][0];
		}

		Packet array = { "C_PLAYER_MOVEMENT", getX, getY, getZ };
		auto toSend = packetBuilder(array);
		this->_client[secondPlayer]->clientWrite(toSend);
	}
}