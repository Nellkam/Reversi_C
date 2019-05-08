#include <stdio.h>
#include <stdlib.h>
#include "../headers/posicao.h"
#include "../headers/jogada.h"

void resetJogada(Jogada* jogada)    
{    
    jogada->isvalid=false;    
    jogada->turn=VAZIA;    
    jogada->cima = jogada->baixo = jogada->esq = jogada->dir = 0;    
    jogada->ce = jogada->cd = jogada->be = jogada->bd = 0;    
    jogada->soma=0;    
}


int cercoByDir(ESTADO* game,Posicao* coords,int x,int y)
{
    int contador=0,l,c;
    VALOR enemy = game->peca==VALOR_X ? VALOR_O : VALOR_X;
    
    l = coords->linha  ;
    c = coords->coluna ;
    l += x;
    c += y;
    
    if( 
            l<0 || l>7 ||
            c<0 || c>7
      )
        return 0;

    for(  ; game->grelha[l][c]==enemy ; l+=x,c+=y )
    {
        contador++;

        if(
            (l+x)<0 || (l+x)>7 ||
            (c+y)<0 || (c+y)>7 
          )
          return 0;
    }        

    if(game->grelha[l][c]==VAZIA)
        return 0;
    
    return contador;
}


void checkCerco(ESTADO* gameptr, Jogada* jogadaptr)
{

    jogadaptr->cima  = cercoByDir(gameptr,&(jogadaptr->coords),-1, 0);
    jogadaptr->baixo = cercoByDir(gameptr,&(jogadaptr->coords), 1, 0);
    jogadaptr->esq   = cercoByDir(gameptr,&(jogadaptr->coords), 0,-1);
    jogadaptr->dir   = cercoByDir(gameptr,&(jogadaptr->coords), 0, 1);
    jogadaptr->ce    = cercoByDir(gameptr,&(jogadaptr->coords),-1,-1);
    jogadaptr->cd    = cercoByDir(gameptr,&(jogadaptr->coords),-1, 1);
    jogadaptr->be    = cercoByDir(gameptr,&(jogadaptr->coords), 1,-1);
    jogadaptr->bd    = cercoByDir(gameptr,&(jogadaptr->coords), 1, 1);
    // fim contagens

    jogadaptr->soma = jogadaptr->cima + jogadaptr->baixo + jogadaptr->esq + jogadaptr->dir +
                        jogadaptr->ce + jogadaptr->cd + jogadaptr->be + jogadaptr->bd;

    if(jogadaptr->soma > 0)
        jogadaptr->isvalid=true;
}


Jogada isValidPlay(ESTADO* gameptr, Posicao* coords)
{
    Jogada jogada;
    
    resetJogada(&jogada);
    
    jogada.coords.linha  = coords->linha;
    jogada.coords.coluna = coords->coluna;
    jogada.turn          = gameptr->peca;
    
    // coordenadas fora do mapa
    if(
            coords->linha<0  || coords->linha>7 ||
            coords->coluna<0 || coords->coluna>7 
      )
        return jogada;

    // caso na posicao escolhida ja exista jogada
    if(gameptr->grelha[coords->linha][coords->coluna]!=VAZIA)
        return jogada;

    // contagens a converter em cada direcao. Caso nenhuma conversao->jogada invalida
    checkCerco(gameptr,&jogada);

    return jogada;
}


int getValidPlays(ESTADO* gameptr, Posicao** listptr)
{
    Posicao coords;
    Jogada jogada;
    Posicao** nextPos=NULL;
    Posicao* newPos=NULL;
    int size=0;

    if( (*listptr) != NULL )
        *listptr=freeAllPos(*listptr);

    for(coords.linha=0 ; coords.linha<8 ; coords.linha++)
        for(coords.coluna=0 ; coords.coluna<8 ; coords.coluna++)
        {
            jogada=isValidPlay(gameptr,&coords);

            if(jogada.isvalid==true)
            {
                // pointer inicio da lista
                if(*listptr==NULL)
                {
                    *listptr = malloc(sizeof(Posicao));
                    (*listptr)->linha  = coords.linha;
                    (*listptr)->coluna = coords.coluna;
                    (*listptr)->next   = NULL;
                    nextPos = &( (*listptr)->next );

                }
                
                else
                {
                    newPos = malloc(sizeof(Posicao));
                    newPos->linha  = coords.linha;
                    newPos->coluna = coords.coluna;
                    newPos->next   = NULL;
                    *nextPos = newPos;
                    nextPos = &( newPos->next );
                }

                size++;
            }
        }
    return size;
}


Jogada nextValidPlay(ESTADO* game,Posicao* lastCoords)
{
    Jogada jogada;
    
    resetJogada(&jogada);
    
    // incrementar posicao: se coluna for ultima da linha atual
    if(lastCoords->coluna==7)
    {
        lastCoords->linha++;
        lastCoords->coluna=0;
    }
    else
        lastCoords->coluna++;
    
    // se nova coordenada fora do tabuleiro
    if(
            lastCoords->linha<0  || lastCoords->linha>7 ||
            lastCoords->coluna<0 || lastCoords->coluna>7
      )
        return jogada;
    
    for( lastCoords->linha ; lastCoords->linha < 8 ; lastCoords->linha++ )
        for(lastCoords->coluna ;  ; lastCoords->coluna++)
        {
            resetJogada(&jogada);
            
            jogada = isValidPlay(game,lastCoords);

            if(jogada.isvalid == true)
                return jogada;
            
            if( lastCoords->coluna == 7 )
            {
                lastCoords->coluna = 0;
                break;
            }
        }          
    return jogada;
}



Jogada maxPlay(ESTADO* gameptr)
{
    Jogada jogada;
    Jogada max;   
    Posicao* poslist=NULL;
    Posicao* next=NULL;   
   
    getValidPlays(gameptr,&poslist);
   
    next=poslist;
   
    resetJogada(&max);
    resetJogada(&jogada);

    if( poslist==NULL )
        return jogada; 

    while(1)           
    {                  
        resetJogada(&jogada);

        jogada = isValidPlay(gameptr,next);
   
        if(jogada.soma>max.soma)
            max = jogada;
   
        next = next->next;
   
        if(next==NULL)                       
            break;    
   
    }                 
   
    freeAllPos(poslist);
   
    return max;
}

