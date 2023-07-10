#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

//Esse header é um header exclusivo de Unix, mas a função chamada dele está disponível no windows.h
#include <unistd.h>

//eu tenho a leeeeve impressão de que o Unix e o Windows não tratam sleeps da mesma forma, mas não tenho certeza.
//O sleep é usado nas funções callWin() e callLose(), que estão no graphic.c
//#include <windows.h>

//Define as vidas do jogador no inicio.
#define LIFES 3
//define a velocidade do jogo e dos inimigos. Inimigos caminham e ativam a IA deles a cada QNTTICKMONSTRO ticks. O jogador ganha um movimento ou ataque por tick.
#define TICK 15
//define a cada quantos ticks os monstros se mexerão.
#define QNTTICKMONSTRO 4
//Comprimento da espada.
#define SWORDLEN 5
//O número de ticks que dura um ataque.
#define ATACKTIMER 2
//Quantos ciclos de IA os monstros rodam antes de randomizar a IA de novo
#define MAXCYCLE 5
//Taxa de quadros
#define FPS 120

//Isso é legado de uma ideia antiga da espada dar dano e knockback. só precisa ser um número superior a zero.
#define ENEMYHEALTH 25

//Dimensões. 60 pixels serão acescidos da altura para a barra superior de stats. Mudar isso irá dar problema com os sprites, pois eles são cortados para caber em 2500 pixels em um quadrado.
#define WIDTH 1200
#define HEIGHT 800
#define LTH 50

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

