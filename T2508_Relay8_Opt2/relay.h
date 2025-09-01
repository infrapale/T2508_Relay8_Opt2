#ifndef __RELAY_H__
#define __RELAY_H__
#include "Arduino.h"

#define NBR_OF_RELAY  8

typedef struct 
{
    uint8_t pin;
} relay_st;

void relay_on(uint8_t indx);

void relay_off(uint8_t indx);

#endif