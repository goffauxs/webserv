#include <string>
#include <iostream>
#include <sstream>

bool is_not_space(char c) { return !std::isspace(c); }
void ltrim(std::string& str) { str.erase(str.begin(), std::find_if(str.begin(), str.end(), is_not_space)); }
void rtrim(std::string& str) { str.erase(std::find_if(str.rbegin(), str.rend(), is_not_space).base(), str.end()); }
void trim(std::string& str) { ltrim(str); rtrim(str); }

int main()
{
	std::string header = "Host:         localhost:8080";

	std::stringstream headerStream(header);
	std::string key;
	std::string value;
	std::getline(headerStream, key, ':');
	std::getline(headerStream, value);
	trim(value);

	std::cout << "key: " << key << " value: " << value << std::endl;
}
