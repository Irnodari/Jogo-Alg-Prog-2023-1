#include "functions.h"


void callPause(struct map **M, int *mapNum){
	bool shouldKeepGoing = true;
	int i = 0;
	struct map *buf;

	while(shouldKeepGoing){
		if (IsKeyPressed(KEY_S)){
			i++;
			if (i == 3)
				i = 0;
		}
		if (IsKeyPressed(KEY_W)){
			i--;
			if (i == -1)
				i = 2;
		}
		BeginDrawing();
		DrawRectangle(8 * LTH, 4 * LTH, 8 * LTH, 8 * LTH, BLACK); 
		DrawText("Continuar", 9 * LTH, 5 * LTH, 40, WHITE);
		DrawText("Salvar", 9 * LTH, 6 * LTH, 40, WHITE);
		DrawText("Carregar", 9 * LTH, 7 * LTH, 40, WHITE);
		DrawRectangle(8 * LTH + 10, 15 + 5 * LTH + i * LTH, 25, 10, RED);
		EndDrawing();
		if (IsKeyPressed(KEY_ENTER)){
			if (i == 1){
				saveGame(*M);
			}
			if (i == 2){
				buf = loadGame(&i);
				if (buf != NULL){
					closeMap(*M, false);
					*M = buf;
				}
			}
			shouldKeepGoing = false;
		}
	}
}


void callScoreboard(void){
	struct score scores[10];
	char* template = "%s %d";
	char buffer[50];
	int i = 0, j, size;
	FILE* arq = fopen("scoreboard", "rb");
	if (arq == NULL){
		while(!IsKeyPressed(KEY_ENTER)){
			BeginDrawing();
			ClearBackground(BLACK);
			DrawText("Nenhuma pontuação encontrada! Aperte Enter para sair", 20, 300, 40, WHITE);
			EndDrawing();
		}
	}
	else{
		fread(&size, sizeof(int), 1, arq);
		fread(scores, sizeof(struct score), size, arq);
		fclose(arq);
		do{
			BeginDrawing();
			ClearBackground(BLACK);
			for (j = 0; j < size; j++){
				sprintf(buffer, template, scores[j].name, scores[j].score);
				DrawText(buffer, 100, 10 + 80 * j, 50, WHITE);
			}
			DrawText("Aperte Enter para continuar", 100, 10 + 80 * j, 55, WHITE);
			EndDrawing();
		} while (!(IsKeyPressed(KEY_ENTER)));
	}
}

void saveScore(char* name, int score){
	int size, i, j;
	struct score scores[10];
	FILE* arq = fopen("scoreboard", "rb");
	if (arq != NULL){
		fread(&size, sizeof(int), 1, arq);
		fread(scores, sizeof(struct score), size, arq);
		fclose(arq);
		i = 0;
		while (i < size && score < scores[i].score) i++;
		j = size;
		while (j > i){
			if (j != 10)
				scores[j] = scores[j - 1];
			j--;
		}
		strcpy(scores[i].name, name);
		scores[i].score = score;
		if (size != 10)
			size++;
	}
	else{
		size = 1;
		strcpy(scores->name, name);
		scores->score = score;
	}
	arq = fopen("scoreboard", "wb");
	fwrite(&size, sizeof(int), 1, arq);
	fwrite(scores, sizeof(struct score), size, arq);
	fclose(arq); 
}

char getKeyPress(void){

	if (IsKeyPressed(KEY_A))
		return 'A';
	if (IsKeyPressed(KEY_B))
		return 'B';
	if (IsKeyPressed(KEY_C))
		return 'C';
	if (IsKeyPressed(KEY_D))
		return 'D';
	if (IsKeyPressed(KEY_E))
		return 'E';
	if (IsKeyPressed(KEY_F))
		return 'F';
	if (IsKeyPressed(KEY_G))
		return 'G';
	if (IsKeyPressed(KEY_H))
		return 'H';
	if (IsKeyPressed(KEY_I))
		return 'I';
	if (IsKeyPressed(KEY_J))
		return 'J';
	if (IsKeyPressed(KEY_K))
		return 'K';
	if (IsKeyPressed(KEY_L))
		return 'L';
	if (IsKeyPressed(KEY_M))
		return 'M';
	if (IsKeyPressed(KEY_N))
		return 'N';
	if (IsKeyPressed(KEY_O))
		return 'O';
	if (IsKeyPressed(KEY_P))
		return 'P';
	if (IsKeyPressed(KEY_Q))
		return 'Q';
	if (IsKeyPressed(KEY_R))
		return 'R';
	if (IsKeyPressed(KEY_S))
		return 'S';
	if (IsKeyPressed(KEY_T))
		return 'T';
	if (IsKeyPressed(KEY_U))
		return 'U';
	if (IsKeyPressed(KEY_V))
		return 'V';
	if (IsKeyPressed(KEY_W))
		return 'W';
	if (IsKeyPressed(KEY_X))
		return 'X';
	if (IsKeyPressed(KEY_Y))
		return 'Y';
	if (IsKeyPressed(KEY_Z))
		return 'Z';
	if (IsKeyPressed(KEY_BACKSPACE))
		return 0x7f;
	if (IsKeyPressed(KEY_SPACE))
		return 32;
	if (IsKeyPressed(KEY_ENTER))
		return 10;
	return 0;

}

void callWin(){

	BeginDrawing();
	ClearBackground(BLACK);
	DrawText("Você Ganhou!", 250, 350, 100, GREEN);
	EndDrawing();
	sleep(4);

}

void callLose(){
	BeginDrawing();
	ClearBackground(BLACK);
	DrawText("Você Perdeu!", 250, 350, 100, RED);
	EndDrawing();
	sleep(4);


};

