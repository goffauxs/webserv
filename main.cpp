#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stack>
#include <list>
#include "utils.hpp"
#include "server_config.hpp"
#include "location_config.hpp"

int has_uneven_brackets(std::ifstream& stream)
{
	std::stack<int> brackets;

	int line_counter = 1;
	char curr_char;
	while (!stream.eof())
	{
		stream.get(curr_char);
		if (curr_char == '{')
			brackets.push(line_counter);
		else if (curr_char == '}')
		{
			if (brackets.size())
				brackets.pop();
			else
			{
				std::cerr << "Error: Extra '}' on line " << line_counter << std::endl;
				return 1;
			}
		}
		if (stream.peek() == 10)
			line_counter++;
	}
	if (brackets.empty())
		return 0;
	else
	{
		std::cerr << "Error: Unclosed '{' on line " << brackets.top() << std::endl;
		return 1;
	}
}

void advance_to_next_bracket(std::ifstream& stream)
{
	std::stack<bool> brackets;

	char curr_char;
	std::string server;
	while (server != "server" && !stream.eof())
		stream >> server;
	if (stream.eof())
		return;
	stream >> server;
	if (server == "{")
		brackets.push(true);
	else
		std::cerr << "Error: Unexpected token after server" << std::endl;
	while (!stream.eof())
	{
		stream.get(curr_char);
		if (curr_char == '{')
			brackets.push(true);
		else if (curr_char == '}')
			brackets.pop();
		if (brackets.empty())
			return ;
	}
}

int main()
{
	std::list<ServerConfig> list;
	std::ifstream ifs("default.conf", std::ifstream::in);

	if (ifs.good())
	{
		if (has_uneven_brackets(ifs))
			return 1;
		ifs.seekg(0, ifs.beg);
		char* server_str;
		std::ifstream::streampos begin = ifs.tellg();
		std::ifstream::streampos end;
		std::streamsize diff;
		while (!ifs.eof())
		{
			advance_to_next_bracket(ifs);
			if (ifs.eof())
				break;
			end = ifs.tellg();
			diff = end - begin;
			ifs.seekg(begin);
			server_str = new char[diff + 1];
			ifs.read(server_str, diff);
			server_str[diff] = '\0';
			std::string tmp(server_str);
			list.push_back(ServerConfig(tmp));
			delete server_str;
			begin = end;
		}
	}
	// for (std::list<ServerConfig>::iterator it = list.begin(); it != list.end(); it++)
	// {
	// 	std::cout << "--- SERVER ---" << std::endl;
	// 	std::cout << "\tserver_name: " << it->getServerName() << std::endl;
	// 	std::cout << "\tindex: " << it->getIndex() << std::endl;
	// 	std::cout << "\troot: " << it->getRoot() << std::endl;
	// 	std::cout << "\thost: " << it->getHost() << std::endl;
	// 	std::cout << "\tport: " << it->getPort() << std::endl;
	// 	std::cout << "\tallowed_methods: ";
	// 	for (std::set<Method>::iterator itr = it->getAllowedMethods().begin(); itr != it->getAllowedMethods().end(); itr++)
	// 		std::cout << to_string(*itr);
	// 	std::cout << std::endl;
	// 	for (std::map<std::string, LocationConfig>::const_iterator itr = it->getLocations().begin(); itr != it->getLocations().end(); itr++)
	// 	{
	// 		std::cout << "\t--- LOCATION ---" << std::endl;
	// 		std::cout << "\t\tpath: " << itr->second.getPath() << std::endl;
	// 		std::cout << "\t\tallowed_methods: ";
	// 		std::set<Method> allowed_methods = itr->second.getAllowedMethods();
	// 		for (std::set<Method>::iterator itr2 = allowed_methods.begin(); itr2 != allowed_methods.end(); itr2++)
	// 			std::cout << to_string(*itr2);
	// 		std::cout << std::endl;
	// 		std::cout << "\t\tautoindex: " << std::boolalpha << itr->second.isAutoIndexed() << std::endl;
	// 		std::cout << "\t\tuploadable: " << std::boolalpha << itr->second.isUploadable() << std::endl;
	// 		std::cout << "\t\tupload_dir: " << itr->second.getUploadDir() << std::endl;
	// 		std::cout << "\t\troot: " << itr->second.getRoot() << std::endl;
	// 		std::cout << "\t\tindex: " << itr->second.getIndex() << std::endl;
	// 	}
	// }
	return 0;
}
