#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/jogada.h"
#include "../headers/posicao.h"
#include "../headers/saveNload.h"


// historico -> implementacao LIFO    
void pushGame(ESTADO* gameptr, Historico** listptr)    
{    
    int ln,col;    
    Historico* ptr=NULL;    
    
    ptr = malloc(sizeof(Historico));    
        
    if(*listptr==NULL)    
    {    
        *listptr=ptr;    
        ptr->previous = NULL;    
    }    
    
    else    
    {    
        ptr->previous = *listptr;    
        *listptr=ptr;    
    }    
        
        
    (ptr->game).peca = gameptr->peca;    
    (ptr->game).modo = gameptr->modo;    
    (ptr->game).difficulty = gameptr->difficulty;

     for(ln=0 ; ln<8 ; ln++)    
         for(col=0 ; col<8 ; col++)    
             (ptr->game).grelha[ln][col] = gameptr->grelha[ln][col];    
}    


// pop de uma lista LIFO    
void popGame(ESTADO* gameptr, Historico** listptr)    
{    
    int ln,col;    
    Historico* ptr=NULL;    
        
    if((*listptr)->previous==NULL)    
        return;    
    
    ptr = (*listptr)->previous;    
    free(*listptr);    
    *listptr=ptr;    
    
    *gameptr = (*listptr)->game;        
}  



int sizeHistorico(Historico** listptr)
{
    if(*listptr==NULL)
        return 0;
    return 1 + sizeHistorico( &((*listptr)->previous) );
}


void startHistorico(Historico** histoptr,char modo,VALOR turn,int difficulty)
{
    int ln,col;

    *histoptr=malloc(sizeof(Historico));

    // reset tabuleiro e estado
    for(ln=0 ; ln<8 ; ln++)
        for(col=0 ; col<8 ; col++)
            (*histoptr)->game.grelha[ln][col] = VAZIA;

    (*histoptr)->game.grelha[3][3] = (*histoptr)->game.grelha[4][4] = VALOR_O;    
    (*histoptr)->game.grelha[3][4] = (*histoptr)->game.grelha[4][3] = VALOR_X;

    (*histoptr)->game.peca = turn;
    (*histoptr)->game.modo = modo;
    (*histoptr)->game.difficulty = difficulty;

}


Historico* freeAllHistorico(Historico* histoptr)
{
    Historico* previous;
    
    if(histoptr==NULL)
        return NULL;
    
    previous = histoptr->previous;
    while(1)
    {
        free(histoptr);
        
        if(previous==NULL)
            break;;

        histoptr = previous;
        previous = histoptr->previous;

    }

    return NULL;
}



//--------------------------------------
//--------------------------------------


int saveGame(Historico** histoptr,char* filename)
{
    Posicao coords;
    char peca;
    FILE* myfile=NULL;
    char finalName[61];
    
    finalName[0] = '\0';

    strcat(finalName,"../SavedGames/");
    strcat(finalName,filename);
    strcat(finalName,".txt");
    
    myfile = fopen(finalName,"w");
    
    if(myfile==NULL)
        return 0;
    
    while(*histoptr!=NULL)
    {
        peca = ( ((*histoptr)->game).peca == VALOR_X ) ? 'X' : 'O'; 

        fprintf(myfile,"%c %c",((*histoptr)->game).modo,peca);
        if( (*histoptr)->game.difficulty !=0 )
            fprintf( myfile, " %d\n", (*histoptr)->game.difficulty );
        else
            fprintf(myfile,"\n");

        for(coords.linha=0 ; coords.linha<8 ; coords.linha++)
        {
            for(coords.coluna=0 ; coords.coluna<8; coords.coluna++)
            {
                if ( ((*histoptr)->game).grelha[coords.linha][coords.coluna] == VALOR_X )
                    peca = 'X';
                else if (  ((*histoptr)->game).grelha[coords.linha][coords.coluna] == VALOR_O  )
                    peca = 'O';
                else
                    peca = '-';

                fprintf(myfile,"%c ",peca);
            }

            fprintf(myfile,"\n");
        }
            fprintf(myfile,"\n");       

        histoptr = & ( (*histoptr)->previous );

    }
    
    fclose(myfile);

    return 1;
}





int loadGame(Historico** histoptr,ESTADO* gameptr,char* filename)
{
    int ln,col;
    char ch;
    FILE* myfile=NULL;
    VALOR peca;
    Historico* temp=NULL, *currentHisto=NULL;
    char finalName[61];

    finalName[0] = '\0';
    
    strcat(finalName,"../SavedGames/");
    strcat(finalName,filename);
    strcat(finalName,".txt");

    myfile = fopen(finalName,"r");
    
    if(myfile == NULL)
        return 0;

    *histoptr = freeAllHistorico(*histoptr);

    ch=fgetc(myfile); // capta modo
    while(1)
    {
        // setup nova estrutura
        temp = malloc(sizeof(Historico));
        
        if(*histoptr==NULL)
        {
            *histoptr = temp;
            currentHisto = temp;
        }
        else
        {
            currentHisto->previous = temp;
            currentHisto = temp;
        }
        
        currentHisto->previous = NULL;
        // fim setup

        currentHisto->game.modo = ch;

        fgetc(myfile); // espaco entre modo e peca;
        ch = fgetc(myfile); // peca

        currentHisto->game.peca = (ch=='X') ? VALOR_X : VALOR_O;
      
        ch = fgetc(myfile); // le enter ou espaço
        if(ch=='\n')
            currentHisto->game.difficulty = 0;
        else
        {
            ch = fgetc(myfile); // le numero
            currentHisto->game.difficulty = (char) ch - 48; // conversao para num
            fgetc(myfile);// consome \n
        }

        //inicio leitura tabuleiro
        for(ln=0 ; ln<8 ; ln++)
        {
            for(col=0 ; col<8 ; col++)
            {
                ch = fgetc(myfile); // char
                fgetc(myfile); // espaco entre chars

                switch(ch)
                {
                    case '-':
                        peca = VAZIA;
                        break;
                    case 'X':
                        peca = VALOR_X;
                        break;
                    case 'O':
                        peca = VALOR_O;
                        break;
                }
                currentHisto->game.grelha[ln][col] = peca;
            }
            fgetc(myfile); // \n fim linha
        }
        //fim leitura tabuleiro
        
        while(
                (ch=fgetc(myfile)) != 'A' &&
                ch != 'M' &&
                ch != EOF
             );

            if(ch==EOF)
                break;
    }

    fclose(myfile);
    
    *gameptr = (*histoptr)->game;    
    
    return 1;
}


