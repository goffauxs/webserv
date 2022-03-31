#include <string>
#include <iostream>
#include <sstream>
#include "utils.hpp"

int main()
{
	//std::string request = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nConnection: keep-alive\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"macOS\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.109 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en-GB;q=0.9,en;q=0.8,fr-BE;q=0.7,fr;q=0.6,ko-KR;q=0.5,ko;q=0.4,ja-JP;q=0.3,ja;q=0.2";
	std::string request = "POST /test/demo_form.php HTTP/1.1\r\nHost: w3schools.com\r\n\r\nname1=value1&name2=value2";

	std::stringstream requestStream(request);
	std::string request_line;
	std::getline(requestStream, request_line);
	trim(request_line);

	std::vector<std::string> segments = split(request_line);
	const Method method = method_from_string(segments[0]);
	const std::string resource = segments[1];
	const Version version = version_from_string(segments[2]);

	std::cout << "request_line: '" << request_line << "'" << std::endl;
	std::cout << "method: " << to_string(method) << " resource: " << resource << " version: " << to_string(version) << std::endl;

	while (std::getline(requestStream, request_line) && request_line != "\r")
	{
		trim(request_line);
		std::cout << "[" << request_line << "]" << std::endl;
	}
}
