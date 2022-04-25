#pragma once

#include <string>
#include <vector>
#include "request.hpp"

//just a c style .h file for prototypes

std::string	parse(Request const &req);

std::string exec_cgi(std::string path, Request const &req);
char	**vec_to_tab(std::vector<std::string> vec);
std::vector<std::string>    create_env(Request const &req/*, also the parse of the conf file*/);
