#pragma once
#include "Map.h"

class Game : public GameInterface
{
public:
	Game() : state(0) { setGame(this); };
	~Game() {};

	void init();
	void begin();
	void end();
	void moveMonsters();
	void moveProjectiles();
	void showHealth(Actor*, std::string name, int x, int y);

	void setWin() { state = WIN; };
	void setLose() { state = LOSE; };
	void win();
	void lose();
	void askRestart();

	Map map;
	Actor* player;
	int state;
};