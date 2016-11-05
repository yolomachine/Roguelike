#pragma once
#include "GameObjects.h"

void setMap(MapInterface* actual_map) {
	map = actual_map;
}

void setGame(GameInterface* actual_game) {
	game = actual_game;
}

//StaticObjects properties
void StaticObject::x(int x) { _x = x; };
void StaticObject::y(int y) { _y = y; };

int StaticObject::x() { return _x; };
int StaticObject::y() { return _y; };
char StaticObject::symbol() { return _symbol; };
Color StaticObject::foreground() { return _foreground; };
Color StaticObject::background() { return _background; };

//StaticObjects collisions
CollisionType StaticObject::collision(Tile&) {
	return WALL;
};
CollisionType StaticObject::collision(Actor&) {
	return WALL;
};
CollisionType StaticObject::collision(StaticObject&) {
	return WALL;
};

//Floor collisions
CollisionType Floor::collision(Tile&) { 
	return FREESPACE; 
};
CollisionType Floor::collision(Actor&) {
	return FREESPACE; 
};
CollisionType Floor::collision(StaticObject&) {
	return FREESPACE; 
};

//Pickup collisions
CollisionType HealingPotion::collision(Actor& other) { 
	if (other.type == Type::PLAYER) return PICKUP; 
	return WALL;
};

//Actor properties
void Actor::x(int x) { _x = x; };
void Actor::y(int y) { _y = y; };
void Actor::health(int h) { _health = h; };
void Actor::damage(int d) { _damage = d; };

int Actor::x() { return _x; };
int Actor::y() { return _y; };
int Actor::health() { return _health; };
int Actor::damage() { return _damage; };
char Actor::symbol() { return _symbol; };
Color Actor::foreground() { return _foreground; };
Color Actor::background() { return _background; };

//Player properties
void Player::armor(int a) { _armor = a; };

int Player::armor() { return _armor; };

//Actor collisions
CollisionType Actor::collision(Tile& other) {
	return other.collision(*this);
}
CollisionType Actor::collision(Actor& other) {
	if (this->type == Type::PLAYER && other.type == Type::NPC) return ENDGAME;
	if (this->type == Type::NPC && other.type == Type::PLAYER) return ENDGAME;
	if (this->type == Type::PLAYER && other.type == Type::MONSTER) return ENEMY;
	if (this->type == Type::MONSTER && other.type == Type::PLAYER) return ENEMY;
	if (other.type == Type::PROJECTILE && (this->type == Type::PLAYER || this->type == Type::MONSTER)) return ENEMY;
	return WALL;
}
CollisionType Actor::collision(StaticObject& other) {
	return other.collision(*this);
}

//Actor receiving damage
void Actor::attack(Actor& other) {
	other.receiveDamage(_damage);
	if (other.health() <= 0) kill(other);
	if (this->health() == 0) kill(*this);
}
void Actor::receiveDamage(int damage) {
	_health -= damage;
	game->showHealth(this,"Monster", 0, 1);
}
void Player::receiveDamage(int damage) {
	if (_armor - damage < 0) _health -= damage - _armor;
	game->showHealth(this, "Player", 0, 0);
}
void Actor::kill(Actor& other) {
	map->removeObject(other.x(), other.y());
	if (other.type == Type::PLAYER) game->setLose();
}

//Actor moves
void Actor::move(int x, int y) {
	Tile* other = map->getObject(x, y);
	switch (this->collision(*other)) {
		case FREESPACE: map->swap(this, other); break;
		case ENEMY: this->attack(static_cast<Actor&>(*other)); if (this->type == Type::PROJECTILE) map->removeObject(this->x(), this->y()); break;
		case PICKUP: this->use(other); map->swap(this, map->getObject(x, y)); break;
		case ENDGAME: map->removeObject(this->x(), this->y()); game->setWin();
		default: if (this->type == Type::PROJECTILE) map->removeObject(this->x(), this->y()); break;
	}
	refresh();
}
void Actor::moveLeft() {
	move(x() - 1, y());
}
void Actor::moveRight() {
	move(x() + 1, y());
}
void Actor::moveUp() {
	move(x(), y() - 1);
}
void Actor::moveDown() {
	move(x(), y() + 1);
}
void Actor::shootLeft() {
	map->setNewProjectile(new Projectile(RED, BLACK, this->x() - PROJECTILE_SPEED, this->y(), -PROJECTILE_SPEED, 0));
}
void Actor::shootRight() {
	map->setNewProjectile(new Projectile(RED, BLACK, this->x() + PROJECTILE_SPEED, this->y(), PROJECTILE_SPEED, 0));
}
void Actor::shootUp() {
	map->setNewProjectile(new Projectile(RED, BLACK, this->x(), this->y() - PROJECTILE_SPEED, 0, -PROJECTILE_SPEED));
}
void Actor::shootDown() {
	map->setNewProjectile(new Projectile(RED, BLACK, this->x(), this->y() + PROJECTILE_SPEED, 0, PROJECTILE_SPEED));
}

//Healing
void Actor::use(Tile* other) {
	HealingPotion* hp = static_cast<HealingPotion*>(other);
	hp->heal(this);
	game->showHealth(this, "Player", 0, 0);
	map->removeObject(other->x(), other->y());
}

void HealingPotion::heal(Actor* other) {
	other->health(STANDARD_HEALTH);
}