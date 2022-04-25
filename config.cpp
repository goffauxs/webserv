#include "config.hpp"
#include <stack>
#include <fstream>
#include <iostream>

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