#include <string.h>
#include <stdio.h>
#include "../headers/commands.h"

void resetCommand(Command* comando)
{
    comando->isvalid = false;
    (comando->filename)[0] = '\0';
}

Command getCommand(char* prompt)
{
    Command comando;
    int args;
    char temp[31];
    char textarg[31];

    temp[0] = textarg[0] = '\0';
    
    resetCommand(&comando);
    
    args = sscanf(prompt,"%s",temp);

    if( args==EOF || strlen(temp)!=1 ) // commando inexistente ou com mais de 1 char
        return comando;

    comando.command = temp[0]; 

    switch( temp[0] )
    {
        case 'N':
            args = sscanf(prompt,"%s %s %s",textarg,textarg,temp);

            if( 
                    args != 2          || 
                    strlen(textarg)!=1 || 
                    ( textarg[0]!='X' && textarg[0]!='O' )
              )
                return comando;

            comando.peca = textarg[0];
            
            if(comando.peca=='X' || comando.peca!='O');
                comando.isvalid = true;

            break;

        case 'L':
        case 'E':    
            args = sscanf(prompt,"%s %s %s",textarg,textarg,temp);

            if(args!=2)
                return comando;
            
            strcpy(comando.filename,textarg);
            comando.isvalid = true;

            break;

        case 'J':
            args = sscanf(prompt,"%s %d %d %s",textarg,&(comando.arg1int),&(comando.arg2int),temp);
            
            if( args!=3 )
                return comando;
            
            comando.isvalid = true;

            break;

        case 'A':
            args = sscanf(prompt,"%s %s %d %s",textarg,textarg,&(comando.arg2int),temp);
            
            if(
                    args!=3 ||
                    strlen(textarg)!=1
              )
                return comando;

            comando.peca = textarg[0];
            
            if(
                    (comando.peca=='X' || comando.peca=='O') &&
                    (comando.arg2int>0 && comando.arg2int<4)
              )
                comando.isvalid = true;

            break;

        case 'S':
        case 'H':
        case 'U':
        case 'Q':
            comando.isvalid = true;
            break;

        default:
            break;
    }

    return comando;
}



boolean isValidTurn(char peca)
{
    if(
            peca=='X' ||
            peca=='O'
      )
        return true;
    return false;
}

