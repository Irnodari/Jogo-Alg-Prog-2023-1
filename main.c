#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>
#include "header.h"
#include <time.h>

int main(void){
	srand(clock());
	struct map *M;
	int i, j;
	int input, frame = 0;
	char* mapName = "mapas/mapa1.txt";
	M = initializeMap(mapName);

	if (M != NULL){

		InitWindow(WIDTH, HEIGHT, "Game");

		SetTargetFPS(FPS);

		while(!WindowShouldClose()){


			calcDMG(M);


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
		}
	}
	else
		printf("Erro na abertura do mapa!\n");

	return 0;
}
