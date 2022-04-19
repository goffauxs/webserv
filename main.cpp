#include <string>
#include <iostream>
#include <sstream>
#include "utils.hpp"

int main()
{
	std::string test = "server {\nallowed_methods GET POST PUT DELETE\nroot server/\n}\n";
	std::stringstream testStream(test);
	std::string out;

	char s[500];
	testStream.get(s, std::numeric_limits<std::streamsize>::max(), '}');
	std::cout << s << std::endl;
}
