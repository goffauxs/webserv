#include "webserv.hpp"

std::string autoindex_gen(std::string path, std::string path2)
{
	DIR             *direc;
	struct dirent   *current;
	std::set<std::string>		files;		
	std::string		return_string("<!DOCTYPE html>\n<html>\n<head>\n\t<meta charset=\"utf-8\"/>\n\t<title>autoindex</title>\n</head>\n<body>\n\n<p>");

	std::cout << path << std::endl << path2 << std::endl;
	direc = opendir(path.c_str());
	if (!direc)
		return (std::string(""));
	current = readdir(direc);
	while(current)
	{
		files.insert(current->d_name);
		current = readdir(direc);
	}
	closedir(direc);
	for (std::set<std::string>::iterator it = files.begin(); it != files.end(); it++)
	{
		if (path2[path2.size() - 1] == '/')
			return_string.append("\n\t<a href=\"" + *it + "\">" + *it + "<br>");
		else
			return_string.append("\n\t<a href=\"." + path2 + ((path2[path2.length() - 1] == '/') ? "" : "/") + *it + "\">" + *it + "<br>");
	}
	return_string.append("\n</p>\n\n</body>\n</html>");
	return (return_string);
}
