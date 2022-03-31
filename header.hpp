#ifndef HEADER_HPP
#define HEADER_HPP
#include "utils.hpp"

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
	Header() {}
	Header(const Header&) {}
	Header& operator==(const Header&) { return *this; }

	std::string _key;
	std::string _value;
};

#endif /* HEADER_HPP */
