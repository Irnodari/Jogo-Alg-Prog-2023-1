#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <raylib.h>
#include "header.h"
#include <time.h>

int main(void){
	srand(clock());
	struct map *M;
	int i, j;
	int input, frame = 0;
	char* template = "mapas/mapa%d.txt";
	char mapName[32];
	int mapNum = 1;
	struct player TemporaryPlayer;
	bool shouldKeepGoing;

	system("touch save");

	TemporaryPlayer.health = -2;

	InitWindow(WIDTH, HEIGHT, "Game");

	SetTargetFPS(FPS);
	do{

		sprintf(mapName, template, mapNum);
		M = initializeMap(mapName);
		if (M != NULL){
			if (TemporaryPlayer.health != -2){
				TemporaryPlayer.posX = TemporaryPlayer.posXIni = M->Link.posX;
				TemporaryPlayer.posY = TemporaryPlayer.posYIni = M->Link.posY;
				TemporaryPlayer.orientation = 2;
				M->Link = TemporaryPlayer;
				M->mapNum = mapNum;
			}
			do{
				if (!(frame % TICK)){
					input = getMovement();
					if (isPlayerAtacking(M)){
						M->playerAttack.timer--;
						if (!M->playerAttack.timer)
							M->Link.isAtacking = false;
					}
					else if (input == -1){
						input = getPlayerAtack();
						if (input){
							setPlayerAtack(M, input);
						}
					}
					else if (input == 4)
						saveGame(M);
					else if (input == 5){
						closeMap(M, false);
						M = loadGame(&mapNum);
					}
					else if (input != -1){
						if(canMovePlayer(M, input))
							movePlayer(M, input);
						else
							M->Link.orientation = input;
					}
				}
				if (!((frame+1) % (4*TICK))){
					j = getMonsterQuantity(M);
					for (i = 0; i < j; i++) 
						moveMonster(M, i);
				}
	
				compose(M);
	
				frame++;
				if (frame == FPS)
					frame = 0;
			}while(!WindowShouldClose() && !calcDMG(M) && M->aliveEnemyNo);

			TemporaryPlayer = M->Link;
			TemporaryPlayer.isAtacking = false;
			shouldKeepGoing = !(M->aliveEnemyNo);
			closeMap(M, false);
			M = NULL;
			mapNum++;
		}
		else{
			printf("Você ganhou!\n");
			shouldKeepGoing = false;
		}
	}while(shouldKeepGoing);
	if (M != NULL)	
		closeMap(M, true);

	return 0;
}
