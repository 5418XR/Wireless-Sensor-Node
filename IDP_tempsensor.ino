//defining macros for constant values
#define RT0 10000   // 10000 ohms
#define B 3900      //Constant taken from data sheet
#define Vs 5    //Supply voltage
#define R 10000  //Resistance of thermistor is 10000 ohms

//defining variables
float RT, VR, lnRfrac, temp, T0, ThermistorOut;

void setup() {
  // put your setup code here, to run once:

  //setting baud rate
  Serial.begin(9600); 
  //conversion of room temp from C to K
  T0 = 25 + 273.15; 
}

void loop() {
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
}
