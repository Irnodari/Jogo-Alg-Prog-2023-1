#include "structsDefs.h"

void initializeMonster(struct map *M, int coordx, int coordy);

void initializePlayer(struct map *M, int coordx, int coordy);

void saveScore(char* name, int score);

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

void callScoreboard(void);
