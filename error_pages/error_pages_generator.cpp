#include <iostream>
#include <string>
#include <map>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

std::map<std::string, std::string>	create_map()
{
	std::map<std::string, std::string>	mp;

	mp.insert(std::pair<std::string, std::string>("505", "HTTP Version Not Supported "));
	mp.insert(std::pair<std::string, std::string>("504", "Gateway Time-Out"));
	mp.insert(std::pair<std::string, std::string>("503", "Out of Resources"));
	mp.insert(std::pair<std::string, std::string>("502", "Bad Gateway"));
	mp.insert(std::pair<std::string, std::string>("501", "Not Implemented"));
	mp.insert(std::pair<std::string, std::string>("500", "Server Error"));
	mp.insert(std::pair<std::string, std::string>("415", "Unsupported Media Type"));
	mp.insert(std::pair<std::string, std::string>("414", "Request-URL Too Large"));
	mp.insert(std::pair<std::string, std::string>("413", "Request Entity Too Large"));
	mp.insert(std::pair<std::string, std::string>("412", "Precondition Failed"));
	mp.insert(std::pair<std::string, std::string>("411", "Length Required"));
	mp.insert(std::pair<std::string, std::string>("410", "Gone"));
	mp.insert(std::pair<std::string, std::string>("409", "Conflict"));
	mp.insert(std::pair<std::string, std::string>("408", "Request Time-Out"));
	mp.insert(std::pair<std::string, std::string>("407", "Proxy Authentication Required"));
	mp.insert(std::pair<std::string, std::string>("406", "Not Acceptable"));
	mp.insert(std::pair<std::string, std::string>("405", "Method Not Allowed "));
	mp.insert(std::pair<std::string, std::string>("404", "Not Found"));
	mp.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	mp.insert(std::pair<std::string, std::string>("402", "Payment Required "));
	mp.insert(std::pair<std::string, std::string>("401", "Unauthorized"));
	mp.insert(std::pair<std::string, std::string>("400", "Bad Request"));
	return (mp);
}

int main()
{
	std::map<std::string, std::string> mp = create_map();

	for (std::map<std::string, std::string>::iterator it = mp.begin(); it != mp.end(); it++)
	{
		int fd = open((it->first + ".html").c_str(), O_CREAT | O_WRONLY, S_IRUSR + S_IWUSR + S_IRGRP + S_IWGRP + S_IROTH);
		std::string file = "<!DOCTYPE html>\n<head>\n<title>" + it->second + "</title>\n</head>\n<body style=\"background-color:#ff9090\">\n<div id=\"main\">\n<div class=\"fof\">\n<h1 style=\"text-align:center\">Error " + it->first +"</h1>\n<h2 style=\"text-align:center\">" + it->second + "</h2>\n</div>\n</div>\n</body>\n</html>\n";
		write(fd, file.c_str(), file.length());
		close(fd);
	}
	return (0);
}
