#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stack>
#include "utils.hpp"
#include "location_config.hpp"

int has_uneven_brackets(std::ifstream& stream)
{
	std::stack<int> brackets;

	int line_counter = 1;
	std::string word;
	while (!stream.eof())
	{
		stream >> word;
		if (word == "{") // TODO find a solution to check for multiple brackets following eachother
			brackets.push(line_counter);
		else if (word == "}")
		{
			std::cout << "brackets.size(): " << brackets.size() << std::endl;
			if (brackets.size())
				brackets.pop();
			else
			{
				std::cerr << "Extra '}' on line " << line_counter << std::endl;
				return 1;
			}
		}
		if (stream.peek() == 10)
			line_counter++;
	}
	if (brackets.empty())
		return 0;
	else
	{
		std::cerr << "Unclosed '{' on line " << line_counter << std::endl;
		return 1;
	}
}

int main()
{
	std::ifstream ifs("default.conf", std::ifstream::in);

	if (ifs.good())
	{
		if (has_uneven_brackets(ifs))
			return 1;
	}
	return 0;
}
