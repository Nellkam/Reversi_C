#include "./data.h"

#ifndef __PLAYREV__
#define __PLAYREV__

void resetTabuleiro(ESTADO*);
void resetPoints(Pontos*);
void resetGame(ESTADO*,Historico**,Pontos*,VALOR,char);

Pontos getPoints(ESTADO*);
void changeMode(ESTADO*);

void updateTabuByDir(ESTADO*,Posicao*,const int,const int,const int);
void updateTabuleiro(ESTADO*,Jogada*);
void updatePoints(Jogada*,Pontos*);

void nextTurn(ESTADO*);
boolean isEndgame(ESTADO*);    

void printTabuleiro(ESTADO*,Pontos*,int);
void copyTabuleiro(ESTADO*,ESTADO*);

boolean play(ESTADO*,Posicao*,Pontos*,Historico**);

#endif
