#ifndef XML_PARSER_H_
# define XML_PARSER_H_

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

struct OneOf {
	std::string chars;
	OneOf(const std::string& s) : chars(s) {}
	bool operator()(char c) const {
		return chars.find_first_of(c) != std::string::npos;
	}
};

class xmlParser
{
public:
	xmlParser(const char *const path);
	std::map<std::string, std::string> Data;
	std::vector<std::string> cleanData(std::string data);
	std::string cleanParent(std::string data);
private:
};

#endif