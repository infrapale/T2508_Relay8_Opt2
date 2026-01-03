#include "io.h"
#include "relay.h"

relay_st relay[NBR_OF_RELAY] =
{
    {OUT_PIN_RELAY_1, LOW},
    {OUT_PIN_RELAY_2, LOW},
    {OUT_PIN_RELAY_3, LOW},
    {OUT_PIN_RELAY_4, LOW},
    {OUT_PIN_RELAY_5, LOW},
    {OUT_PIN_RELAY_6, LOW},
    {OUT_PIN_RELAY_7, LOW},
    {OUT_PIN_RELAY_8, LOW}
};


void relay_on(uint8_t indx)
{
    digitalWrite(relay[indx].pin, HIGH);
    relay[indx].state = HIGH;
} 

void relay_off(uint8_t indx)
{
    digitalWrite(relay[indx].pin, LOW);
    relay[indx].state = LOW;
} 

void relay_toggle(uint8_t indx)
{
    if (relay[indx].state == HIGH)  relay_off(indx);
    else relay_on(indx);
} 

void relay_debug_print(void)
{
    Serial.print("Relay:");
    for(uint8_t i = 0; i < NBR_OF_RELAY; i++)
    {
        Serial.print(" Pin: "); Serial.print(relay[i].pin); 
        Serial.print("=");Serial.print(relay[i].state);
    }
    Serial.println();
}
