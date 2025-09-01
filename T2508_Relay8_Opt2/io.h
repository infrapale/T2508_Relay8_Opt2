#ifndef __IO_H__
#define __IO_H__


#define NBR_OF_DIP_SW   2

#define INP_PIN_DIP_SW_1  A7
#define INP_PIN_DIP_SW_2  A6

#define INP_PIN_OPT_1   5u
#define INP_PIN_OPT_2   4u

#define OUT_PIN_RELAY_1 13u
#define OUT_PIN_RELAY_2 12u
#define OUT_PIN_RELAY_3 11u
#define OUT_PIN_RELAY_4 10u
#define OUT_PIN_RELAY_5 9u
#define OUT_PIN_RELAY_6 8u
#define OUT_PIN_RELAY_7 7u
#define OUT_PIN_RELAY_8 6u

void io_initialize(void);

void io_print_all(void);

#endif