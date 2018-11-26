#include <vector>
#include <unordered_map>

//forward declarations (for all pointer types used in method signatures)
class Entity;
class Sprite;
struct SDL_Surface;

class AssetHandler {
public:
	AssetHandler();
	~AssetHandler();

	void Clean();
	bool Update();

	Sprite* GetSprite(const char* spriteSheet, int spriteIndex);

	//Progresses each sprite to the next frame of their animation.
	void UpdateSpriteFrames(std::vector<Sprite*> sprites);
	//Changes the sprite that an entity is displaying.
	void UpdateEntityAnimations(std::vector<Entity*> entities);

private:
	std::unordered_map<const char*, SDL_Surface*> loadedSpriteSheets;
	int updates_per_frame;
	int animationCounter;
};