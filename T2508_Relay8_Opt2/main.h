#ifndef __MAIN_H__
#define __MAIN_H__

#define UNIT_TAG            'Y'
#define UNIT_ADDR           '0'
//#define UPDATE_INTERVAL     10000
#define OPTO_FUNCTION       'O'
#define RELAY_FUNCTION      'R'
#define ACTION_SET          '='
#define ACTION_GET          '?'
#define ACTION_REPLY        ':'
#define WILD_CHAR           '*'

typedef struct 
{
    char my_tag;
    char my_addr;
    bool use_watchdog;
    bool clr_watchdog;
} main_conf_st;
#endif