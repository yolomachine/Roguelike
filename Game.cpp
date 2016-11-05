#pragma once
#include "Game.h"

void Game::init() {
	map.read();
	map.render();

	state = 0;
	player = static_cast<Actor*>(map.tiles[map.cursor.y][map.cursor.x]);
	map.cursor_available = true;
	srand(time(0));
}

void Game::begin() {
	int key;
	for (;map.cursor_available;) {
		key = getch();
		if (key == 'q' || key == 1081) break;
		if (key == KEY_LEFT) player->moveLeft();
		if (key == KEY_RIGHT) player->moveRight();
		if (key == KEY_UP) player->moveUp();
		if (key == KEY_DOWN) player->moveDown();
		if (key == KEY_SLEFT) player->shootLeft();
		if (key == KEY_SRIGHT) player->shootRight();
		if (key == KEY_SUP) player->shootUp();
		if (key == KEY_SDOWN) player->shootDown();
		moveMonsters();
		moveProjectiles();
	}
	switch (state) {
		case WIN: win(); break;
		case LOSE: lose(); break;
		default: break;
	}
	askRestart();
	if ((key = getch()) == 'r') {
		end();
		init();
		begin();
	}
}

void Game::end() {
	while (map.actors.size()) map.actors.pop_back();
	while (map.projectiles.size()) map.projectiles.pop_back();
	for (size_t i = map.tiles.size() - 1; i > 0; --i) {
		for (size_t j = 0; j < map.tiles[i].size(); ++j)
			delete map.tiles[i][j];
		map.tiles.pop_back();
	}
}

void Game::moveMonsters() {
	for (int i = map.actors.size() - 1; i >= 0; --i) {
		if (!map.cursor_available) return;
		if (map.actors[i]->type == Type::PROJECTILE) {
			Projectile* p = static_cast<Projectile*>(map.actors[i]);
			p->move(p->x() + p->dir_x, p->y() + p->dir_y);
			continue;
		}
		if (abs(map.actors[i]->x() - player->x()) > 20 || 
			abs(map.actors[i]->y() - player->y()) > 20 ||
			rand() % 10 > 6) 
			continue;
		if (abs(map.actors[i]->x() - player->x()) > abs(map.actors[i]->y() - player->y())) {
			if (map.actors[i]->x() < player->x()) rand() % 5 > 3 ? map.actors[i]->shootRight() : map.actors[i]->moveRight();
			else if (map.actors[i]->x() > player->x()) rand() % 5 > 3 ? map.actors[i]->shootLeft() : map.actors[i]->moveLeft();
		}
		else {
			if (map.actors[i]->y() < player->y()) rand() % 5 > 3 ? map.actors[i]->shootDown() : map.actors[i]->moveDown();
			else if (map.actors[i]->y() > player->y()) rand() % 5 > 3 ? map.actors[i]->shootUp() : map.actors[i]->moveUp();
		}
	}
}

void Game::moveProjectiles() {
	int i = 0, size = map.projectiles.size();
	while (i < size) {
		if (!map.cursor_available) return;
		Projectile* p = static_cast<Projectile*>(map.projectiles[i]);
		p->move(p->x() + p->dir_x, p->y() + p->dir_y);
		if (size > map.projectiles.size())
			size = map.projectiles.size();
		else ++i;
	}
}

void Game::showHealth(Actor* actor, std::string name, int x, int y) {
	std::string health = name + " health: " + std::to_string(actor->health());
	mvprintw(y, x, ""); clrtoeol();
	mvprintw(y, x, health.c_str());
}

void Game::win() {
	clear();
	mvprintw(map.height / 2, map.width / 2 - 4, "Is there really a princess?");
}

void Game::lose() {
	clear();
	mvprintw(map.height / 2, map.width / 2 - 5, "Have you ever become a zombie?");
}

void Game::askRestart() {
	mvprintw(2 + map.height / 2, map.width / 2, "Press R to restart");
	mvprintw(3 + map.height / 2, map.width / 2 - 2, "Press anything to quit");
}