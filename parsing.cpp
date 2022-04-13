#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "header.hpp"
#include "request.hpp"
#include "utils.hpp"

std::string	parse(Request const &req)
{
	switch (req.get_method())
	{
		case GET:
			return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 3\n\nGET");
		default:
			return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nThis method is not yet handled");
	}
}
