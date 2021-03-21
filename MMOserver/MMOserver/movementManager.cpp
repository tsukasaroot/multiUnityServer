#include "Server.h"

void Server::playerMovement(Packet cmd)
{
	if (checkAll(6, cmd, &this->playerList))
	{
		auto playerNameToSend = cmd[0];
		auto getX = cmd[1];
		auto getY = cmd[2];
		auto getZ = cmd[3];
		auto room = this->_client[cmd[0]]->getRoom();
		std::string secondPlayer;

		if (cmd[0] == this->playerRoom[room][0])
			secondPlayer = this->playerRoom[room][1];
		else
			secondPlayer = this->playerRoom[room][0];

		Packet array = { "C_PLAYER_MOVEMENT", playerNameToSend, getX, getY, getZ };
		auto toSend = packetBuilder(array);
		this->_client[secondPlayer]->clientWrite(toSend);
	}
}