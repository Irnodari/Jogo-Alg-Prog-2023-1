#include "functions.h"

//Controle do mapa
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

struct map* initializeMap(char* mapName){
	int i, j;
	struct map *M;
	char** matrix;
	matrix = getMap(mapName);
	if (matrix == NULL)
		return NULL;
	M = malloc(sizeof(struct map));
	M->mapNum = 0;
	M->matrix = matrix;
	M->Enemies = NULL;
	M->Ground = LoadTexture("./sprites/Ground.png");
	M->Obstacle = LoadTexture("./sprites/Obstacle.png");

	M->playerAttack.Textures[1] = LoadTexture("./sprites/Attack_right.png");
	M->playerAttack.Textures[3] = LoadTexture("./sprites/Attack_left.png");
	M->playerAttack.Textures[0] = LoadTexture("./sprites/Attack_up.png");
	M->playerAttack.Textures[2] = LoadTexture("./sprites/Attack_down.png");

	
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
	M->aliveEnemyNo = M->enemyNo;
	return M;
}

void closeMap(struct map *M, bool endGame){
	int i;
	if (endGame){
		CloseWindow();
	}
	free(M->Enemies);
	for (i = 0; i < HEIGHT / LTH; i++){
		free(M->matrix[i]);
	}
	free(M);
}

//controle dos elementos do jogo
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
	M->Enemies[i].posX = M->Enemies[i].posXIni = coordx;
	M->Enemies[i].posY = M->Enemies[i].posYIni = coordy;
	M->Enemies[i].cycle = 0;
	M->Enemies[i].health = ENEMYHEALTH;
	M->Enemies[i].orientation = rand() % 4;
	M->Enemies[i].Textures[0] = LoadTexture("./sprites/Enemy_right.png");
	M->Enemies[i].Textures[2] = LoadTexture("./sprites/Enemy_left.png");
	M->Enemies[i].Textures[3] = LoadTexture("./sprites/Enemy_back.png");
	M->Enemies[i].Textures[1] = LoadTexture("./sprites/Enemy_front.png");
}

void initializePlayer(struct map *M, int coordx, int coordy){
	M->Link.posX = M->Link.posXIni = coordx;
	M->Link.posY = M->Link.posYIni = coordy;
	M->Link.health = 3;
	M->Link.orientation = 2;
	M->Link.lifes = LIFES;
	M->Link.isAtacking = false;
	M->Link.score = 0;
	M->Link.Textures[1] = LoadTexture("./sprites/Link_right.png");
	M->Link.Textures[3] = LoadTexture("./sprites/Link_left.png");
	M->Link.Textures[0] = LoadTexture("./sprites/Link_back.png");
	M->Link.Textures[2] = LoadTexture("./sprites/Link_front.png");

}


//funções de salvamento e carregamento
void saveGame(struct map *M){

	int i;
	FILE* arq = fopen("save", "wb");
	fwrite(M, sizeof(struct map), 1, arq);
	for (i = 0; i < HEIGHT / LTH; i++)
		fwrite(M->matrix[i], 1, 1 + WIDTH / LTH, arq);
	fwrite(M->Enemies, sizeof(struct enemy), M->enemyNo, arq);
	fclose(arq);
}

struct map* loadGame(int* mapName){
	int i, j;
	FILE* arq = fopen("save", "rb");
	if (arq == NULL)
		return NULL;
	struct map *M = malloc(sizeof(struct map));
	fread(M, sizeof(struct map), 1, arq);
	M->matrix = malloc((HEIGHT / LTH) * 8);
	for (i = 0; i < HEIGHT / LTH; i++){
		M->matrix[i] = malloc(1 + WIDTH / LTH);
		fread(M->matrix[i], 1, 1 + WIDTH / LTH, arq);
	}
	j = M->enemyNo;
	M->Enemies = malloc(j * sizeof(struct enemy));
	fread(M->Enemies, sizeof(struct enemy), j, arq);

