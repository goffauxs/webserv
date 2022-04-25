#include <vector>
#include <string>
#include <iostream>
#include "header.hpp"
#include "request.hpp"
#include "webserv.hpp"

std::vector<std::string>    create_env(Request const &req/*, also the parse of the conf file*/)
{
	std::vector<std::string>        vec_env;
	std::vector<Header>::iterator   it;
	
	//SERVER_NAME is define in the http request with the key "Host"
	// it = find_header(headers, "Host");
	// if (it != headers.end())
	// {
	// 	vec_env.push_back("SERVER_NAME=" + it->get_value());
	// }

	//GATEWAY_INTERFACE is always CGI/1.1
	vec_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	
	//SERVER_PROTOCOL Always HTTP/1.1 but we can find it at the end of the first line
	// {
	// 	std::string	first_header(headers[0].get_value());
	// 	int			index = first_header.rfind(" ");

	// 	vec_env.push_back("SERVER_PROTOCOL=" + first_header.substr(index, first_header.size() - index));
	// }
	
	//SERVER_PORT TODO
	
	//REQUEST_METHOD at the begining of the first line -> think to check if is key or value
	
	//PATH_INFO in the first line, after the GET/POST, before the "?" or the second " "
	// {
	// 	std::string	first_header(headers[0].get_value());
	// 	int			end_l = std::min(first_header.find("?"), first_header.rfind(" "));
	// 	vec_env.push_back("PATH_INFO=" + first_header.substr(first_header.find(" "), first_header.size() - end_l));
	// }
	
	//PATH_TRANSLATED TODO
	
	//SCRIPT_NAME TODO
	
	//QUERY_STRING in the first line between the "?" and the last " "
	{
		std::string	first_header(req.get_resource());

		if (first_header.find("?") != (size_t)-1)
			vec_env.push_back("QUERY_STRING=" + first_header.substr(first_header.find("?") + 1));
	}
	
	//REMOTE_HOST
	
	//REMOTE_ADDR
	
	//CONTENT_TYPE Only for POST request, is define in the http request with the key "Content-Type"
	// it = find_header(headers, "Content-Type");
	// if (headers[0].get_key() == "POST" && it != headers.end())
	// {
	// 	vec_env.push_back("CONTENT_TYPE=" + it->get_value());
	// }

	//CONTENT_LENGTH TODO

	//HTTP_ACCEPT is define in the http request with the key "Accept"
	// it = find_header(headers, "Accept");
	// if (it != headers.end())
	// {
	// 	vec_env.push_back("HTTP_ACCEPT=" + it->get_value());
	// }

	//HTTP_ACCEPT_LANGUAGE is define in the http request with the key "Accept-Language"
	// it = find_header(headers, "Accept-Language");
	// if (it != headers.end())
	// {
	// 	vec_env.push_back("HTTP_ACCEPT_LANGUAGE=" + it->get_value());
	// }

	//HTTP_USER_AGENT is define in the http request with the key "User-Agent"
	// it = find_header(headers, "User-Agent");
	// if (it != headers.end())
	// {
	// 	vec_env.push_back("HTTP_USER_AGENT=" + it->get_value());
	// }

	//HTTP_REFERER is define in the http request with the key "Referer"
	// it = find_header(headers, "Referer");
	// if (it != headers.end())
	// {
	// 	vec_env.push_back("HTTP_REFERER=" + it->get_value());
	// }


	return (vec_env);
}

char	**vec_to_tab(std::vector<std::string> vec)
{
	char	**tab;

	tab = (char **)malloc(sizeof(char *) * vec.size() + 1);
	for (size_t i = 0; i < vec.size(); i++)
	{
		tab[i] =(char *) malloc(sizeof(char) * vec[i].size() + 1);
		strcpy(tab[i], vec[i].c_str());
	}
	tab[vec.size()] = NULL;
	return (tab);
}
