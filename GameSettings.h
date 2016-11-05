#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <typeinfo>
#include <curses.h>

enum class Type    { NONPLAYER, NPC, PLAYER, MONSTER, PROJECTILE };
enum CollisionType { FREESPACE, WALL, ENEMY, ENDGAME, PICKUP };

typedef int Color;

const int NONE = 0;

//Console colors
const Color BLACK    =  0;
const Color BLUE     =  1;
const Color YELLOW   =  6;
const Color WHITE    =  7;
const Color GRAY     =  8;
const Color GREEN    =  10;
const Color CYAN     =  11;
const Color RED      =  12;
const Color MAGENTA  =  13;

//Weapons
const int FISTS_DAMAGE = 5;
const int STICK_DAMAGE = 10;
const int SWORD_DAMAGE = 50;

//Armors
const int LEATHER_ARMOR   = 5;
const int CHAINMAIL_ARMOR = 10;
const int LIGHT_PLATE_ARMOR = 30;
const int HEAVY_PLATE_ARMOR = 50;

//Default actors' damage
const int DEF_HERO_ATTACK = 10;
const int DEF_ZOMBIE_ATTACK = 2;
const int DEF_DRAGON_ATTACK = 30;

//Health
const int LOW_HEALTH = 50;
const int STANDARD_HEALTH = 100;
const int EXTENDED_HEALTH = 200;
const int PROJECTILE_LIFETIME = 3;
const int PROJECTILE_SPEED = 1;

//Game objects
const char WALL_SYMBOL  = '#';
const char FLOOR_SYMBOL = '.';
const char KNIGHT_SYMBOL = 'K';
const char ZOMBIE_SYMBOL = 'Z';
const char DRAGON_SYMBOL = 'D';
const char PRINCESS_SYMBOL = 'P';
const char PROJECTILE_SYMBOL = '*';
const char HEALING_POTION_SYMBOL = '&';

//States
#define WIN 1
#define LOSE 2