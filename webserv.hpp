#pragma once

#include <string>
#include <vector>
#include "request.hpp"
#include <sys/types.h>
#include <dirent.h>
#include "location_config.hpp"

//just a c style .h file for prototypes

std::string	parse(Request const &req);

std::string exec_cgi(std::string path, Request const &req, LocationConfig conf);
char	**vec_to_tab(std::vector<std::string> vec);
void	free_tab(char **tab);
std::vector<std::string>    create_env(Request const &req, LocationConfig conf);
// std::vector<Header>::const_iterator	find_header(const std::vector<Header> &vec, std::string key);
std::string autoindex_gen(std::string path, std::string path2);
std::string mime_select(std::string ext);
