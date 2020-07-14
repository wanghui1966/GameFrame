#include "common.h"
#include "common_string.h"

#include <iostream>

int main()
{
	std::string str = "asd#ghjjjj#ssss";
	std::vector<std::string> vv = StringHelper::Split(str, '#');
	std::cout << str << ":";
	for (auto it = vv.begin(); it != vv.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	str = "sfdfdAADdee1351DD";
	std::string upper_str = StringHelper::ToUpper(str);
	std::string lower_str = StringHelper::ToLower(str);
	std::cout << str << ":" << upper_str << std::endl;
	std::cout << str << ":" << lower_str << std::endl;
	return 0;
}
