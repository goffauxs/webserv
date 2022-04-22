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
	char curr_char;
	while (!stream.eof())
	{
		stream.get(curr_char);
		if (curr_char == '{')
			brackets.push(line_counter);
		else if (curr_char == '}')
		{
			if (brackets.size())
				brackets.pop();
			else
			{
				std::cerr << "Error: Extra '}' on line " << line_counter << std::endl;
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
		std::cerr << "Error: Unclosed '{' on line " << brackets.top() << std::endl;
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
		ifs.seekg(0, ifs.beg);
		std::string server_str;
		
	}
	return 0;
}
