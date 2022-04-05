
int HzOutPut =6;
double inputV = A0;
double BatteryV = A1;// not yet coding

double boostVout = 0;
double dutysycle = 0;

double RESISTER_VALUE_ONE = 30;
double RESISTER_VALUE_TWO = 120;

int DutyCycle = 50;

double  setCharge = 9;
double  outDC= 0;
double  DCtemp = 0;

//timer
unsigned long myTime1;
unsigned long myTime2;

//Vb
double realBatteryV =0;

void setup() {
  Serial.begin(9600);
  initBluetooth();

TCCR0B = TCCR0B &11111000 | B00000001;//62kHz
pinMode(HzOutPut, outDC);
myTime2 = myTime1;
}
// main part
void loop() {
  //updateSerial();
  //get the read from A0
  inputV = analogRead(A0);
  BatteryV = analogRead(A1);
  //
  double  DCtemp = outDC;
  outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);
  
  realBatteryV = BatteryV*0.0045*6.25;
  
  analogWrite(HzOutPut, outDC);

    myTime1 = millis();
  if(myTime1 - myTime2>= 300000*6.2){

    myTime2 = myTime1;
    Serial.print("boostVin:");
    Serial.println(boostVout);
    Serial.print("dutycycle:");
    Serial.println(dutysycle);
    Serial.print("baterry:");
    Serial.println(realBatteryV);
    //Serial.println(realBatteryV);
  }
  
}

double DutyCycleCauculation(double inputV, double setCharge, double preDC){
  double realVin = (inputV*0.0045)*5;
  
  boostVout = realVin;


  double out = preDC;
  if (realVin< setCharge && out<200){
    out++;
  }
 else if(realVin > setCharge && out>1){
  out--; 
 }
 dutysycle = out;

  return out;
}
