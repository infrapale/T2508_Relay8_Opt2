#include "main.h"
#include "uart.h"
#include "opto.h"
#include "relay.h"
#include "atask.h"

typedef struct
{
    char  rx_buff[UART_BUFF_LEN];
    char  tx_buff[UART_BUFF_LEN]; 
    uart_msg_st rx_decoded;  
    uart_msg_st tx_decoded;  
    uint8_t rx_len;
} uart_ctrl_st;

extern main_conf_st main_conf;

void uart_rx_task(void);
//atask_st rx_handle        = {"Opto Task      ", 100,0, 0, 255, 0, 1, opto_task};
atask_st rx_handle          = {"UART Rx Task   ", 100,0, 0, 255, 0, 1, uart_rx_task};
uart_ctrl_st uart_ctrl;

void uart_initialize(void)
{
    atask_add_new(&rx_handle);
}

uint8_t uart_read_line(char *buff)
{
    uint8_t len = 0;
    if (Serial.available())
    {
        String Str;
        Str  = Serial.readStringUntil('\n');
        //Serial.println(Str);
        if (Str.length()> 0)
        {
            Str.trim();
            len = Str.length();
            Str.toCharArray(buff, UART_BUFF_LEN);
        }
    } 
    return len;
}


void uart_build_decoded_msg(char *buff, uart_msg_st *decod)
{
    buff[0] = '<';
    buff[1] = decod->to_tag;
    buff[2] = decod->to_addr;
    buff[3] = decod->from_tag;
    buff[4] = decod->from_addr;
    buff[5] = decod->function;
    buff[6] = decod->func_indx;
    buff[7] = decod->action;
    buff[8] = decod->value;
    buff[9] = '>';
    buff[10] = '\r';
    buff[11] = '\n';
    buff[12] = 0x00;
}

bool uart_get_decode_msg(char *buff, uart_msg_st *decod)
{
    bool frame_ok = false;
    if((buff[0] == '<')  && (buff[9] == '>'))
    {
        decod->to_tag     = buff[1];
        decod->to_addr    = buff[2];
        decod->from_tag   = buff[3];
        decod->from_addr  = buff[4];
        decod->function   = buff[5];
        decod->func_indx  = buff[6];
        decod->action     = buff[7];
        decod->value      = buff[8];
        frame_ok = true;
    }
    if((decod->to_tag != main_conf.my_tag) || (decod->to_addr !=main_conf.my_addr))
        frame_ok = false;
    return frame_ok;
}

void uart_print_decoded(uart_msg_st *decod)
{
    Serial.print(decod->to_tag);
    Serial.print(decod->to_addr);
    Serial.print(decod->from_tag);
    Serial.print(decod->from_addr);
    Serial.print(decod->function);
    Serial.print(decod->func_indx);
    Serial.print(decod->action);
    Serial.print(decod->value);
    Serial.println();
}

uart_reply_opto_state(uart_msg_st *decod)
{
    uint8_t opto_status = 0;
    for(uint8_t i=0; i< NBR_OF_OPTO; i++){
       opto_status |= (opto_get_inp_event(i) << i);
    }

    uart_ctrl.tx_decoded.to_tag = decod->from_tag;
    uart_ctrl.tx_decoded.to_addr = decod->from_addr;
    uart_ctrl.tx_decoded.from_tag = decod->to_tag;
    uart_ctrl.tx_decoded.from_addr = decod->to_addr;
    uart_ctrl.tx_decoded.function = decod->function;
    uart_ctrl.tx_decoded.func_indx = decod->func_indx;
    uart_ctrl.tx_decoded.action = ACTION_REPLY;
    uart_ctrl.tx_decoded.value = opto_status + '0';
    uart_build_decoded_msg(uart_ctrl.tx_buff, &uart_ctrl.tx_decoded);
    Serial.println(uart_ctrl.tx_buff);   
}

void uart_action_set_relay(uart_msg_st *decod)
{
    uint8_t rindx = decod->func_indx - '1';
    if (rindx < 8) {
        if (decod->action == ACTION_SET) {
            switch(decod->value )
            {
                case '0':
                    relay_off(rindx);
                    break;
                case '1':
                    relay_on(rindx);
                    break;
                case 'T':
                    relay_toggle(rindx);
                    break;
            }
        }
    }
    relay_debug_print();

}

void uart_rx_action(uart_msg_st *decod)
{
    switch(decod->function)
    {
        case 'O':
            if(decod->func_indx == '*') uart_reply_opto_state(decod);
            break;
        case 'S': // Set relay
            uart_action_set_relay(decod);
            break;
    }
}

void uart_rx_task(void)
{
    switch(rx_handle.state)
    {
        case 0:
            rx_handle.state = 10;
            break;
        case 10:
            uart_ctrl.rx_len = uart_read_line(uart_ctrl.rx_buff);
            if(uart_ctrl.rx_len > 8) rx_handle.state = 20;
            break;
        case 20:
            main_conf.clr_watchdog = true;
            if (uart_get_decode_msg(uart_ctrl.rx_buff, &uart_ctrl.rx_decoded))
            {
                // Serial.print("uart_rx_task, decoded: ");
                // uart_print_decoded( &uart_ctrl.rx_decoded);
                uart_rx_action(&uart_ctrl.rx_decoded);
            }
            rx_handle.state = 10;
            break;
        case 100:
            rx_handle.state = 10;
            break;
     }
}