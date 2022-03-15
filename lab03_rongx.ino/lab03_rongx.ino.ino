
int HzOutPut =6;
double inputV = A0;
int outputV = A1;// not yet coding

double RESISTER_VALUE_ONE = 10;
double RESISTER_VALUE_TWO = 100;

int DutyCycle = 50;

double  setCharge = 9;
double  outDC= 0;
double  DCtemp = 0;

void setup() {
Serial.begin(9600);
TCCR0B = TCCR0B &11111000 | B00000001;//62kHz
pinMode(HzOutPut, outDC);

}
// main part
void loop() {
  //get the read from A0
  inputV = analogRead(A0);
  //
  double  DCtemp = outDC;
  outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);
  
  analogWrite(HzOutPut, outDC);
}

double DutyCycleCauculation(double inputV, double setCharge, double preDC){
  double realVin = inputV/RESISTER_VALUE_ONE*(RESISTER_VALUE_ONE+RESISTER_VALUE_TWO);
  Serial.print("Vin:");
  Serial.print(realVin);

  double out = preDC;
  if (realVin< setCharge){
    out++;
  }
 else if(realVin > setCharge){
  out--; 
 }
  Serial.print("VoutDC:");
  Serial.print(out);
  return out;
}
