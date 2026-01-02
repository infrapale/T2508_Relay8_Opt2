#ifndef __UART_H__
#define __UART_H__

#define   UART_BUFF_LEN     40
#include  "Arduino.h"
#include  "main.h"


typedef struct
{
    char to_tag;
    char to_addr;
    char from_tag;
    char from_addr;
    char function;
    char func_indx;
    char action;
    char value;
} uart_msg_st;

void uart_initialize(void);

uint8_t uart_read_uart(char *buff);

#endif