#include "custom_arp.h"
#include <Windows.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>
#include <iterator>
std::string remove_spaces(std::string& str)
{
	str.erase(
		std::unique_copy(str.begin(), str.end(), str.begin(),
			[](char c1, char c2) { return c1 == ' ' && c2 == ' '; }),
		str.end());
	str.erase(str.begin());
	return str;
}
std::string exec(const char* cmd) 
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose) > pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	int string_iter = 0;
	for (auto i = result.end(); i > result.begin(), string_iter != 15; i--, string_iter++)
		result.erase(i);
	return remove_spaces(result);
}
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	if (argc > 3)
	{
		std::cout << "Invalid input\n";
		return 0;
	}
	custom_arp arp;
	if(!std::strcmp(argv[1], "/ip"))
		arp.entry_point(argv[2]);
	if (!std::strcmp(argv[1], "/mac")) //cc-1a-fa-87-c1-12
	{
		std::string res = "arp -a | find /i ";
		res += "\""; 
		res += argv[2];
		res += "\"";
		std::cout << exec(res.c_str());
	}
	return 0;
}