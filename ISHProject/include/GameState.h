#include <vector>
#include <SDL.h>

//forward declarations (for all pointer types used in method signatures)
class GameState;
class AssetHandler;
struct SDL_Surface;

class GameState
{
public:
	//virtual void HandleEvents(SDL_Event event) {}; //should be unneeded
	virtual GameState* Update(SDL_Event event) { return this; };
	virtual void Render(std::vector<SDL_Surface*> &surfaces, float interpolation, bool forceReRender = false);
	virtual SDL_Surface* RenderLayers(float interpolation) { return nullptr; };
	void initializeRenderSurface(int width, int height) {
		surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
	};
	
	GameState(AssetHandler* assetHandler);
	~GameState() {};

protected:
	//the last gamestate you were in, for switching purposes
	GameState* previous;

	//AssetHandler generates sprites and reads from a file. AssetHandler needs to be given to the various generators
	AssetHandler* assetHandler;

	//an internal storage so that rerendering doesn't have to happen constantly
	//This surface only contains the layers that belong to this state.
	SDL_Surface* surface;
	// whether the surface has been updated visually and needs to be re-written to a surface
	bool needsRender;
	
};

class MenuState : public GameState
{
public:
	MenuState(AssetHandler* assetHandler, GameState* previous = nullptr);
	SDL_Surface* RenderLayers(float interpolation) override;
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
	SDL_Surface* RenderLayers(float interpolation) override;
	~PlayState();

	//void HandleEvents(SDL_Event event) override;
	GameState* Update(SDL_Event event) override;
private:
};