#include "atask.h"
#include "io.h"
#include "opto.h"
#include "uart.h"

void debug_print_task(void);

atask_st debug_print_handle        = {"Debug Print    ", 5000,0, 0, 255, 0, 1, debug_print_task};

void setup() {
    delay(2000);
    Serial.begin(9600);


    atask_initialize();
    //atask_add_new(&debug_print_handle);
    io_initialize();
    opto_initialize();
    uart_initialize();


}

void loop() {
  atask_run();  

}


void debug_print_task(void)
{
  atask_print_status(true);
  io_print_all();
}