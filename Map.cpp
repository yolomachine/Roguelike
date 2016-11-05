#pragma once
#include "Map.h"

void Map::read() {
	std::ifstream fin("level.txt");
	std::vector<std::string> scheme;
	std::string line;
	while (getline(fin, line))
		scheme.push_back(line);
	fin.close();

	height = scheme.size();
	width = scheme.front().size();

	tiles.resize(height);
	for (size_t i = 0; i < tiles.size(); ++i) {
		tiles[i] = std::vector<Tile*>(width);
		for (size_t j = 0; j < tiles[i].size(); ++j)
			switch (scheme[i][j]) {
				case WALL_SYMBOL:     tiles[i][j] = new Wall(j, i); break;
				case FLOOR_SYMBOL:    tiles[i][j] = new Floor(j, i); break;
				case PRINCESS_SYMBOL: tiles[i][j] = new Princess(j, i); break;
				case KNIGHT_SYMBOL:   tiles[i][j] = new Player(j, i); cursor.y = i; cursor.x = j; break;
				case ZOMBIE_SYMBOL:   tiles[i][j] = new Zombie(j, i); actors.push_back(static_cast<Actor*>(tiles[i][j])); break;
				case DRAGON_SYMBOL:   tiles[i][j] = new Dragon(j, i); actors.push_back(static_cast<Actor*>(tiles[i][j])); break;
				case HEALING_POTION_SYMBOL: tiles[i][j] = new HealingPotion(j, i);
		}
	}
}

void Map::swap(Tile* t1, Tile* t2) {
	int dest_x = t2->x();
	int dest_y = t2->y();
	tiles[t1->y()][t1->x()] = t2;
	tiles[t2->y()][t2->x()] = t1;
	t2->x(t1->x());
	t2->y(t1->y());
	t1->x(dest_x);
	t1->y(dest_y);
	if (t1->type == Type::PLAYER) { cursor.x = t1->x(); cursor.y = t1->y(); };
	if (t2->type == Type::PLAYER) { cursor.x = t2->x(); cursor.y = t2->y(); };
	render(t1);
	render(t2);
	refresh();
}

void Map::render() {
	initscr();
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);
	resize_term(height + 10 + height % 2, width + 20 + !(width % 2));
	start_color();

	for (size_t i = 0; i < tiles.size(); ++i)
		for (size_t j = 0; j < tiles[i].size(); ++j)
			render(tiles[i][j]);

	refresh();
}

void Map::render(Tile* t) {
	init_pair(t->symbol(), t->foreground(), t->background());
	attron(COLOR_PAIR(t->symbol()));
	mvaddch(t->y() + 5, t->x() + 10, t->symbol());
	attroff(COLOR_PAIR(t->symbol()));
}

Tile* Map::getObject(int x, int y) {
	return tiles[y][x];
}

void Map::removeObject(int x, int y) {
	if (tiles[y][x]->type == Type::MONSTER) {
		for (size_t i = 0; i < actors.size(); ++i)
			if (tiles[y][x] == actors[i])
				actors.erase(actors.begin() + i);
		mvprintw(1, 0, ""); clrtoeol();
		if (rand() % 10 > 7) {
			tiles[y][x] = new HealingPotion(x, y);
			render(tiles[y][x]);
			refresh();
			return;
		}
	}
	else if (tiles[y][x]->type == Type::PROJECTILE) {
		for (size_t i = 0; i < projectiles.size(); ++i)
			if (tiles[y][x] == projectiles[i])
				projectiles.erase(projectiles.begin() + i);
	}
	else if (tiles[y][x]->type == Type::PLAYER) {
		cursor_available = false;
		return;
	}
	delete tiles[y][x];
	tiles[y][x] = new Floor(x, y);
	render(tiles[y][x]);
	refresh();
}

void Map::setNewProjectile(Projectile* other) {
	if (tiles[other->y()][other->x()]->collision(*other) == FREESPACE) {
		delete tiles[other->y()][other->x()];
		tiles[other->y()][other->x()] = other;
		projectiles.push_back(other);
		render(other);
		refresh();
	}
}