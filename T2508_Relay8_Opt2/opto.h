#ifndef __OPTO_H__
#define __OPTO_H__

#define NBR_OF_OPTO     2

typedef struct
{
    uint8_t     index;
    uint8_t     pin;
    uint8_t     status;
    uint8_t     event;
    uint16_t    state;
    uint32_t    timeout;
    uint32_t    next_update;
} opto_st;

void opto_initialize(void);

uint8_t opto_get_inp_event(uint8_t oindx);
#endif
