#include "../headers/posicao.h"
#include "../headers/saveNload.h"
#include "../headers/jogada.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/game.h"


void resetTabuleiro(ESTADO* gameptr)
{        
    int ln,col;

    for(ln=0 ; ln<8 ; ln++)
        for(col=0 ; col<8 ; col++)
            gameptr->grelha[ln][col] = VAZIA;

    gameptr->grelha[3][3] = gameptr->grelha[4][4] = VALOR_O;    
    gameptr->grelha[3][4] = gameptr->grelha[4][3] = VALOR_X;

}


void resetPoints(Pontos* pontos)
{
    pontos->player1 = pontos->player2 = 2;
}


void resetGame(
                ESTADO* gameptr, 
                Historico** histoptr, 
                Pontos* pontosptr,
                VALOR turn,
                char mode
              )
{
    resetTabuleiro(gameptr);
    gameptr->peca=turn;
    gameptr->modo=mode;

    resetPoints(pontosptr);
    *histoptr = freeAllHistorico(*histoptr);
    pushGame(gameptr,histoptr);
}


Pontos getPoints(ESTADO* game)
{
    Pontos points;
    int ln,col;

    
    points.player1 = 0;
    points.player2 = 0;

    for( ln=0 ; ln<8 ; ln++ )
        for( col=0 ; col<8 ; col++ )
        {
            if ( game->grelha[ln][col] == VALOR_X )
                points.player1++;
            else if ( game->grelha[ln][col] == VALOR_O )
                points.player2++;
        }
    
    return points;
}


void changeMode(ESTADO* gameptr)
{
    gameptr->modo = (gameptr->modo=='A') ? 'M' : 'A';
}

void updateTabuByDir(ESTADO* game,Posicao* coords,const int total,const int x,const int y)
{
    int l,c;
    int contador=0;

    l = coords->linha;
    c = coords->coluna,
    l += x;
    c += y;

    for( ; contador<total ; l+=x,c+=y,contador++ )
        game->grelha[l][c] = game->peca;

    if(total>0)
    {
        game->grelha[coords->linha][coords->coluna] = game->peca;
        game->grelha[l][c] = game->peca;
    }
}

void updateTabuleiro(ESTADO* game, Jogada* jogadaptr) 
{

    if(jogadaptr->isvalid==false)
        return;
    
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->cima ,-1, 0);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->baixo, 1, 0);   
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->esq  , 0,-1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->dir  , 0, 1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->ce   ,-1,-1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->cd   ,-1, 1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->be   , 1,-1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->bd   , 1, 1);
}


void updatePoints(Jogada* jogadaptr, Pontos* pontosptr)
{
    if(jogadaptr->turn==VALOR_X)
    {
        pontosptr->player1 += 1 + jogadaptr->soma; // player1=VALOR_X
        pontosptr->player2 -= jogadaptr->soma; // player2=VALOR_O
    }

    else
    {
        pontosptr->player2 += 1 + jogadaptr->soma;
        pontosptr->player1 -= jogadaptr->soma;

    }
}


void nextTurn(ESTADO* gameptr)
{
    int size=0;
    VALOR next;
    Posicao* listptr = NULL;

    next = (gameptr->peca==VALOR_X) ? VALOR_O : VALOR_X;
    gameptr->peca = next;

    size=getValidPlays(gameptr,&listptr);

    if(!size)
       gameptr->peca = (next==VALOR_X) ? VALOR_O : VALOR_X;     

    freeAllPos(listptr);    
}


boolean isEndgame(ESTADO* game)
{
    Posicao* poslist = NULL;
    int size1,size2;

    size1 = getValidPlays(game,&poslist);
    poslist = freeAllPos(poslist);

    game->peca = (game->peca==VALOR_X) ? VALOR_O : VALOR_X;
    size2 = getValidPlays(game,&poslist);
    poslist = freeAllPos(poslist);

    game->peca = (game->peca==VALOR_X) ? VALOR_O : VALOR_X;
    
    if( !size1 && !size2 )
        return true;
    return false;
}



