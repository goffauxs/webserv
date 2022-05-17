#include "config.hpp"

Config::Config(const std::string& path)
{
	if (path.substr(path.find_last_of('.') + 1) != "conf")
		throw BadExtension();
	std::fstream ifs(path.c_str(), std::fstream::in);

	if (ifs.good())
	{
		if (ifs.peek() == std::fstream::traits_type::eof())
		{
			std::cerr << "Error: " << path << " is empty" << std::endl;
			exit(1);
		}
		if (has_uneven_brackets(ifs))
			return ;
		ifs.seekg(0, ifs.beg);
		char* server_str;
		std::fstream::pos_type begin = ifs.tellg();
		std::fstream::pos_type end;
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
			_list.push_back(new ServerConfig(tmp));
			delete [] server_str;
			begin = end;
		}
	}
	else
	{
		std::cerr << "Error: error opening file '" << path << "'" << std::endl;
		exit(1);
	}
}

Config::~Config()
{
	for (std::list<ServerConfig*>::iterator it = _list.begin(); it != _list.end(); it++)
		delete *it;
	_list.clear();
}

ServerConfig* Config::getServerConfig(int port, const std::string& server_name) const
{
	std::list<ServerConfig*> applicable;
	for (std::list<ServerConfig*>::const_iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it)->getPort() == port)
			applicable.push_back(*it);
	}
	if (applicable.size() > 1)
	{
		for (std::list<ServerConfig*>::const_iterator it = applicable.begin(); it != applicable.end(); it++)
			if ((*it)->getServerName() == server_name)
				return new ServerConfig(**it);
	}
	return new ServerConfig(*applicable.front());
}

std::list<ServerConfig*> Config::getServerList() const { return _list; }

int Config::has_uneven_brackets(std::fstream& stream)
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

void Config::advance_to_next_bracket(std::fstream& stream)
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
