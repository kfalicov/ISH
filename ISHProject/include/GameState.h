#include <vector>
#include <string>
#include <unordered_map>
#include <SDL.h>
#include <functional>

//forward declarations (for all pointer types used in method signatures)
class GameState;
class AssetHandler;
struct SDL_Surface;
class Environment;
class Camera;
class Entity;

class GameState
{
public:
	GameState(AssetHandler* assetHandler) {
		//TODO each state can have a different sized surface. For example, PlayState will have
		//a square surface to render the environment.
		initializeRenderSurface(800, 600);
		this->assetHandler = assetHandler;
		this->previous = nullptr;
		//when it is created it needs to have its surface rendered at least once
		this->needsRender = true;
	}
	virtual GameState* Update(SDL_Event event) { return this; };
	virtual SDL_Surface* Render(float interpolation, bool forceReRender = false);
	//modifies the surface to 
	virtual void WriteSurface(float interpolation) = 0;
	void setPrevious(GameState* previous) {
		this->previous = previous;
	}
	GameState* getPrevious() {
		return previous;
	}
	virtual ~GameState() {
		SDL_FreeSurface(surface);
		std::cout << "deleted gamestate" << std::endl;
		if(previous)
			delete previous;
	}

protected:
	//the last gamestate you were in, for switching purposes
	GameState* previous;

	//AssetHandler generates sprites and reads from a file. AssetHandler needs to be given to the various generators
	AssetHandler* assetHandler;

	//an internal storage so that rerendering doesn't have to happen constantly.
	//Contains a snapshot of exactly what the surface and its subsurfaces looked like
	//when Render() was last called.
	SDL_Surface* surface;
	// whether the surface has been updated visually and needs to be re-written to a surface
	bool needsRender;

	//sets up the render surface
	void initializeRenderSurface(int width, int height) {
		surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
	}
};

class MenuState : public GameState
{
public:
	MenuState(AssetHandler* assetHandler, GameState* previous = nullptr);
	void WriteSurface(float interpolation) override;
	~MenuState();

	//void HandleEvents(SDL_Event event) override;
	GameState* Update(SDL_Event event) override;
private:
	//std::vector<Element*> menuitems;
};

class PlayState : public GameState
{
public:
	PlayState(AssetHandler* assetHandler, GameState* previous = nullptr);
	void WriteSurface(float interpolation) override;
	~PlayState();

	//void HandleEvents(SDL_Event event) override;
	GameState* Update(SDL_Event event) override;
private:
	Environment* environment;
	Camera* environmentCamera;

	SDL_Surface* environmentSurface;
	SDL_Surface* uiSurface;

	Entity* player;

	//renders the environment to the EnvironmentSurface
	void RenderEnvironment();
	//renders the entities to the 
	void RenderEntities(float interpolation);
	//render a single entity (helper function)
	void RenderEntity(Entity* e, float interpolation);
	//renders the UI to the UISurface
	void RenderUI();
};

class ConsoleState : public GameState
{
public:
	ConsoleState(AssetHandler* assetHandler, GameState* previous);
	void WriteSurface(float interpolation) override;
	~ConsoleState();

	//void HandleEvents(SDL_Event event) override;
	GameState* Update(SDL_Event event) override;
	void parseCommand(std::string command);
	std::vector<std::string> split(const std::string & s, char delimiter);
private:
	std::unordered_map<std::string, void(ConsoleState::*)()> functions;
	std::string currentCommand;
	int commandIndex = 0;
	std::vector<std::string> consoleOutput;
	std::vector<std::string> commands;
	bool canType;

//console functions
private:
	//the arguments are stored here when a command is entered.
	//it can be used in any of the functions below, but make sure to check
	//for argument length.
	std::vector<std::string> args;
	void hello() { 
		consoleOutput.push_back("hello there");
		std::cout << "hello" << std::endl; 
	}
	void changeState() { 
		if (args.size() > 0) {
			if (args[0] == "menu") {
				delete previous;
				previous = new MenuState(assetHandler);
				consoleOutput.push_back("Switched to Menu");
			}
			else if (args[0] == "play") {
				delete previous;
				previous = new PlayState(assetHandler);
				consoleOutput.push_back("Switched to Play");
			}
		}
	}
	void toggleTurnBased() {
		//TODO
	}
	void displayHelp() {
		//TODO
	}
};