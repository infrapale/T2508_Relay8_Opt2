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
    for (uint8_t i = 0; i < NBR_OF_OPTO; i++) {
        opto[i].next_update = UPDATE_INTERVAL + (i *4000);
    }
}


void opto_send_state_msg(opto_st *optop)
{
    Serial.print("<R");
    Serial.print(MAIN_UNIT_INDEX);
    Serial.print('O');
    Serial.print(optop->index);
    Serial.print(':');
    if(optop->status == LOW) Serial.print('H');
    else Serial.print('L');
    Serial.println(">");
    optop->next_update = millis() + UPDATE_INTERVAL;
}


void opto_manage(opto_st *optop)
{
    switch(optop->state)
    {
        case 0:
            optop->state = 10;
            break;
        case 10:
            if(optop->status == LOW) 
            {
                optop->prev_status = optop->status;
                opto_send_state_msg(optop);
                //Serial.println("<R1O1:H>");
                relay_on(0);
                opto_ctrl.radiated = true;
                optop->timeout = millis() + 1000;
                optop->state = 20;
            }
            break;
        case 20:
            if(millis() > optop->timeout) optop->state = 30;
            break;
        case 30:
            if(optop->status == HIGH) 
            {
                opto_send_state_msg(optop);
                //Serial.println("<R1O1:L>");
                relay_off(0);
                opto_ctrl.radiated = true;
                optop->state = 40;
            }
            break;
        case 40:
            optop->state = 10;
            break;

    }
    if (millis() > optop->next_update) opto_send_state_msg(optop);
}

void opto_task(void)
{
    switch(opto_handle.state)
    {
        case 0:
            opto_handle.state = 10;
            opto_ctrl.indx = 0;
            opto_ctrl.radiated = false;
            break;
        case 10:
            opto_manage(&opto[opto_ctrl.indx]);
            if (opto_ctrl.radiated) 
            {
                opto_ctrl.timeout = millis() + 2000;
                opto_handle.state = 20;
            }
            else opto_handle.state = 50;
            break;
        case 20:
            if (millis() > opto_ctrl.timeout)
            {
                opto_ctrl.radiated = false;
                opto_handle.state = 50;
            }
            break;
        case 50:
            opto_ctrl.indx++;
            if(opto_ctrl.indx >= NBR_OF_OPTO) opto_ctrl.indx=0;
            opto_ctrl.timeout = millis() + 5000;
            opto_handle.state = 60;
            break;
        case 60:
            if (millis() > opto_ctrl.timeout){
                opto_handle.state = 10;
            }
            break;
        default:
            Serial.println(F("Lost case in opto_task!!"));
            break;
    }
}
