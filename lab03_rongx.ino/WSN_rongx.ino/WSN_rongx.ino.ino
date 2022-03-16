
int HzOutPut =6;     //output pin
double inputV = A0;  //input pin
int outputV = A1;    // not yet coding but it will code

double RESISTER_VALUE_ONE = 10;   //resister for the input
double RESISTER_VALUE_TWO = 100;  //resister for the other resister

int DutyCycle = 50;   //defualt duty cycle

double  setCharge = 9; // the target charging voltage
double  outDC= 0;      // the start input dutyCycle
double  DCtemp = 0;    // the start input dutycycle +1


//setup the the 62k Hz clock 
void setup() {
Serial.begin(9600);
TCCR0B = TCCR0B &11111000 | B00000001;//62kHz
pinMode(HzOutPut, outDC);

}

// main part
void loop() {
  //get the read from A0
  inputV = analogRead(A0);
  //set the DutyCycle with PID mofdle
  double  DCtemp = outDC;
  outDC = DutyCycleCauculation(inputV, setCharge, DCtemp);// using the function 
  //setal the output votagle
  analogWrite(HzOutPut, outDC);
}

//the output DutyCycal changing Function
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
