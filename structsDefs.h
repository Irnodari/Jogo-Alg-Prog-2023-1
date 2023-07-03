#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define WIDTH 1200
#define HEIGHT 800
#define LTH 50
#define ENEMYHEALTH 25
#define LIFES 3
#define MAXCYCLE 5
#define FPS 120
#define TICK 15
#define SWORDLEN 5
#define ATACKTIMER 2

struct score{
	char name[50];
	int score;

};

struct player{
	int posX, posXIni;
	int posY, posYIni;
	short orientation;

	int health;
	int lifes;
	bool isAtacking;
	int score;

	Texture2D Textures[4];
};

struct enemy{
	int posX, posXIni;
	int posY, posYIni;
	short orientation;
	int cycle;
	int health;
	int type;

	Texture2D Textures[4];

};

struct dangerArea{
	int xIni, xEnd, yIni, yEnd;
	int timer;
	Texture2D Textures[4];
};

struct map{
	int mapNum;
	char** matrix;
	struct enemy* Enemies;
	int enemyNo;
	int aliveEnemyNo;
	struct player Link;
	struct dangerArea playerAttack;

	Texture2D Ground;
	Texture2D Obstacle;
};

