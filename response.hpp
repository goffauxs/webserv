#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "utils.hpp"
# include "header.hpp"
# include <map>

class Response
{
private:
	int _response_code;
	Version _version;
	std::map<std::string, std::vector<Header> > _headers;
	std::string _body;
public:
	const static int OK = 200;
	const static int CREATED = 201;
	const static int ACCEPTED = 202;
	const static int NO_CONTENT = 203;
	const static int BAD_REQUEST = 400;
	const static int FORBIDDEN = 403;
	const static int NOT_FOUND = 404;
	const static int REQUEST_TIMEOUT = 408;
	const static int INTERNAL_SERVER_ERROR = 500;
	const static int BAD_GATEWAY = 502;
	const static int SERVICE_UNAVAILABLE = 503;

	Response(int response_code, Version version, const std::map<std::string, std::vector<Header> >& headers, const std::string& body)
		: _response_code(response_code), _headers(headers), _body(body) {}
	
	Response(const std::string& response)
	{
		
	}
	
	int get_response_code() const { return this->_response_code; }
	const std::string& get_body() const { return this->_body; }
	const std::map<std::string, std::vector<Header> > get_headers() const { return this->_headers; }
};

#endif /* RESPONSE_HPP */
