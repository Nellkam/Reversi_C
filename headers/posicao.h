#include "./data.h"

#ifndef __POSREV__
#define __POSREV__

Posicao* freeAllPos(Posicao*);
void freeLastPos(Posicao**);
int posListSize(Posicao*);
boolean existePos(Posicao*,Posicao);
void addPos(Posicao**,Posicao*);


#endif
