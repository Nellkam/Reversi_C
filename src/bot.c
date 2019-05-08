#include "../headers/jogada.h"
#include "../headers/posicao.h"
#include "../headers/game.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/bot.h"


Minimax resetMinimax()
{
    Minimax minmax;

    minmax.coords.linha = minmax.coords.coluna = -1;
    minmax.turno = VAZIA;
    minmax.offset = -64;
    minmax.depth=0;

    return minmax;
}



void resetNode(Node* node)    
{    
    node->depth = 0;
    node->game = NULL;
    node->next  = NULL;    
    node->histoJogadas = NULL;    
}    
    


void freeNode(Node** parent)    
{    
    Node** next = NULL; 
    
    // se Node nao existir 
    if(*parent==NULL)
        return;

    next = &( (*parent)->next );
    
    // free estado alocado
    if( (*parent)->game != NULL )
    {
        free( (*parent)->game );    
        (*parent)->game = NULL;
    }
    
    //free array de pos jogada
    free((*parent)->histoJogadas);
    (*parent)->histoJogadas = NULL;
    
    // free node alocado
    free(*parent);
    *parent = NULL;

    return freeNode(next);
}



void comparePlays(Minimax* max,Node* node)
{
    Pontos points;
    int newOffSet;

    // calculo pontos
    points = getPoints(node->game);
    
    // determinacao do offset
    if( max->turno == VALOR_X)
        newOffSet = points.player1 - points.player2;
    else
        newOffSet = points.player2 - points.player1;
    
    // comparacao
    if(node->depth > max->depth)
    {
        max->coords.linha  = (node->histoJogadas)[0].linha;
        max->coords.coluna = (node->histoJogadas)[0].coluna;
        max->coords.next = NULL;
        max->depth = node->depth;
        max->offset = newOffSet;
    }
    else if ( newOffSet > max->offset )
    {
        max->offset = newOffSet;
        max->coords.linha  = (node->histoJogadas)[0].linha;
        max->coords.coluna = (node->histoJogadas)[0].coluna;
        max->coords.next = NULL;
    }
    
    return;
}


void minimax(Node** parent,Minimax* max,const int maxdepth)
{
    Jogada* jogada = malloc( sizeof(Jogada) );
    Posicao* coords = malloc( sizeof(coords) );
    Node* child = NULL;
    int pos;
    
    // se falhar alocacao memoria
    if(jogada==NULL || coords==NULL)
        exit(1);
    
    if((*parent)->depth==maxdepth)
    {
        free(jogada);
        free(coords);
        freeNode(parent);
        return;
    }

    // reset coordenadas para nextValidPlay()
    coords->linha = 0;
    coords->coluna = -1;
    coords->next = NULL;

    while( 1 )
    {
        resetJogada(jogada);
        *jogada = nextValidPlay( (*parent)->game , coords );
        
        //se nao houverem mais jogadas validas termina este node
        if(jogada->isvalid==false)
            break;

        //se next node nao estiver a null
        if( (*parent)->next!=NULL )
        {
            printf("\n\n***NEXT NODE PODE TER MEMORIA ALOCADA***");
            printf("\n***DETETADO EM MINIMAX***\n");
            exit(1);
        }
        
        //se houverem jogadas: criacao de novo node e recursao
        
        child = NULL;
        child=malloc(sizeof(Node)); // alocacao mem node
        if( child == NULL ) 
            exit(1);
        
        resetNode(child);
        
        child->game=malloc(sizeof(ESTADO)); // alocacao mem estado
        if( child->game == NULL )
            exit(1);
        
        *(child->game) = *((*parent)->game); // copia estado de parent para child
        updateTabuleiro(child->game,jogada); // aplica jogada
        child->depth = (*parent)->depth + 1; // incrementa depth
        child->game->peca = (child->game->peca==VALOR_X) ? VALOR_O : VALOR_X; // turno
        
        //alocacao mem para novo array de pos jogada
        child->histoJogadas = malloc( (child->depth)*(sizeof(Posicao)) );
        
        //copia array de pos de parent para child
        for( pos=0 ; pos<(*parent)->depth ; pos++)
        {
            (child->histoJogadas)[pos].linha  = ((*parent)->histoJogadas)[pos].linha;
            (child->histoJogadas)[pos].coluna = ((*parent)->histoJogadas)[pos].coluna;
            (child->histoJogadas)[pos].next   = NULL;
        }
        
        //atribui nova coordenada
        (child->histoJogadas)[child->depth-1].linha  = jogada->coords.linha;
        (child->histoJogadas)[child->depth-1].coluna = jogada->coords.coluna;       
         
        (*parent)->next = child; // atribui novo nodo a parent
        
        // calculo minmax apos cada jogada do bot
        if((*parent)->game->peca == max->turno)
            comparePlays(max,child); 

        minimax( &((*parent)->next) , max , maxdepth); // recursao
    }
    
    free(jogada);
    free(coords);
    freeNode(parent);
    return;
}





Posicao playBot(
                ESTADO* game,
                Pontos* pontos,
                Historico** histo,
                const int maxdepth
               )
{
    Node* root = malloc(sizeof(Node));
    Minimax minmax;
    
    minmax = resetMinimax();
    minmax.turno = game->peca;

    if(root==NULL)
        exit(1);

    resetNode(root);
    
    root->game=malloc(sizeof(ESTADO));
    if( root->game == NULL )
        exit(1);

    *(root->game) = *game;
    
    minimax(&root,&minmax,maxdepth);

    if(root!=NULL)
    {
        printf("\n\n****ROOT != NULL***");
        printf("\nDETECTADO EM playBot\n\n");
        exit(1);
    }
     
    play(game,&(minmax.coords),pontos,histo);

    return minmax.coords;
}







