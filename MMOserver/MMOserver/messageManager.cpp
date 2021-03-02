#include "messageManager.h"

void  messageManager::sendPrivateMessage(Client* clientFrom, Client* clientToSend, std::string message)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	std::string mkdir = "mkdir messagesDumper\\" + clientFrom->getNickName();
	std::string writeFile = "echo \"" + std::to_string(time.wHour) + ':' + std::to_string(time.wMinute) + " - to:" + clientToSend->getNickName() + "  -  " + message + "\" >> messagesDumper\\" + clientFrom->getNickName() + '\\' + std::to_string(time.wDay) + '-' +  std::to_string(time.wMonth) + '-' + std::to_string(time.wYear);

	if (fs::exists("messagesDumper\\" + clientFrom->getNickName()) == false)
		std::system(mkdir.c_str());
	std::system(writeFile.c_str());

	std::vector<std::string> array = { "C_GETMESSAGE", clientFrom->getNickName(), message };
	auto toSend = packetBuilder(array);

	clientToSend->clientWrite(toSend);
}

void messageManager::sendGlobalMessage(Client* clientFrom, std::string message, std::map<std::string, Client*> clientsToSend)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	std::string mkdir = "mkdir messagesDumper\\" + clientFrom->getNickName();
	std::string writeFile = "echo \"" + std::to_string(time.wHour) + ':' + std::to_string(time.wMinute) + "  -  " + message + "\" >> messagesDumper\\" + clientFrom->getNickName() + '\\' + std::to_string(time.wDay) + '-' + std::to_string(time.wMonth) + '-' + std::to_string(time.wYear);

	if (fs::exists("messagesDumper\\" + clientFrom->getNickName()) == false)
		std::system(mkdir.c_str());
	std::system(writeFile.c_str());
	
	std::vector<std::string> array = { "C_GETMESSAGE", clientFrom->getNickName(), message };
	auto toSend = packetBuilder(array);

	for (auto it = clientsToSend.begin(); it != clientsToSend.end(); it++)
	{
		if (it->first != clientFrom->getNickName())
			it->second->clientWrite(toSend);
	}
}
