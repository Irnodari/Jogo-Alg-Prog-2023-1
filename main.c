#include "functions.h"

int main(void){
	srand(clock());
	struct map *M, *buf;
	int i, j, input, frame = 0, mapNum = 1, controlState;
	char* template = "mapas/mapa%d.txt";
	char mapName[32];
	struct player TemporaryPlayer;
	bool shouldKeepGoing;
	bool gameEnded = false;


	TemporaryPlayer.health = -2;

	InitWindow(WIDTH,60 + HEIGHT, "Game");
	SetTargetFPS(FPS);

	controlState = callMainMenu();
	if (controlState == 2){
		CloseWindow();
		return 0;
	}

	do{
		sprintf(mapName, template, mapNum);
		M = initializeMap(mapName);
		if (M != NULL){
			M->mapNum = mapNum;
			if (TemporaryPlayer.health != -2){
				TemporaryPlayer.posX = TemporaryPlayer.posXIni = M->Link.posX;
				TemporaryPlayer.posY = TemporaryPlayer.posYIni = M->Link.posY;
				TemporaryPlayer.orientation = 2;
				M->Link = TemporaryPlayer;
				M->mapNum = mapNum;
			}
			do{
				if (controlState == 1){
					buf = loadGame(&i);
					if (buf != NULL){
						closeMap(M, false);
						M = buf;
					}
					controlState = 0;
				}
				if (IsKeyPressed(KEY_P))
						callPause(&M, &mapNum);
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
						buf = loadGame(&i);
						if (buf != NULL){
							closeMap(M, false);
							M = buf;
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
			} while(!WindowShouldClose() && !calcDMG(M) && M->aliveEnemyNo);

			TemporaryPlayer = M->Link;
			TemporaryPlayer.isAtacking = false;
			shouldKeepGoing = !(M->aliveEnemyNo);
			closeMap(M, false);
			M = NULL;
			mapNum++;
		}
		else{
			shouldKeepGoing = false;
			gameEnded = true;
		}
	} while(shouldKeepGoing);
	if (TemporaryPlayer.lifes >= 0 && gameEnded){
		callWin();
		callSaveScore(TemporaryPlayer.score);
	}
	else if (TemporaryPlayer.lifes < 0){
		callLose();
		callSaveScore(TemporaryPlayer.score);
	}
	

	if (M != NULL)	
		closeMap(M, true);
	else
		CloseWindow();

	return 0;
}
