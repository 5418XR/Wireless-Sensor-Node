//#include <avr/power.h>
//#include <avr/wdt.h>
//#include <avr/sleep.h>

void enterSleep(void){
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);//slect the mode
  MCUSR &= ~(1 << WDRF);              //flag
  WDTCSR |= (1<<WDCE) | (1<<WDE);     //enable configuration changes
  WDTCSR = (1<<WDP0) | (1<<WDP3);     //set 8s timer
  WDTCSR |= (1<<WDIE);                //enagble interrupt
  sleeping();
  //sleep_enable();                     //Enable the sleep 
  //sleep_mode();                       //triger the sleep
  //time pass
  //sleep_disable();                    //wakeup
}


void sleeping(){
  ADCSRA &= ~(1<<7); //disable ADC
  SMCR |= (1<<2);    //power down 
  SMCR |= 1;         // enable sleep
 //BOD diabled need befoe the asambler sleep
  MCUCR |=(3<<5);     //set the BODS and BODSE at the same time
  MCUCR = (MCUCR & ~(1<<5)) | (1<<6);//then sete them clear at the same time
  __asm__ __volatile__("sleep");//asambler sleep

}