#include <stdlib.h>
#include <stdio.h>
#include "../headers/posicao.h"

Posicao* freeAllPos(Posicao* listptr)    
{                     
    Posicao* next;
                         
    if(listptr==NULL)   
        return NULL;
                           
    next=listptr->next;
    
    do   
    {    
        free(listptr);
        listptr=next;
        
        if(next==NULL)
            break;
                               
        next=listptr->next;
    
    } while(1);      
                                         
    return NULL;
}    



void freeLastPos(Posicao** list)
{
    if(*list==NULL)
        return;

    if( (*list)->next==NULL)
    {
        free(*list);
        *list=NULL;
        return;
    }
    return freeLastPos( &((*list)->next) );
}

     
        
int posListSize(Posicao* listptr)    
{    
    int size=0;          
        
    if(listptr==NULL)    
        return 0;        
        
    while(listptr!=NULL)    
    {        
        size++;    
        listptr=listptr->next;        
    }        
        
    return size;    
}



boolean existePos(Posicao* listptr,Posicao coords)    
{    
    
    if(listptr==NULL)    
        return false;    
    
    do    
    {    
        if(coords.linha==listptr->linha && coords.coluna==listptr->coluna)    
            return true;    
    
        if(listptr->next==NULL)    
            break;

        listptr = listptr->next;

    } while (1);

    return false;
}



// adiciona nova posicao no fim de uma lista de posicoes
void addPos(Posicao** list,Posicao* newPos)
{
    if(newPos==NULL)
        exit(1);

    while(*list!=NULL)
        list = &( (*list)->next );
    
    *list = newPos;
    (*list)->next = NULL;
}
