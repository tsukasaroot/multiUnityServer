#include "SQLmanager.h"

SQLManager::SQLManager()
{
	this->reader = new xmlParser("config/DatabaseConfig.xml");
	this->config = stockXML(this->reader);

	this->server = "tcp://";
	this->server += config["hostName"][0] + ':' + config["port"][0];
	this->server += '/' + config["database"][0];

	this->userName = config["username"][0];
	if (this->config.find("password") != this->config.end())
		this->password = config["password"][0];
	this->port = std::stoi(config["port"][0]);

	bool reconnector;

	connection_properties["hostName"] = this->server;
	connection_properties["userName"] = this->userName;
	connection_properties["password"] = this->password;
	connection_properties["port"] = this->port;
	std::istringstream(this->config["reconnect"][0]) >> std::boolalpha >> reconnector;
	connection_properties["OPT_RECONNECT"] = reconnector;

	try
	{
		driver = get_driver_instance();
		this->con = driver->connect(connection_properties);
		std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "SQLManager initialiazed" << std::endl;
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}
}

void SQLManager::insert(std::string table, std::string column, std::string values)
{
	sql::Statement* stmt;
	std::string query = "INSERT INTO " + table + "(" + column + ") VALUES(" + values +")";

	std::cout << query << std::endl;

	stmt = con->createStatement();
	stmt->executeUpdate(query);
}

void SQLManager::update(std::string user, std::string cond, std::string table, std::vector<std::pair<std::string, std::string>> values)
{
	sql::Statement* stmt;
	std::string query = "UPDATE " + table + " SET ";
	std::string condition = " WHERE " + cond + " = '" + user + "'";

	for (auto it = values.begin(); it != values.end(); it++)
	{
		query += it->first + " = '" + it->second + "'";
		if (it + 1 != values.end())
			query += ", ";
	}
	query += condition;
		
	try
	{
		stmt = con->createStatement();
		stmt->executeUpdate(query);
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
	delete stmt;
}

void SQLManager::get(std::string table, std::vector<std::string> fields, std::vector<std::string> columnName, std::vector<std::string> where)
{
	// to refactor

	sql::Statement* stmt;
	sql::ResultSet* res;
	std::string query = "SELECT ";
	std::string condition = " WHERE ";

	for (auto it = fields.begin(); it != fields.end(); it++)
	{
		query += *it;
		if (it + 1 != fields.end())
			query += ',';
	}
	query += " FROM " + table + condition;
	
	for (size_t i = 0; i < where.size() && i < columnName.size(); i++)
	{
		query += columnName[i] + "=" + where[i];
		if (i + 1 != where.size() && i + 1 != columnName.size())
			query += " AND ";
	}
	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery(query);
		std::map<int, std::vector<double>> inventories;
		std::vector<double> items_id;
		
		while (res->next())
		{
			items_id.push_back(res->getDouble("item_id"));
			inventories.insert(std::pair<int, std::vector<double>>(res->getInt("player_id"), items_id));
		}
		items_id.clear();
		delete res;
		delete stmt;
		for (auto it = inventories.begin(); it != inventories.end(); it++)
		{
			std::cout << "player " << it->first << std::endl;
			for (auto itI = it->second.begin(); itI != it->second.end(); itI++)
			{
				std::cout << "Has those items in inventory: " << *itI << std::endl;
			}
		}
		return;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
}

std::map<std::string, std::string> SQLManager::initPlayer(std::string nickName)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT * FROM accounts WHERE name = '" + nickName + "'";
	std::map<std::string, std::string> result;
	int id = 0;

	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("client_id", std::to_string(res->getInt("id"))));
		}

		delete res;
		delete stmt;

		return result;
	}
	catch (sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
}

std::map<std::string, std::string> SQLManager::checkLogin(std::string nickName)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT name, password FROM accounts WHERE name LIKE '" + nickName + "'";
	std::map<std::string, std::string> result;

	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("name", res->getString("name")));
			result.insert(std::pair<std::string, std::string>("password", res->getString("password")));
		}

		delete res;
		delete stmt;
		return result;
	}
	catch (sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
}