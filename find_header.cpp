#include "header.hpp"
#include <vector>

//	return the iterator of the header with the given key
std::vector<Header>::iterator	find_header(std::vector<Header> vec, std::string key)
{
	std::vector<Header>::iterator	it = vec.begin(), end = vec.end();

	while (it != end)
	{
		if (it->get_key() == key)
			return (it);
		it++;
	}
	return (end);
}
