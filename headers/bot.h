#include "./data.h"

#ifndef __BOTREV__
#define __BOTREV__

Minimax resetMinimax();
void resetNode(Node*);
void freeNode(Node**);
void comparePlays(Minimax*,Node*);
void minimax(Node**,Minimax*,const int);
Posicao playBot(ESTADO*,Pontos*,Historico**,const int);

#endif
