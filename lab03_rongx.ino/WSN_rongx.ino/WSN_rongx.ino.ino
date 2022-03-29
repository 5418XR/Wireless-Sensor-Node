
int HzOutPut =6;
double inputV = A0;
int outputV = A1;// not yet coding

double RESISTER_VALUE_ONE = 30;
double RESISTER_VALUE_TWO = 120;

int DutyCycle = 50;

double  setCharge = 9;
double  outDC= 0;
double  DCtemp = 0;

void setup() {
  initBluetooth();
//Serial.begin(9600);
TCCR0B = TCCR0B &11111000 | B00000001;//62kHz
pinMode(HzOutPut, outDC);

}
// main part
void loop() {
  //updateSerial();
  //get the read from A0
  inputV = analogRead(A0);
  //
  double  DCtemp = outDC;
  outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);

  analogWrite(HzOutPut, outDC);
}

double DutyCycleCauculation(double inputV, double setCharge, double preDC){
  double realVin = (inputV*0.0045)*5;
  Serial.print("Vin:");
  Serial.print(realVin);
  //Serial.print(\0);

  double out = preDC;
  if (realVin< setCharge && out<200){
    out++;
  }
 else if(realVin > setCharge && out>1){
  out--; 
 }
  Serial.print("VoutDC:");
  Serial.print(out);
  return out;
}
