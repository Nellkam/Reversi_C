#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./headers/data.h"
#include "./headers/posicao.h"
#include "./headers/jogada.h"
#include "./headers/game.h"
#include "./headers/commands.h"
#include "./headers/saveNload.h"
#include "./headers/bot.h"

#define CLEARMYSCREEN (system("cls"))

void clearBuffer()
{
    while(getchar()!='\n');
}


int main()
{
    ESTADO game; // jogo
    Historico* histoptr=NULL; // apontador LIFO historico de estados
    Pontos pontos; // pontos actuais do jogo
    Posicao coords; // cordenadas inseridas pelo humano
    Command comando; // construçao do comando
    int printMode=0; // modo de impressao. 0-normal,1-com pos validas,2-sugestao
    char prompt[52]={'\0'}; // string que recebe comando inserido
    Jogada tempPlay; // jogada temporaria para dificuldade 2 
    VALOR bot=VALOR_O; // inicializacao peca bot
    VALOR turn=VALOR_X; // inicializacao turno inicial
    Posicao* validPosList = NULL, *tempPos=NULL;
    int sizePosList,randPos;
    int saveload; //controla validade de save e load

    srand(time(NULL));
    
    game.difficulty=1;
    resetGame(&game,&histoptr,&pontos,VALOR_X,'A'); // inicializacao do estado 
   
GOTO_INICIO_JOGO:
    CLEARMYSCREEN;

    printTabuleiro(&game,&pontos,printMode);
    printMode=0;
    
    if( isEndgame(&game) )
    {
        if( pontos.player1 > pontos.player2 )
            printf("\n\t\tJOGADOR 'X' SAIU VENCEDOR COM:\n\t\t  %d vs %d pontos\n\n",
                        pontos.player1,pontos.player2);
        else if( pontos.player2 > pontos.player1 )
            printf("\n\t\tJOGADOR 'O' SAIU VENCEDOR COM:\n\t\t  %d vs %d pontos\n\n",
                        pontos.player2,pontos.player1);
        else
            printf("\n\t\tJOGO EMPATADO:\n\t\t  %d vs %d pontos\n\n");

        resetGame(&game,&histoptr,&pontos,VALOR_X,game.modo);
        getchar();
        
        goto GOTO_INICIO_JOGO;
    }
    
    // turno bot
    if( game.peca==bot )
    {
        printf("BOT TURN...ANY KEY TO CONTINUE OR 'U' TO UNDO!\n");
        if( getchar()=='U' )
        {
            clearBuffer();
            goto GOTO_UNDO;
        }
        clearBuffer(); 

        //jogada random
        if( game.difficulty==1 )
        {
            sizePosList = getValidPlays(&game,&validPosList);
            randPos = 1 + rand() % sizePosList;

            tempPos = validPosList;
            for( ; randPos!=1 ; randPos-- )
                tempPos = tempPos->next;
            
            play(&game,tempPos,&pontos,&histoptr);

            validPosList = freeAllPos(validPosList);
            tempPos = NULL;

        }
        // jogada maxima
        else if( game.difficulty==2 )
        {
            tempPlay = maxPlay(&game);
            play(&game,&(tempPlay.coords),&pontos,&histoptr);
        }
        // minimax
        else 
            playBot(&game,&pontos,&histoptr,4);
        
        goto GOTO_INICIO_JOGO;
    }
    
    // obtencao de comando
    printf(" COMMAND LINE:$ ");
    fgets(prompt,51,stdin);
    
    comando = getCommand(prompt);
    
    // avaliacao de comando
    if(comando.isvalid==false)
    {
        printf("#INVALID COMMAND...PRESS TO CONTINUE");
        getchar();
        goto GOTO_INICIO_JOGO;
    }
    
    // executa comando se for valido
    switch(comando.command)
    {
        case 'N': // human vs human
            turn = ( comando.peca=='X' ) ? VALOR_X : VALOR_O;
            bot = VAZIA;
            
            game.difficulty=0;
            resetGame(&game,&histoptr,&pontos,turn,'M');

            goto GOTO_INICIO_JOGO;
        
        case 'L': // load
            saveload = loadGame(&histoptr,&game,comando.filename);
            
            if(!saveload)
            {
                printf("\n!FALHA LEITURA DE JOGO! VERIFIQUE NOME DO FICHEIRO!\n");
                getchar();
                goto GOTO_INICIO_JOGO;
            }
            else
            {
                printf("\n!LOAD COM SUCESSO!\n");
                getchar();
            }

            if(game.difficulty==0)
                bot=VAZIA;
            else
                bot=(game.peca==VALOR_X) ? VALOR_O : VALOR_X;
            
            pontos = getPoints(&game);

            goto GOTO_INICIO_JOGO;
        case 'E': // save
            saveload = saveGame(&histoptr,comando.filename);

            if(!saveload)
            {
                printf("\n!FALHA ESCRITA DE JOGO!\n");
                getchar();
            }
            else
            {
                printf("\n!JOGO GUARDADO!\n");
                getchar();
            }

            goto GOTO_INICIO_JOGO;

        case 'H': // hint
            printMode = 2;
            goto GOTO_INICIO_JOGO;

        case 'A': // vs Bot
            bot = (comando.peca == 'X') ? VALOR_X : VALOR_O;

            game.difficulty = comando.arg2int;
            resetGame(&game,&histoptr,&pontos,VALOR_X,'A');

            goto GOTO_INICIO_JOGO;

        case 'J': // play
            coords.linha  = comando.arg1int - 1;
            coords.coluna = comando.arg2int - 1;

            play(&game,&coords,&pontos,&histoptr);
            goto GOTO_INICIO_JOGO;

        case 'S': // valid plays
            printMode = 1;
            goto GOTO_INICIO_JOGO;

        case 'U': // undo
        GOTO_UNDO:
            popGame(&game,&histoptr);
            pontos = getPoints(&game);
            goto GOTO_INICIO_JOGO;
        
        case 'Q': // quit
            break;

        default:
            printf("\n\nCOMMAND VALIDATION FAIL (MAIN)");
            getchar();
            exit(1);
    }

GOTO_FIM_JOGO: 

    getchar();
    return 0;
}


