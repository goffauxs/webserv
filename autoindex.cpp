#include "webserv.hpp"

std::string autoindex_gen(std::string path)
{
	DIR             *direc;
	struct dirent   *current;
	std::string		return_string("<!DOCTYPE html>\n<html>\n<body>\n\n<p>");

	direc = opendir(path.c_str());
	if (!direc)
		return (std::string(""));
	current = readdir(direc);
	while(current)
	{
		//std::cout << current->d_name << std::endl;
		return_string.append("\n\t<a href=\"" + path + "/" + current->d_name + "\">" + current->d_name + "<br>");
		current = readdir(direc);
	}
	return_string.append("\n</p>\n\n</body>\n</html>");
	return (return_string);
}

int main()
{
	std::cout << autoindex_gen(".") << std::endl;
}