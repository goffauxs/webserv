#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# include <unistd.h>
# include <string>
# include <vector>
# include <set>
# include <list>
# include <map>
# include <stack>
# include <algorithm>
# include <iostream>
# include <fstream>
# include <sstream>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/socket.h> //socket functions
# include <netinet/in.h> //internet protocol functions and structs
# include <dirent.h> //dir functions
# include <exception>

# include "config.hpp"
# include "server_config.hpp"
# include "location_config.hpp"
# include "request.hpp"
# include "utils.hpp"

# define BUFFSIZE 65536

# define HERE std::cout << "here" << std::endl;

// cgi_env.cpp
std::vector<std::string>    create_env(std::string path, Request const &req, const LocationConfig& conf);
char						**vec_to_tab(std::vector<std::string> vec);

// cgi.cpp
std::string	exec_cgi(std::string path, Request const &req, const LocationConfig& conf);

// autoindex.cpp
std::string	autoindex_gen(std::string path, std::string path2);

// mime_select.cpp
std::string mime_select(std::string ext);

// parsing.cpp
std::string	parse(Request const &req);

//run_server.cpp
void	run_serv(std::set<int> servers, Config& conf);

//setup_server.cpp
std::set<int>	setup_serv(int backlog, const Config& conf);

//utils.cpp
std::string					get_error_response(Request const &req, size_t error_code);
int							check(int val, std::string msg);
std::vector<char>::iterator	vector_find(std::vector<char>& vector, const char* str);

#endif /* WEBSERV_HPP */
