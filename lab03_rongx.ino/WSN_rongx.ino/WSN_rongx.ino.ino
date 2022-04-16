#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

int HzOutPut =6;
double inputV = A0;
double BatteryV = A1;// not yet coding
double chargein = A2;// not yet coding

double boostVout = 0;
double dutysycle = 0;

double RESISTER_VALUE_ONE = 30;
double RESISTER_VALUE_TWO = 120;

int DutyCycle = 50;

double  setCharge = 9.2;
double  outDC= 0;
double  DCtemp = 0;

//timer
unsigned long myTime1;
unsigned long myTime2;

//Vb
double realBatteryV =0;


//vin 
double  Vin = 0;

//int stage
int STAGE = 0;
int STAGE1 = 1;
int STAGE2 = 2;
int STAGE3 = 3;
int STAGE4 = 4;

//clock
double clocknumber = 0;

//int defult
int ZERO=0;

//WDT
volatile int f_wdt=0;

void setup() {
  //Serial.begin(9600);
  initBluetooth();

  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
TCCR0B = TCCR0B &11111000 | B00000001;//62kHz
pinMode(HzOutPut, outDC);
myTime2 = myTime1;
}
// main part
void loop() {
    f_wdt = 0;
  //updateSerial();
  //get the read from A0
  inputV = analogRead(A0);
  BatteryV = analogRead(A1);
  chargein = analogRead(A2);
  //
  
  
//  double  DCtemp = outDC;
//  outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);
//  realBatteryV = BatteryV*0.0045*6.25*0.37;
  if (STAGE == 1){
      realBatteryV = BatteryV*0.0045*6.25*0.32;
      analogWrite(HzOutPut, ZERO);
      //digitalWrite(5, HIGH); // 5 to pmose
      clocknumber = 300000;
      //analogWrite(HzOutPut, ZERO);
  }
  if (STAGE == 2 || STAGE == 0){
      double  DCtemp = outDC;
      outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);
      realBatteryV = BatteryV*0.0045*6.25*0.32;
      analogWrite(HzOutPut, outDC);
      //digitalWrite(5, LOW);
      clocknumber = 300000;
  }
  if (STAGE == 3){
    realBatteryV = BatteryV*0.0045*6.25*0.32;
//    ADCSRA &= ~(1<<ADEN);  // adc off
//    analogWrite(HzOutPut, ZERO);
//    delay(100);
//    clocknumber = 300000;
    //ADCSRA |= (1<<ADEN);
//    if (realBatteryV>0){
//      f_wdt = 0;
//    }
      if(f_wdt == 0)
  {
    enterSleep(); 
  }
  f_wdt = 0;
  STAGE = 2;
  }
  if (STAGE == 4){
    realBatteryV = BatteryV*0.0045*6.25*0.32;
    //ADCSRA &= ~(1<<ADEN);
        //delay(10000000);
    //analogWrite(HzOutPut, ZERO);
    clocknumber = 300000;
        Serial.println("Sleeping...");
        delay(100);
        //f_wdt=0;
              if(f_wdt == 0)
  {
    enterSleep(); 
    enterSleep(); 
    enterSleep(); 
  } 
    f_wdt = 0;
    STAGE = 2;
       // ADCSRA |= (1<<ADEN); 
  }
  //analogWrite(HzOutPut, outDC);

    myTime1 = millis();
  //if(myTime1 - myTime2>= 300000*6.2){
  //if(myTime1 - myTime2>= 30000){
  if(myTime1 - myTime2>= clocknumber){
    myTime2 = myTime1;
    Serial.print("boostVin:");
    Serial.println(boostVout);
    Serial.print("dutycycle:");
    Serial.println(dutysycle);
    Serial.print("baterry:");
    Serial.println(realBatteryV);
    Serial.print("Vin:");
    Serial.println(chargein);

  if (chargein>1 && realBatteryV>9.2){
    STAGE = STAGE1;
  }
  if (chargein>1 && realBatteryV<9.2){
    STAGE = STAGE2;
  }
  if (chargein<1 && realBatteryV>7){
    STAGE = STAGE3;
  }
  if (chargein<1 && realBatteryV<7){
    STAGE = STAGE4;
  }
    Serial.print("Stage:");
    Serial.println(STAGE);
    //Serial.println(realBatteryV);
  }
  
}

double DutyCycleCauculation(double inputV, double setCharge, double preDC){
  double realVin = (inputV*0.0045)*5;
  
  boostVout = realVin;


  double out = preDC;
  if (realVin< setCharge && out<1000){
    out++;
  }
 else if(realVin > setCharge && out>0){
  out--; 
 }
 dutysycle = out;

  return out;
}

//interrupt
ISR(WDT_vect)
{
  //realBatteryV = BatteryV*0.0045*6.25*0.32;
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
}
