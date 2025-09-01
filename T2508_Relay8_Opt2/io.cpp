#include "Arduino.h"
#include "io.h"
#include "opto.h"
#include "atask.h"
#include "relay.h"

void io_task(void);
atask_st io_handle          = {"IO Task        ", 1000,0, 0, 255, 0, 1, io_task};

extern opto_st  opto[NBR_OF_OPTO];
extern relay_st relay[NBR_OF_RELAY];

void io_initialize(void)
{
  for(uint8_t i=0; i < NBR_OF_OPTO; i++) pinMode(opto[i].pin, INPUT);
  for(uint8_t i=0; i < NBR_OF_RELAY; i++)
  {
      pinMode(relay[i].pin, OUTPUT);
      digitalWrite(relay[i].pin, LOW);
  } 
    


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

void io_task(void)
{
    for(uint8_t i=0; i < NBR_OF_OPTO; i++) 
    {
        opto[i].status = digitalRead(opto[i].pin);
    } 
}