#pragma once
#include "GameObjects.h"

typedef struct Position {
	int x;
	int y;
	Position() : x(0), y(0) {};
	Position(int x, int y) : x(x), y(y) {};
} Vector;

class Map : public MapInterface
{
public:
	Map() : width(0), height(0), cursor(), cursor_available(false) { setMap(this); };
	~Map() {};

	void read();
	void render();
	void render(Tile*);
	void swap(Tile*, Tile*);
	Tile* getObject(int x, int y);
	void removeObject(int x, int y);
	void setNewProjectile(Projectile*);

	int width;
	int height;
	Position cursor;
	bool cursor_available;
	std::vector<std::vector<Tile*>> tiles;
	std::vector<Actor*> actors;
	std::vector<Actor*> projectiles;
};