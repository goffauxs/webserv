#include "config.hpp"
#include <stack>
#include <fstream>
#include <iostream>

Config::Config(const std::string& path)
{
	std::ifstream ifs("default.conf", std::ifstream::in);

	if (ifs.good())
	{
		if (has_uneven_brackets(ifs))
			return ;
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
			_list.push_back(ServerConfig(tmp));
			delete server_str;
			begin = end;
		}
	}
}

int Config::has_uneven_brackets(std::ifstream& stream)
{
	std::stack<int> brackets;

	size_t line_counter = 1;
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
				throw BracketException(CLOSED, line_counter);
		}
		if (stream.peek() == 10)
			line_counter++;
	}
	if (brackets.empty())
		return 0;
	else
		throw BracketException(OPEN, line_counter);
}

void Config::advance_to_next_bracket(std::ifstream& stream)
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
		throw UnexpectedToken();
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