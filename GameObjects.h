#pragma once
#include "GameSettings.h"

class Tile;
class Actor;
class Player;
class StaticObject;
class Projectile;

class MapInterface
{
public:
	virtual void swap(Tile*, Tile*) = 0;
	virtual Tile* getObject(int x, int y) = 0;
	virtual void removeObject(int x, int y) = 0;
	virtual void setNewProjectile(Projectile*) = 0;
};

static MapInterface* map;
void setMap(MapInterface* actual_map);

class GameInterface
{
public:
	virtual void showHealth(Actor*, std::string name, int x, int y) = 0;
	virtual void setWin() = 0;
	virtual void setLose() = 0;
};

static GameInterface* game;
void setGame(GameInterface* actual_game);

//Base abstract class
class Tile
{
public:
	Tile(Type type, char s, Color f, Color b, int x, int y) : type(type), _symbol(s), _foreground(f), _background(b), _x(x), _y(y) {};
	virtual ~Tile() {};

	virtual int x() = 0;
	virtual int y() = 0;
	virtual void x(int x) = 0;
	virtual void y(int y) = 0;
	virtual char symbol() = 0;
	virtual Color foreground() = 0;
	virtual Color background() = 0;

	virtual CollisionType collision(Tile&) = 0;
	virtual CollisionType collision(Actor&) = 0;
	virtual CollisionType collision(StaticObject&) = 0;

	Type type;

protected:
	int _x;
	int _y;
	char _symbol;
	Color _foreground;
	Color _background;
};

//Static object classes
class StaticObject : public Tile
{
public:
	StaticObject(char s, Color f, Color b, int x, int y) : Tile(Type::NONPLAYER, s, f, b, x, y) {};
	~StaticObject() {};

	virtual int x();
	virtual int y();
	virtual void x(int x);
	virtual void y(int y);
	virtual char symbol();
	virtual Color foreground();
	virtual Color background();

	virtual CollisionType collision(Tile&);
	virtual CollisionType collision(Actor&);
	virtual CollisionType collision(StaticObject&);
};

class Floor : public StaticObject
{
public:
	Floor(int x, int y) : StaticObject(FLOOR_SYMBOL, GRAY, BLACK, x, y) {};
	~Floor() {};

	CollisionType collision(Tile&);
	CollisionType collision(Actor&);
	CollisionType collision(StaticObject&);
};

class Wall : public StaticObject
{
public:
	Wall(int x, int y) : StaticObject(WALL_SYMBOL, YELLOW, BLACK, x, y) {};
	~Wall() {};
};

class HealingPotion : public StaticObject
{
public:
	HealingPotion(int x, int y) : StaticObject(HEALING_POTION_SYMBOL, RED, BLACK, x, y) {};
	~HealingPotion() {};

	void heal(Actor*);

	CollisionType collision(Actor&);
};

//Actor classes
class Actor : public Tile
{
public:
	Actor(Type t, char s, Color f, Color b, int h, int d, int x, int y) : Tile(t, s, f, b, x, y), _health(h), _damage(d) {};
	~Actor() {};

	virtual int health();
	virtual int damage();
	virtual void health(int h);
	virtual void damage(int d);

	virtual int x();
	virtual int y();
	virtual void x(int x);
	virtual void y(int y);
	virtual char symbol();
	virtual Color foreground();
	virtual Color background();

	virtual void attack(Actor&);
	virtual void receiveDamage(int damage);
	virtual void kill(Actor&);
	virtual void use(Tile*);

	virtual void move(int x, int y);
	virtual void moveLeft();
	virtual void moveRight();
	virtual void moveUp();
	virtual void moveDown();

	virtual void shootLeft();
	virtual void shootRight();
	virtual void shootUp();
	virtual void shootDown();

	virtual CollisionType collision(Tile&);
	virtual CollisionType collision(Actor&);
	virtual CollisionType collision(StaticObject&);

protected:
	int _health;
	int _damage;
};

class Player : public Actor
{
public:
	Player(int x, int y) : Actor(Type::PLAYER, KNIGHT_SYMBOL, CYAN, BLACK, STANDARD_HEALTH, DEF_HERO_ATTACK, x, y), _armor(LEATHER_ARMOR) {};
	~Player() {};

	int armor();
	void armor(int a);
	void receiveDamage(int damage);

private:
	int _armor;
};

class Princess : public Actor
{
public:
	Princess(int x, int y) : Actor(Type::NPC, PRINCESS_SYMBOL, MAGENTA, BLACK, NONE, NONE, x, y) {};
	~Princess() {};
};

class Zombie : public Actor
{
public:
	Zombie(int x, int y) : Actor(Type::MONSTER, ZOMBIE_SYMBOL, GREEN, BLACK, LOW_HEALTH, DEF_ZOMBIE_ATTACK, x, y) {};
	~Zombie() {};

	void shootLeft() {};
	void shootRight() {};
	void shootUp() {};
	void shootDown() {};
};

class Dragon : public Actor
{
public:
	Dragon(int x, int y) : Actor(Type::MONSTER, DRAGON_SYMBOL, RED, BLACK, EXTENDED_HEALTH, DEF_DRAGON_ATTACK, x, y) {};
	~Dragon() {};
};

class Projectile : public Actor
{
public:
	Projectile(Color f, Color b, int x, int y, int dir_x, int dir_y) : Actor(Type::PROJECTILE, PROJECTILE_SYMBOL, f, b, PROJECTILE_LIFETIME, DEF_DRAGON_ATTACK, x, y), dir_x(dir_x), dir_y(dir_y) {};
	~Projectile() {};

	int dir_x;
	int dir_y;
};