// modo=1 imprime . (pontos) jogadas validas
void printTabuleiro(ESTADO* gameptr,Pontos* pontos,int modo)
{
    int linhaReal=0;
    Posicao coords;
    Posicao *listptr=NULL;
    Jogada max;

    coords.linha = coords.coluna = 0;
    
    printf("\n         1       2       3       4       5       6       7       8\n");
    
    if(modo==1)
        getValidPlays(gameptr,&listptr);
    if(modo==2)
        max = maxPlay(gameptr);

    for(coords.linha=0 ; coords.linha<8 ; coords.linha++)
    {         
        printf("     -----------------------------------------------------------------");
        linhaReal++;

        if(linhaReal==1 || linhaReal==5 || linhaReal==9 || linhaReal==33)
            printf(" #######################");
        if(linhaReal==13)
            printf("                       #");
        
        if(linhaReal==17)
            printf(" L <filename> (load)   #");
        else if(linhaReal==21)
            printf(" H (hint)              #");
        else if(linhaReal==25)
            printf(" J <l> <c> (jogar)     #");
        else if(linhaReal==29)
            printf(" U (undo)              #");

        printf("\n");
        printf("     |       |       |       |       |       |       |       |       |");
        linhaReal++;
        
        if(linhaReal==2)
            printf("\t     -> SCORE <-     #");
        else if(
                linhaReal==6  || linhaReal==10 || linhaReal==14 ||
                linhaReal==18 || linhaReal==22 || linhaReal==26 ||
                linhaReal==30
               )
            printf("                       #");

        printf("\n");
        linhaReal++;
        for(coords.coluna=0 ; coords.coluna<8 ; coords.coluna++)
        {
            if(coords.coluna==0)
                printf("  %d  ",coords.linha+1);
            
            if(modo==1 && existePos(listptr,coords)==true)
            {
                printf("|   .   "); // jogada valida 
                continue;
            }

            if(modo==2 && max.coords.linha==coords.linha && max.coords.coluna==coords.coluna)
            {
                printf("|   ?   "); // hint
                continue;
            }

            if(gameptr->grelha[coords.linha][coords.coluna]==VALOR_X)
                printf("|   X   ");
            else if(gameptr->grelha[coords.linha][coords.coluna]==VALOR_O)
                printf("|   O   ");
            else
                printf("|       "); // casa vazia


        }
        
        printf("|");

        if(linhaReal==3)
            printf("                       #");
        else if(linhaReal==7)
        {
            if(gameptr->peca==VALOR_X && gameptr->difficulty==0)
                printf("\tTurno: X             #");
            else if (gameptr->peca==VALOR_O && gameptr->difficulty==0)
                printf("\tTurno: O             #");
            else if(gameptr->peca==VALOR_X)
                printf("\tTurno: X | Lvl: %d    #",gameptr->difficulty);
            else
                printf("\tTurno: O | Lvl: %d    #",gameptr->difficulty);

        }
        else if(linhaReal==11)
            printf("\t   -> COMMANDS <-    #");
        else if(linhaReal==15)
            printf(" N <peca> (1vs1)       #");
        else if(linhaReal==19)
            printf(" E <filename> (save)   #");
        else if(linhaReal==23)
            printf(" A <peca> <nivel> (bot)#");
        else if(linhaReal==27)
            printf(" S (jogadas possiveis) #");
        else if(linhaReal==31)
            printf(" Q (quit)              #");

        printf("\n");
        linhaReal++;
        printf("     |       |       |       |       |       |       |       |       |");
        
        if(linhaReal==4)
        {
            printf("\t  X: %d   |   O: %d",pontos->player1,pontos->player2);
            if(pontos->player1>=10 && pontos->player2>=10)
                printf("  #");
            else if(pontos->player1>=10 || pontos->player2>=10)
                printf("   #");
            else
                printf("    #");
        }
        else if (
                    linhaReal==8  || linhaReal==12 || linhaReal==16 ||
                    linhaReal==20 || linhaReal==24 || linhaReal==28 ||
                    linhaReal==32
                )
            printf("                       #");

        printf("\n");
    }
        
    printf("     -----------------------------------------------------------------");
    linhaReal++;

    if(linhaReal==33)
        printf(" #######################");
    
    printf("\n\n");
    freeAllPos(listptr);
}



void copyTabuleiro(ESTADO* dest, ESTADO* source)
{
    int ln,col;

    for(ln=0 ; ln<8 ; ln++)
        for(col=0 ; col<8 ; col++)
            dest->grelha[ln][col] =  source->grelha[ln][col];
}



boolean play(
                ESTADO* gameptr,
                Posicao* coords,
                Pontos* pontosptr,
                Historico** histoptr
            )
{
    Jogada jogada;

    jogada=isValidPlay(gameptr,coords);

    if(jogada.isvalid==false)
        return false;

    updateTabuleiro(gameptr,&jogada);

    updatePoints(&jogada,pontosptr);
    
    nextTurn(gameptr);
    
    pushGame(gameptr,histoptr);

    return true;
}
