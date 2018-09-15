#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <vector>

typedef std::string (*funct)(std::vector<std::string>);

std::string add(std::vector<std::string>)
{

	return "i+j";
}
std::string sub(std::vector<std::string>)
{
	return "i-j";
}

std::map<std::string, funct> functions;

std::string parseCommand(std::string s) 
{
	//tokenize s to vector of strings
	//functions[s[0]](restOfS);
}

int main(int argc, char *argv[]) {
	functions["add"] = add;
	functions["sub"] = sub;

	//functions["add"](1, 3);
	// usage:
	std::string s("add 1 2");
	std::cout << parseCommand(s) << std::endl;
}