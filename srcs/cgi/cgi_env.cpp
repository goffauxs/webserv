#include "webserv.hpp"

std::vector<std::string>    create_env(Request const &req, const LocationConfig& conf)
{
	std::vector<std::string>							vec_env;
	std::map<std::string, std::string>					headers(req.get_headers());
	std::map<std::string, std::string>::const_iterator	it;


	//SERVER_NAME is define in the http request with the key "Host"
	it = headers.find("Host");
	if (it != headers.end())
		vec_env.push_back(std::string("SERVER_NAME=") + it->second);

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
	vec_env.push_back("SERVER_PORT=" + ::to_string(conf.getPort()));

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

	//PATH_TRANSLATED TODO .conf file parsed needed; the absolute path of the cgi
	{
		//Think to change the path when we will set pwd at the directory
		char pwd[PATH_MAX];
		getcwd(pwd, PATH_MAX);

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
	}

	//SCRIPT_NAME .conf file parsed needed : the path of the cgi script
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
	it = headers.find("Content-Type");
	if (req.get_method() == POST && it != headers.end())
		vec_env.push_back("CONTENT_TYPE=" + it->second);

	//CONTENT_LENGTH
	if (req.get_method() == POST)
		vec_env.push_back("CONTENT_LENGTH=" + to_string(req.get_content_length()));

	//HTTP_ACCEPT is define in the http request with the key "Accept"
	it = headers.find("Accept");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_ACCEPT=" + it->second);
	}

	//HTTP_ACCEPT_LANGUAGE is define in the http request with the key "Accept-Language"
	it = headers.find("Accept-Language");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_ACCEPT_LANGUAGE=" + it->second);
	}

	//HTTP_USER_AGENT is define in the http request with the key "User-Agent"
	it = headers.find("User-Agent");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_USER_AGENT=" + it->second);
	}

	//HTTP_REFERER is define in the http request with the key "Referer"
	it = headers.find("Referer");
	if (it != headers.end())
	{
		vec_env.push_back("HTTP_REFERER=" + it->second);
	}

	return (vec_env);
}

char	**vec_to_tab(std::vector<std::string> vec)
{
	char	**tab;

	tab = new char*[vec.size() + 1];
	for (size_t i = 0; i < vec.size(); i++)
	{
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
