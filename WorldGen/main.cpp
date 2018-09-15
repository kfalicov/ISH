#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>

// Function to 'point' to
const void foo() {}
//...
// Create the 'bar' type for functions with the signature 'const void ...()'
typedef const void(*bar)();
//...
// Store a pointer to the foo() function
bar ptr = &foo;

//typedef void(*ScriptFunction)(void); // function pointer type
//typedef std::unordered_map<std::string, bar> script_map;

std::string add(int i, int j) {	return std::to_string(i + j); }
std::string sub(int i, int j) { return std::to_string(i - j); }

typedef void* (*fooFunc)(void*, void*);

int main(int argc, char *argv[]) {
	std::map<std::string, bar> functions;
	functions["add"] = add;
	functions["sub"] = &sub;

	// usage:
	//std::string s("add");
	//std::string res = functions[s](1, 2);
	
	//while (true);
}