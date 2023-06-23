#include <raylib.h>

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

struct player{
	int posX, posXIni;
	int posY, posYIni;
	short orientation;

	int health;
	int lifes;
	bool isAtacking;

	Texture2D spriteD;
	Texture2D spriteR;
	Texture2D spriteU;
	Texture2D spriteL;
};

struct enemy{
	int posX, posXIni;
	int posY, posYIni;
	short orientation;
	int cycle;
	int health;
	int type;

	Texture2D spriteD;
	Texture2D spriteR;
	Texture2D spriteU;
	Texture2D spriteL;

};

struct dangerArea{
	int xIni, xEnd, yIni, yEnd;
	int timer;
};

struct map{
	int mapNum;
	char** matrix;
	struct enemy* Enemies;
	int enemyNo;
	int aliveEnemyNo;
	struct player Link;
	struct dangerArea playerAttack;
};

bool calcDMG(struct map *M);

bool dmgPlayer(struct map *M);

void dmgEnemy(struct map *M, int enemyNum);

char** getMap(char *mapName);

struct map* initializeMap(char* mapName);

void saveGame(struct map *M);

struct map* loadGame(int* mapNum);
//retorna orientação
int getMovement(void);

int getPlayerAtack(void);

void setPlayerAtack(struct map *M, int input);

bool isPlayerAtacking(struct map *M);

bool canMove(struct map *M, int x, int y);

bool canMovePlayer(struct map *M, int orientation);

void moveMonster(struct map *M, int index);

void movePlayer(struct map *M, int orientation);

void compose(struct map *M);

int getMonsterQuantity(struct map *M);

void closeMap(struct map *M, bool endGame);
