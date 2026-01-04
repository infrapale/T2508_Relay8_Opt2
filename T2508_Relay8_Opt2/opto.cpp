    //rfm_send_msg_st *rx_msg = &send_msg; 
    // Opto:  <R1O2:L>
    //         \\\\\\______ value, message (optional)
    //          \\\\\______ action set: '=' get: '?' reply: ':'
    //           \\\\______ index
    //            \\\______ function
    //             \\______ module_addr
    //              \______ module_tag


#include "Arduino.h"
#include "main.h"
#include "io.h"
#include "opto.h"
#include "atask.h"
#include "relay.h"



opto_st  opto[NBR_OF_OPTO] = 
{
    {1,INP_PIN_OPT_1, 0, 0, 0, 0, 0},
    {2,INP_PIN_OPT_2, 0, 0, 0, 0, 0}
};

typedef struct
{
    uint8_t     indx;
    bool        radiated;
    uint32_t    timeout;
} opto_ctrl_st; 

void opto_task(void);
atask_st opto_handle          = {"Opto Task      ", 100,0, 0, 255, 0, 1, opto_task};

opto_ctrl_st opto_ctrl = {0, false, 0};

void opto_initialize(void)
{
    atask_add_new(&opto_handle);
}


uint8_t opto_get_inp_event(uint8_t oindx)
{   
    uint8_t ret = opto[oindx].event;
    opto[oindx].event = 0;
    return ret;
}

void opto_task(void)
{
    uint8_t oindx= 0;
    switch(opto_handle.state)
    {
        case 0:
            opto_handle.state = 10;
            opto_ctrl.indx = 0;
            opto_ctrl.radiated = false;
            break;
        case 10:
            for(oindx =0; oindx < NBR_OF_OPTO; oindx++){
                if(opto[oindx].status == LOW) opto[oindx].event = 1;
            }
            break;
        default:
            Serial.println(F("Lost case in opto_task!!"));
            break;
    }
}
