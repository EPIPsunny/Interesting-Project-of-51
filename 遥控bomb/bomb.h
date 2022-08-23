#ifndef _BOMB_H
#define _BOMB_H

#include <reg51.h>
#include<intrins.h>

sbit beep=P1^5 ;

void BOMB();
void delay(long int i);



#endif