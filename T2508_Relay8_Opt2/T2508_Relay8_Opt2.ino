#include "atask.h"
#include "io.h"
#include "opto.h"
#include "uart.h"
#include "avr_watchdog.h"

void debug_print_task(void);

main_conf_st main_conf = {
    .my_tag = UNIT_TAG, 
    .my_addr = UNIT_ADDR,
    .use_watchdog = false,
    .clr_watchdog = true};

atask_st debug_print_handle        = {"Debug Print    ", 5000,0, 0, 255, 0, 1, debug_print_task};
AVR_Watchdog watchdog(6);

void setup() {
    delay(2000);
    Serial.begin(9600);


    atask_initialize();
    //atask_add_new(&debug_print_handle);
    io_initialize();
    opto_initialize();
    uart_initialize();
    Serial.print("I am: "); Serial.print(main_conf.my_tag); Serial.println(main_conf.my_addr);
    if(main_conf.use_watchdog){
        watchdog.set_timeout(10);
        Serial.println(F("Watchdog is Activated!"));
    } 
}


void loop() {
  atask_run();  

  if( main_conf.clr_watchdog)
  {
      watchdog.clear();
      main_conf.clr_watchdog =  false;
  }
}


void debug_print_task(void)
{
  atask_print_status(true);
  io_print_all();
}