void callSaveScore(int s){

	char score[] = "Sua pontuação foi %d";
	char buf[50], buf2[50] = {0};
	char a[] = "Insira o seu nome para o placar: ";
	char i = 0;
	int tmp;
	sprintf(buf, score, s);
	do{
		i = getKeyPress();
		if ((i >= 65 && i <= 91) || i == 32){
			tmp = strlen(buf2);
			if (tmp < 11){
				buf2[tmp] = i;
				buf2[tmp + 1] = '\0';
			}
		}
		else if (i == 0x7f){
			buf2[strlen(buf2) - 1] = '\0';

		}
		if (i == 10 && strlen(buf2) == 0) i = 0;
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(buf, 50, 100, 50, WHITE);
		DrawText(a, 50, 200, 50, WHITE);
		DrawText(buf2, 210, 425, 100, WHITE);
		EndDrawing();
	} while(i != 10);
	saveScore(buf2, s);
};

int callMainMenu(void){

	int i = 0, tmp, rv = 0;
	int cycle = 0;
	int input = 0;
	float width;
	bool shouldKeepGoing = true;
	while(shouldKeepGoing){
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("A Lenda de #A_SER_DEFINIDO", 144, 101, 61, RED);
		DrawText("A Lenda de #A_SER_DEFINIDO", 150, 100, 60, WHITE);
		DrawText("Iniciar", 100, 200, 40, WHITE);
		DrawText("Carregar Jogo", 100, 300, 40, WHITE);
		DrawText("Scoreboard", 100, 400, 40, WHITE);
		DrawText("Sair", 100, 500, 40, WHITE);
		switch(i){
			case 0:
				DrawRectangle(50, 215, 40, 10, RED);
				break;
			case 1:
				DrawRectangle(50, 315, 40, 10, RED);
				break;
			case 2:
				DrawRectangle(50, 415, 40, 10, RED);
				break;
			case 3:
				DrawRectangle(50, 515, 40, 10, RED);
		}
		EndDrawing();

		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
			input = 1;
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
			input = 2;
		if (IsKeyPressed(KEY_ENTER))
			input = 3;

		if (!(cycle % 15)){
			if (input == 2){
				i++;
				if (i == 4)
					i = 0;
			}
			if (input == 1){
				i--;
				if (i == -1)
					i = 3;
			}
			if (input == 3){
				if (i == 0){
					rv = 0;
					shouldKeepGoing = false;
				}
				if (i == 1){
					rv = 1;
					shouldKeepGoing = false;
				}
				if (i == 2){
					callScoreboard();	
					i = 0;
				}
				if (i == 3){
					rv = 2;
					shouldKeepGoing = false;
				}
			}
			input = 0;
		}
		cycle++;
	}
	return rv;


}

void compose(struct map *M){
	char vidas[20] = "VIDAS: %d", escore[20] = "ESCORE: %d", nivel[20] = "NIVEL: %d", buf[50];
	int i, j;
	BeginDrawing();
	ClearBackground(BLACK);

	sprintf(buf, vidas, M->Link.lifes);
	DrawText(buf, 10, 15, 30, RAYWHITE);
	sprintf(buf, nivel, M->mapNum);
	DrawText(buf, 180, 15, 30, RAYWHITE);
	sprintf(buf, escore, M->Link.score);
	DrawText(buf, 350, 15, 30, RAYWHITE);


	for (i = 0; i < HEIGHT / LTH; i++)
		for (j = 0; j < WIDTH / LTH; j++){
			if (M->matrix[i][j] == 'B')
				DrawTexture(M->Ground ,j * LTH, 60 + i * LTH, WHITE);
			else if (M->matrix[i][j] == 'O')
				DrawTexture(M->Obstacle ,j * LTH, 60 + i * LTH, WHITE);
		}
	DrawTexture(M->Link.Textures[M->Link.orientation], M->Link.posX * LTH, 60 + M->Link.posY*LTH, WHITE);

	for (i = 0; i < M->enemyNo; i++)
		if (M->Enemies[i].health > 0)
			DrawTexture(M->Enemies[i].Textures[M->Enemies[i].orientation], M->Enemies[i].posX * LTH, 60 + M->Enemies[i].posY * LTH, WHITE);
	if (isPlayerAtacking(M)){
		j = M->Link.orientation;
		if (j % 2){
			j = 2 - j;
			if (j < 0){
				i = M->playerAttack.xIni;
				while (i != M->playerAttack.xEnd){
					DrawTexture(M->playerAttack.Textures[M->Link.orientation], i * LTH, 60 + M->playerAttack.yIni * LTH, WHITE);
					i++;
				}
			}
			else{
				i = M->playerAttack.xIni + 1;
				while (i <= M->playerAttack.xEnd){
					DrawTexture(M->playerAttack.Textures[M->Link.orientation], i * LTH, 60 + M->playerAttack.yIni * LTH, WHITE);
					i++;
				}
			}
		}
		else{
			j--;
			if (j < 0){
				i = M->playerAttack.yIni;
				while (i != M->playerAttack.yEnd){
					DrawTexture(M->playerAttack.Textures[M->Link.orientation], M->playerAttack.xIni * LTH, 60 + i * LTH, WHITE);
					i++;
				}
			}
			else{
				i = M->playerAttack.yIni + 1;
				while (i <= M->playerAttack.yEnd){
					DrawTexture(M->playerAttack.Textures[M->Link.orientation], M->playerAttack.xIni * LTH, 60 + i * LTH, WHITE);
					i++;
				}
			}
		}
	}
	EndDrawing();
}