	M->Link.Textures[1] = LoadTexture("./sprites/Link_right.png");
	M->Link.Textures[3] = LoadTexture("./sprites/Link_left.png");
	M->Link.Textures[0] = LoadTexture("./sprites/Link_back.png");
	M->Link.Textures[2] = LoadTexture("./sprites/Link_front.png");

	M->playerAttack.Textures[1] = LoadTexture("./sprites/Attack_right.png");
	M->playerAttack.Textures[3] = LoadTexture("./sprites/Attack_left.png");
	M->playerAttack.Textures[0] = LoadTexture("./sprites/Attack_up.png");
	M->playerAttack.Textures[2] = LoadTexture("./sprites/Attack_down.png");

	M->Ground = LoadTexture("./sprites/Ground.png");
	M->Obstacle = LoadTexture("./sprites/Obstacle.png");

	for (i = 0; i < M->enemyNo; i++){
		M->Enemies[i].Textures[0] = LoadTexture("./sprites/Enemy_right.png");
		M->Enemies[i].Textures[2] = LoadTexture("./sprites/Enemy_left.png");
		M->Enemies[i].Textures[3] = LoadTexture("./sprites/Enemy_back.png");
		M->Enemies[i].Textures[1] = LoadTexture("./sprites/Enemy_front.png");
	}
	*mapName = M->mapNum;
	fclose(arq);
	return M;
}


//Salvamento de score

void saveScore(char* name, int score){
	int size, i, j;
	struct score scores[10];
	FILE* arq = fopen("scoreboard", "rb");
	if (arq != NULL){
		fread(&size, sizeof(int), 1, arq);
		fread(scores, sizeof(struct score), size, arq);
		fclose(arq);
		i = 0;
		while (i < size && score <= scores[i].score) i++;
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



//funções de controle e IA
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
	else if (IsKeyDown(KEY_Y))
		rv = 4;
	else if (IsKeyDown(KEY_U))
		rv = 5;
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
		for (i = 0; i <= SWORDLEN; i++){
			if (coordx + acc * i > WIDTH / LTH || coordx + acc * i < 0 || M->matrix[coordy][coordx + acc * i] == 'O'){
				break;
			}
		}
		i--;
		if (i){
			if (acc > 0)
				M->playerAttack.xEnd += i;
			else
				M->playerAttack.xIni -= i;
		}
	}
	else{
		acc = orientation - 1;
		for (i = 0; i <= SWORDLEN; i++){
			if (coordy + acc * i > HEIGHT / LTH - 1 || coordy + acc * i < 0 || M->matrix[coordy + acc * i][coordx] == 'O'){
				break;
			}
		}
		i--;
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

int getMonsterQuantity(struct map *M){
	return M->enemyNo;
}

bool calcDMG(struct map *M){

	int enemies, i, j;

	enemies = getMonsterQuantity(M);

	for (i = 0; i < enemies; i++){
		if (M->Link.posX == M->Enemies[i].posX && M->Link.posY == M->Enemies[i].posY && M->Enemies[i].health > 0)
			return dmgPlayer(M);
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
	return 0;
}

bool dmgPlayer(struct map *M){
	int i;
	int rv;
	if (M->Link.lifes - 1 > 0){
		compose(M);
		M->Link.posX = M->Link.posXIni;
		M->Link.posY = M->Link.posYIni;

		for (i = 0; i < M->enemyNo; i++){
			M->Enemies[i].posX = M->Enemies[i].posXIni;
			M->Enemies[i].posY = M->Enemies[i].posYIni;
			M->Enemies[i].orientation = rand() % 4;
		}
		rv = 0;
	}
	else{
		rv = 1;
		M->Link.lifes--;
	}
	M->Link.lifes--;
	return rv;
}

void dmgEnemy(struct map *M, int enemyNum){
	M->Enemies[enemyNum].health = 0;
	M->aliveEnemyNo--;
	M->Link.score += 100;
}


