#include "Arduino.h"
#include "main.h"
#include "io.h"
#include "opto.h"
#include "atask.h"
#include "relay.h"

void io_task(void);
atask_st io_handle          = {"IO Task        ", 1000,0, 0, 255, 0, 1, io_task};

extern opto_st  opto[NBR_OF_OPTO];
extern relay_st relay[NBR_OF_RELAY];
extern main_conf_st main_conf;

void io_initialize(void)
{
    for(uint8_t i=0; i < NBR_OF_OPTO; i++) pinMode(opto[i].pin, INPUT);
    for(uint8_t i=0; i < NBR_OF_RELAY; i++)
    {
        pinMode(relay[i].pin, OUTPUT);
        digitalWrite(relay[i].pin, LOW);
    } 
    
    pinMode(INP_PIN_DIP_SW_1, INPUT);
    pinMode(INP_PIN_DIP_SW_2, INPUT);

    if(analogRead(INP_PIN_DIP_SW_1) < 512) main_conf.my_addr = '1';
    else main_conf.my_addr = '0';
    if(analogRead(INP_PIN_DIP_SW_2) < 512) main_conf.use_watchdog = true; 
    else main_conf.use_watchdog = false;

    atask_add_new(&io_handle);
}

void io_print_all(void)
{
    Serial.print("IO Opto: ");
     for(uint8_t i=0; i < NBR_OF_OPTO; i++) 
    {
        Serial.print(opto[i].status); Serial.print("-");
    } 
    Serial.println();
}

uint8_t io_get_dip_sw(uint8_t sw)
{
    uint8_t ret = 0;
    switch(sw)
    {
        case 1:
            ret = digitalRead(INP_PIN_DIP_SW_1);
            break; 
        case 2:
            ret = digitalRead(INP_PIN_DIP_SW_2);
            break; 
    } 
    if(ret == LOW) ret = 1;else ret = 0;
    return ret;
}
void io_task(void)
{
    for(uint8_t i=0; i < NBR_OF_OPTO; i++) 
    {
        opto[i].status = digitalRead(opto[i].pin);
    } 
}