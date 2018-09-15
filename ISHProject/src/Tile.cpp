#include "Tile.h"

vec2 Tile::worldPos()
{
	return pos + (16 * chunk); //chunk no. * chunk width/height, plus offset within chunk
}