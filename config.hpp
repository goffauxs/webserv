#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "server_config.hpp"
#include "utils.hpp"
#include <list>

#define OPEN true
#define CLOSED false

class Config
{
public:
	Config(const std::string& path);
	// Config(const Config& other);
	Config& operator=(const Config& other);

	ServerConfig getServerConfig(const std::string& port, const std::string& server_name) const;
	class BracketException : public std::exception
	{
	private:
		std::string _str;
	public:
		BracketException(bool open, size_t line)
		{
			_str = (open) ? "Error: Unclosed '{' on line " : "Error: Extra '}' on line ";
			_str += line;
		}
		virtual ~BracketException() throw() { return ; }
		const char* what() const throw()
		{
			return _str.c_str();
		}
	};
	class UnexpectedToken : public std::exception
	{
		const char* what() const throw()
		{
			return "Error: Unexpected token after server";
		}
	};
private:
	void advance_to_next_bracket(std::ifstream& stream);
	int has_uneven_brackets(std::ifstream& stream);

	std::list<ServerConfig> _list;
};

#endif /* CONFIG_HPP */
