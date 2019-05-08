#include "./data.h"

#ifndef __JOGADAREV__
#define __JOGADAREV__

void resetJogada(Jogada*);
int cercoByDir(ESTADO*,Posicao*,int,int);
void checkCerco(ESTADO*,Jogada*);
Jogada isValidPlay(ESTADO*,Posicao*);
int getValidPlays(ESTADO*,Posicao**);
Jogada nextValidPlay(ESTADO*,Posicao*);
Jogada maxPlay(ESTADO*);

#endif
