#pragma once
#include "GameState.h"
#include "Game.h"
#include <string>
#include <unordered_map>
#include <SDL_ttf.h>

using std::string;

typedef int(*ConsoleCallbackFunction)(std::vector<string>, std::string&);

class Console : public GameState
{
public:
	static Console* Instance(static GameState* state);

	void Init() override;
	void Clean() override;

	void HandleEvents(SDL_Event event) override;
	void Update() override;
	void Render(float interpolation) override;

	void ParseCommand(string c);
	std::vector<std::string> split(const std::string& s, char delimiter);

	const char* getName() override { return "Console"; }

	~Console();

protected:
	static Console* instance;
	GameState* oldState;

	TTF_Font* consoleFont;
	SDL_Color fontColor = { 255, 255, 255 };

	string currentCommand;
	int commandIndex = 0;
	vector<string> consoleOutput;
	vector<string> commands;

	std::unordered_map<std::string, ConsoleCallbackFunction> functions;

	Console();
};