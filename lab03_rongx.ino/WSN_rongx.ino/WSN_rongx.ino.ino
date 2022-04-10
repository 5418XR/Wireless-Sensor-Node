
int HzOutPut =6;
double inputV = A0;
double BatteryV = A1;// not yet coding
double chargein = A2;// not yet coding

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
void setup() {
  Serial.begin(9600);
  //initBluetooth();

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
  chargein = analogRead(A2);
  //
  
  
//  double  DCtemp = outDC;
//  outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);
//  realBatteryV = BatteryV*0.0045*6.25*0.37;
  if (STAGE == 1){
      analogWrite(HzOutPut, ZERO);
      digitalWrite(5, HIGH); // 5 to pmose
      clocknumber = 300000*6.2;
  }
  if (STAGE == 2 || STAGE == 0){
      double  DCtemp = outDC;
      outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);
      realBatteryV = BatteryV*0.0045*6.25*0.37;
      analogWrite(HzOutPut, outDC);
      clocknumber = 30000;
  }
  if (STAGE == 3){
    analogWrite(HzOutPut, ZERO);
    clocknumber = 30000;
  }
  if (STAGE == 4){
    analogWrite(HzOutPut, ZERO);
    clocknumber = 300000*6.2*10;
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

  if (chargein>1 && realBatteryV>9){
    STAGE = STAGE1;
  }
  if (chargein>1 && realBatteryV<9){
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
