#include "io.h"
#include "relay.h"

relay_st relay[NBR_OF_RELAY] =
{
    {OUT_PIN_RELAY_1},
    {OUT_PIN_RELAY_2},
    {OUT_PIN_RELAY_3},
    {OUT_PIN_RELAY_4},
    {OUT_PIN_RELAY_5},
    {OUT_PIN_RELAY_6},
    {OUT_PIN_RELAY_7},
    {OUT_PIN_RELAY_8}
};


void relay_on(uint8_t indx)
{
    digitalWrite(relay[indx].pin, HIGH);
} 

void relay_off(uint8_t indx)
{
    digitalWrite(relay[indx].pin, LOW);
} 
