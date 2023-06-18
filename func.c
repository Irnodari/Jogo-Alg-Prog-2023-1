#include <raylib.h>
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


char** getMap(char *mapName){
	FILE* arq = fopen(mapName, "r");
	if(arq == NULL)	return NULL;
	int i;
	char** matrix = malloc((HEIGHT / LTH) *sizeof(char*));
	for(i = 0; i < HEIGHT / LTH; i++){
		matrix[i] = malloc((1 +(WIDTH / LTH)) * sizeof(char));
		fscanf(arq, "%s", matrix[i]);
	}
	fclose(arq);
	return matrix;
}

void initializeMonster(struct map *M, int coordx, int coordy){
	int i;
	if(M->Enemies == NULL){
		M->Enemies = malloc(sizeof(struct enemy));
		M->enemyNo = 1;
		i = 0;
	}
	else{
		M->Enemies = realloc(M->Enemies, (M->enemyNo + 1) * sizeof(struct enemy));
		i = M->enemyNo;
		M->enemyNo++;
	}
	M->Enemies[i].posX = coordx;
	M->Enemies[i].posY = coordy;
	M->Enemies[i].cycle = 0;
	M->Enemies[i].health = ENEMYHEALTH;
	M->Enemies[i].orientation = rand() % 4;
}

void initializePlayer(struct map *M, int coordx, int coordy){
	M->Link.posX = coordx;
	M->Link.posY = coordy;
	M->Link.health = 6;
	M->Link.orientation = 2;
	M->Link.lifes = LIFES;
	M->Link.isAtacking = false;
}

struct map* initializeMap(char* mapName){
	int i, j;
	struct map *M;
	char** matrix;
	matrix = getMap(mapName);
	if (matrix == NULL)
		return NULL;
	M = malloc(sizeof(struct map));
	M->mapNameLth = strlen(mapName);
	M->mapName = malloc(M->mapNameLth + 1);
	strcpy(M->mapName, mapName);
	M->matrix = matrix;
	M->Enemies = NULL;
	
	for (i = 0; i < HEIGHT / LTH; i++)
		for (j = 0; j < WIDTH / LTH; j++){
			switch(matrix[i][j]){
				case 'M':
					initializeMonster(M, j, i);
					matrix[i][j] = 'B';
					break;
				case 'P':
					initializePlayer(M, j, i);
					matrix[i][j] = 'B';
			}
		}
	return M;
}

int getMovement(void){
	int rv = -1;

	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
		rv = 0;
	else if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		rv = 1;
	else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
		rv = 2;
	else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		rv = 3;
	return rv;
}

int getPlayerAtack(void){
	if (IsKeyDown(KEY_J))
		return 1;
	else if (IsKeyDown(KEY_K))
		return 2;
	return 0;

}

bool isPlayerAtacking(struct map *M){
	return M->Link.isAtacking;
}

void setPlayerAtack(struct map *M, int input){
	int orientation = M->Link.orientation;
	int i, acc = 0;
	int coordx = M->Link.posX, coordy = M->Link.posY;
	M->playerAttack.yIni = M->playerAttack.yEnd = M->Link.posY;
	M->playerAttack.xIni = M->playerAttack.xEnd = M->Link.posX;

	if (orientation % 2){
		acc = 2 - orientation;
		for (i = 0; i < SWORDLEN; i++){
			if (M->matrix[coordy][coordx + acc * i] == 'O'){
				i--;
				break;
			}
		}
		if (i){
			if (acc > 0)
				M->playerAttack.xEnd += i;
			else
				M->playerAttack.xIni -= i;
		}
	}
	else{
		acc = orientation - 1;
		for (i = 0; i < SWORDLEN; i++){
			if (M->matrix[coordy + acc * i][coordx] == 'O'){
				i--;
				break;
			}
		}
		if (i){
			if (acc > 0)
				M->playerAttack.yEnd += i;
			else
				M->playerAttack.yIni -= i;
		}
	}

	M->Link.isAtacking = true;
	M->playerAttack.timer = ATACKTIMER;
}

bool canMove(struct map *M, int x, int y){
//0 = line, 1 = column
	if (x < 0 || x >= WIDTH / LTH || y < 0 || y >= HEIGHT / LTH)
		return false;
	else if(M->matrix[y][x] != 'O')
		return true;
	else
		return false;
}

bool canMovePlayer(struct map *M, int orientation){
	int tmpPosX = M->Link.posX, tmpPosY = M->Link.posY;

	if (orientation % 2)
		tmpPosX -= orientation - 2;
	else
		tmpPosY += orientation - 1;
	return canMove(M, tmpPosX, tmpPosY);
}

