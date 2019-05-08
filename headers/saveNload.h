#include "./data.h"

#ifndef __SAVENLOADREV__
#define __SAVENLOADREV__


void pushGame(ESTADO*,Historico**);
void popGame(ESTADO*,Historico**);
int sizeHistorico(Historico**);
void startHistorico(Historico**,char,VALOR,int);
Historico* freeAllHistorico(Historico*);


int saveGame(Historico**,char*);
int loadGame(Historico**,ESTADO*,char*);

#endif
