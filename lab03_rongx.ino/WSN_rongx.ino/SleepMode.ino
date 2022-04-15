#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

void enterSleep(void){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//slect the mode
  MCUSR &= ~(1 << WDRF);              //flag
  WDTCSR |= (1<<WDCE) | (1<<WDE);     //enable configuration changes
  WDTCSR = (1<<WDP0) | (1<<WDP3);     //set 8s timer
  WDTCSR |= (1<<WDIE);                //enagble interrupt
  sleep_enable();                     //Enable the sleep 
  sleep_mode();                       //triger the sleep
  //time pass
  sleep_disable();                    //wakeup
}