void movePlayer(struct map *M, int orientation){
	M->Link.orientation = orientation;
	if (orientation % 2)
		M->Link.posX -= orientation - 2;
	else
		M->Link.posY += orientation - 1;
}

void moveMonster(struct map *M, int index){
	int nxtPosX, nxtPosY;

	M->Enemies[index].cycle++;

	if(M->Enemies[index].cycle >= MAXCYCLE){
		M->Enemies[index].cycle = 0;
		M->Enemies[index].orientation = rand() % 4;
	}

	if(M->Enemies[index].orientation % 2){
		nxtPosX = M->Enemies[index].posX;
		nxtPosY = M->Enemies[index].posY - M->Enemies[index].orientation + 2;
	}
	else{
		nxtPosX = M->Enemies[index].posX - M->Enemies[index].orientation + 1;
		nxtPosY = M->Enemies[index].posY;
	}

	if (canMove(M, nxtPosX, nxtPosY)){
		M->Enemies[index].posX = nxtPosX;
		M->Enemies[index].posY = nxtPosY;
	}
	else
		M->Enemies[index].orientation = rand() % 4;
}

void compose(struct map *M){
	int i, j;
	BeginDrawing();
	ClearBackground(RAYWHITE);
	for (i = 0; i < HEIGHT / LTH; i++)
		for (j = 0; j < WIDTH / LTH; j++){
			if (M->matrix[i][j] == 'B')
				DrawRectangle(j * LTH, i * LTH, LTH, LTH, BROWN);
			else if (M->matrix[i][j] == 'O')
				DrawRectangle(j * LTH, i * LTH, LTH, LTH, GRAY);
		}
	DrawRectangle(M->Link.posX * LTH, M->Link.posY*LTH, LTH, LTH, GREEN);
	for (i = 0; i < M->enemyNo; i++)
		if (M->Enemies[i].health > 0)
			DrawRectangle(M->Enemies[i].posX * LTH, M->Enemies[i].posY * LTH, LTH, LTH, RED);
	if (isPlayerAtacking(M)){

		j = M->Link.orientation;
		if (j % 2){
			j = 2 - j;
			if (j < 0){
				i = M->playerAttack.xIni;
				while (i != M->playerAttack.xEnd){
					DrawRectangle(i * LTH, M->playerAttack.yIni * LTH, LTH, LTH, ORANGE);
					i++;
				}
			}
			else{
				i = M->playerAttack.xIni + 1;
				while (i <= M->playerAttack.xEnd){
					DrawRectangle(i * LTH, M->playerAttack.yIni * LTH, LTH, LTH, ORANGE);
					i++;
				}
			}
		}
		else{
			j--;
			if (j < 0){
				i = M->playerAttack.yIni;
				while (i != M->playerAttack.yEnd){
					DrawRectangle(M->playerAttack.xIni * LTH, i * LTH, LTH, LTH, ORANGE);
					i++;
				}
			}
			else{
				i = M->playerAttack.yIni + 1;
				while (i <= M->playerAttack.yEnd){
					DrawRectangle(M->playerAttack.xIni * LTH, i * LTH, LTH, LTH, ORANGE);
					i++;
				}
			}
		}
	}
	EndDrawing();
}

int getMonsterQuantity(struct map *M){
	return M->enemyNo;
}




void calcDMG(struct map *M){

	int enemies, i, j;

	enemies = getMonsterQuantity(M);

	for (i = 0; i < enemies; i++){
		if (M->Link.posX == M->Enemies[i].posX && M->Link.posY == M->Enemies[i].posY && M->Enemies[i].health > 0)
			dmgPlayer(M);
		if(isPlayerAtacking(M) &&
		M->Enemies[i].health > 0 &&
		M->Enemies[i].posX <= M->playerAttack.xEnd &&
		M->Enemies[i].posX >= M->playerAttack.xIni &&
		M->Enemies[i].posY <= M->playerAttack.yEnd &&
		M->Enemies[i].posY >= M->playerAttack.yIni &&
			(M->playerAttack.xEnd != M->playerAttack.xIni || 
			 M->playerAttack.yEnd != M->playerAttack.yIni))
			dmgEnemy(M, i);
	}
}

void dmgPlayer(struct map *M){
	CloseWindow();
}

void dmgEnemy(struct map *M, int enemyNum){
	M->Enemies[enemyNum].health = 0;
}
