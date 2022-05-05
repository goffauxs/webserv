#include <vector>
#include <string>
#include <iostream>
#include "header.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <string.h>
#include <unistd.h>
#include "location_config.hpp"

std::vector<std::string>    create_env(Request const &req, LocationConfig conf)
{
	std::vector<std::string>			vec_env;
	std::vector<Header>					headers(req.get_headers());
	std::vector<Header>::const_iterator	it;


	//SERVER_NAME is define in the http request with the key "Host"
	it = find_header(headers, "Host");
	if (it != headers.end())
		vec_env.push_back(std::string("SERVER_NAME=") + it->get_value());

	//GATEWAY_INTERFACE is always CGI/1.1
	vec_env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	//SERVER_PROTOCOL Always HTTP/1.1 but we can find it at the end of the first line	
	switch (req.get_version())
	{
		case HTTP_1_0:
			vec_env.push_back("SERVER_PROTOCOL=HTTP/1.0");
			break;
		case HTTP_1_1:
			vec_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
			break;
		case HTTP_2_0:
			vec_env.push_back("SERVER_PROTOCOL=HTTP/2.0");
			break;
		default:
			break;
	}

	//SERVER_PORT .conf file parsed needed : 8080, 80 etc..
	vec_env.push_back("SERVER_PORT=" + conf.getPort());

	//REQUEST_METHOD at the begining of the first line
	switch (req.get_method())
	{
		case POST:
			vec_env.push_back("REQUEST_METHOD=POST");
			break;
		case GET:
			vec_env.push_back("REQUEST_METHOD=GET");
			break;
		case DELETE:
			vec_env.push_back("REQUEST_METHOD=DELETE");
			break;
		default:
			break;
	}
	
	//PATH_INFO the info int url after the location define in the .conf before the "?" if there's one
	//Obsolete but it did it anyway 
	{
		std::string	resource(req.get_resource());
		size_t		q = resource.find('?') - 1;
		
		if (q > resource.size())
	 		vec_env.push_back("PATH_INFO=" + resource.substr(resource.find_first_not_of(conf.getPath(), resource.find(conf.getPath()))));
		else
	 		vec_env.push_back("PATH_INFO=" + resource.substr(resource.find_first_not_of(conf.getPath(), resource.find(conf.getPath())), q));

	}

	//PATH_TRANSLATED TODO .conf file parsed needed; the absolute path of the cgi
	{
		//Think to change the path when we will set pwd at the directory
		char *pwd = get_current_dir_name();
		
		switch (req.get_method())
		{
			case GET:
				vec_env.push_back(std::string("PATH_TRANSLATED=") + pwd + "/server/cgi-bin/test.py");
				break;
			case POST:
				vec_env.push_back(std::string("PATH_TRANSLATED=") + pwd + "/server/cgi-bin/upload.py");
				break;
			default:
				break;
		}
		free(pwd);
		std::cout << vec_env[vec_env.size() - 1] << std::endl;
	}
	
	//SCRIPT_NAME TODO .conf file parsed needed : the path of the cgi script
	{
		switch (req.get_method())
		{
			case GET:
				vec_env.push_back("SCRIPT_NAME=test.py");
				break;
			case POST:
				vec_env.push_back("SCRIPT_NAME=upload.py");
				break;
			default:
				break;
		}
	}

	//QUERY_STRING in the first line between the "?" and the last " "
	{
		std::string	first_header(req.get_resource());

		if (first_header.find("?") != (size_t)-1)
			vec_env.push_back("QUERY_STRING=" + first_header.substr(first_header.find("?") + 1));
	}
	
	//REMOTE_HOST
	
	//REMOTE_ADDR
	//CONTENT_TYPE Only for POST request, is define in the http request with the key "Content-Type"
	it = find_header(headers, "Content-Type");
	if (headers[0].get_key() == "POST" && it != headers.end())
	{
		vec_env.push_back("CONTENT_TYPE=" + it->get_value());
	}

	//CONTENT_LENGTH
	if (headers[0].get_key() == "POST")
	{
		vec_env.push_back("CONTENT_LENGTH=" + to_string(req.get_contentLength()));
	}

	//HTTP_ACCEPT is define in the http request with the key "Accept"
	it = find_header(headers, "Accept");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_ACCEPT=" + it->get_value());
	}

	//HTTP_ACCEPT_LANGUAGE is define in the http request with the key "Accept-Language"
	it = find_header(headers, "Accept-Language");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_ACCEPT_LANGUAGE=" + it->get_value());
	}

	//HTTP_USER_AGENT is define in the http request with the key "User-Agent"
	it = find_header(headers, "User-Agent");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_USER_AGENT=" + it->get_value());
	}

	//HTTP_REFERER is define in the http request with the key "Referer"
	it = find_header(headers, "Referer");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_REFERER=" + it->get_value());
	}

	return (vec_env);
}

char	**vec_to_tab(std::vector<std::string> vec)
{
	char	**tab;

	//std::cerr << "test" << std::endl;
	//tab = (char **)malloc(sizeof(char *) * vec.size() + 1);
	tab = new char*[vec.size() + 1];
	for (size_t i = 0; i < vec.size(); i++)
	{
		//tab[i] =(char *) malloc(sizeof(char) * vec[i].size() + 1);
		tab[i] = new char[vec[i].size() + 1];
		strcpy(tab[i], vec[i].c_str());
	}
	tab[vec.size()] = NULL;
	return (tab);
}

void	free_tab(char **tab)
{
	size_t	i = 0;

	while(tab[i])
	{
		delete [] tab[i];
		i++;
	}
	delete [] tab;
}
