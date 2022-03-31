#include "utils.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define PORT 8080

class Header
{
public:
	Header(const std::string& key, const std::string& value) : _key(key), _value(value) {}
	Header(const std::string& header)
	{
		std::stringstream headerStream(header);
		std::getline(headerStream, _key, ':');
		std::getline(headerStream, _value);
		trim(_value);
	}

	const std::string& get_value() const { return this->_value; }
	const std::string& get_key() const { return this->_key; }
	void set_value(const std::string& value) { this->_value = value; }
	void set_key(const std::string& key) { this->_key = key; }
	std::string serialize() const { return this->_key + ": " + this->_value + LINE_END; }
private:
	std::string _key;
	std::string _value;
};

class Request
{
public:
	Request(Method method, const std::string& resource, const std::vector<Header>& headers, Version version = Version::HTTP_1_1)
		: _version(version), _method(method), _resource(resource), _headers(headers) {}
	std::string serialize() const
	{
		std::string request;
		request = to_string(this->_method) + " " + this->_resource + " " + to_string(this->_version) + LINE_END;
		for (const Header& header : this->_headers)
		{
			request += header.serialize();
		}
		request += LINE_END;
		return request;
	}
private:
	Version _version;
	Method _method;
	std::string _resource;
	std::vector<Header> _headers;
};

void handle_command(int socket, char *request, size_t bufsize)
{
	(void)bufsize;
	std::string hello = "HTTP/1.1 404 Page Not Found";
	std::string buf(request);
	size_t pos = 0;
	std::string method = buf.substr(0, pos = buf.find(' '));
	buf.erase(0, pos + 1);
	std::string target = buf.substr(0, pos = buf.find(' '));
	buf.erase(0, pos + 1);
	std::string protocol = buf.substr(0, buf.find('\n'));
	
	// if (protocol.find(' ') != std::string::npos)
	//     error 400 (Bad Request)
	// if (target == "/favicon.ico")
	// {
	// 	std::ifstream ifs("42_Logo.png", std::ifstream::binary);
	// 	if (ifs)
	// 	{
	// 		std::string header = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: "
	// 		while (ifs.tellg() != -1)
	// 		{
	// 			char *p = new char[1024];
	// 			bzero(p, 1024);
	// 			ifs.read(p, 1024);
	// 			if (send(socket, p, 1024, 0) < 0)
	// 				std::cerr << "err" << std::endl;
	// 		}
	// 	}
	// }
	std::cout << std::endl << request << std::endl;
	write(socket, hello.c_str(), hello.length());
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		
		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000);
		handle_command(new_socket, buffer, 30000);
		printf("------------------Hello message sent-------------------\n");
		close(new_socket);
	}
	return 0;
}
