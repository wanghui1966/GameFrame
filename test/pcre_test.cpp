#include <iostream>
#include <string>

#if __GNUC__ <4
	#include <pcre/pcre.h>
#else
	#include <pcre.h>
#endif

bool match(pcre *regular_reg, const std::string &str)
{
	return (pcre_exec(regular_reg, nullptr, str.c_str(), str.size(), 0, 0, nullptr, 0) == 0); 
}

int main()
{
	std::string regular_str = "^[1-9][2-6].*[4-5][6-9]$";
	std::cout << "regular_str=" << regular_str << std::endl;
	const char *error_msg = nullptr;
	int erroffset = 0;
	pcre *regular_reg = pcre_compile(regular_str.c_str(), PCRE_UTF8, &error_msg, &erroffset, 0);
	if (regular_reg)
	{
		std::string str = "13fte4e47";
		std::cout << "str=" << str << ", match=" << match(regular_reg, str) << std::endl;

		str = "1246";
		std::cout << "str=" << str << ", match=" << match(regular_reg, str) << std::endl;

		str = "03fte4e47";
		std::cout << "str=" << str << ", match=" << match(regular_reg, str) << std::endl;

		str = "13fte4e45";
		std::cout << "str=" << str << ", match=" << match(regular_reg, str) << std::endl;

		pcre_free(regular_reg);
		regular_reg = nullptr;
	}
	else
	{
		std::cout << error_msg << std::endl;
	}

	return 0;
}
