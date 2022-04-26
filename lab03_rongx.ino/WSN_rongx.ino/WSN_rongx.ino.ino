// instiallizing the the loading library 
//#include <avr/power.h>
//#include <avr/wdt.h>
//#include <avr/sleep.h>

//instializing the input pins
int HzOutPut =6;
double inputV = A3;
double BatteryV = A1;// not yet coding
double chargein = A2;// not yet coding

//this is the gloable values for the DC/DC converter
double boostVout = 0;
double dutysycle = 0;

//double RESISTER_VALUE_ONE = 30;
//double RESISTER_VALUE_TWO = 120;

//dutysycle instialize value
int DutyCycle = 50;

//DC intialize and charge target
double  setCharge = 9.2;
double  outDC= 0;
double  DCtemp = 0;

//timer
unsigned long myTime1;
unsigned long myTime2;

//Bettery voltage detect
double realBatteryV =0;


//voltage input
double  Vin = 0;

//inistalize stage
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

//temppreture defultes-------------------------------------------
#define RT0 10000   // 10000 ohms
#define B 3900      //Constant taken from data sheet
#define Vs 5    //Supply voltage
#define R 10000  //Resistance of thermistor is 10000 ohms

//defining variables
float RT, VR, lnRfrac, temp, T0, ThermistorOut;
//----------------------------------------------------------------



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

//----------------------------------------
  //conversion of room temp from C to K
  T0 = 25 + 273.15;
//--------------------------------------------
}
// main part
void loop() {
    f_wdt = 0;
  //updateSerial();
  //get the read from A3
  inputV = analogRead(A3);
  BatteryV = analogRead(A1);
  chargein = analogRead(A2);
  //


//--------------------------------------------
  // put your main code here, to run repeatedly: 

  
  //Reading value from thermistor
  ThermistorOut = analogRead(A0);    
  //Converting value to voltage
  //1023 is max value of ADC          
  ThermistorOut = (5.00 / 1023.00) * ThermistorOut;   
  //Obtaining necessary values for VR and RT   
  VR = Vs - ThermistorOut;
  RT = ThermistorOut / (VR / R); 

  //Taking ln of r/r0             
  lnRfrac = log(RT / RT0);

  //using overall equation to get obtain temperature in kelvin
  temp = (1 / ((lnRfrac / B) + (1 / T0))); 
  //converting from K to C
  temp = temp - 273.15;
  //Converting from C to F 
  temp = (temp*1.8) + 32; 

  //Printing Values to Serial Monitor
  Serial.print("Temperature: "); 
  Serial.print(temp);
  Serial.print(" F"); 
  Serial.println(' '); 
  //adding a delay so that values are displayed at a readable speed
  delay(500);
//---------------------------------------------------------
  
  
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
