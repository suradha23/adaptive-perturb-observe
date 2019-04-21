/*
 * MPPT converter control code
  ReadAnalogVoltage
  Reads an analog input on pin A3, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +3V and ground.
  
  Hardware Required:
  * MSP-EXPF5529 LaunchPad
  * Voltage Divider ccts
 
  This example code is in the public domain.
*/


const float M= 13; // ~0.05 DC variation
float V1;
float exV; 
float exI;
float delV;
float I;
float Pnow; 
float exP;
float Vref;
float delVref;
int PWM= A1;// external LED attached
int dc=127; // 50% DC assumed initially


// the setup routine runs once when you press reset:
void setup() {
  V1= 0;
  exV=0;
  exI=0;
  I= 0;
  exP= 2.5; 
  Pnow= 0;
  delVref= 0; // probs right
  Vref= 1.5;    //cheggit
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); // msp430g2231 must use 4800
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(PWM, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the voltage input on analog pin A3, current on pin 6_3:
  int VADC = analogRead(A3);
  delay(20); // 20ms delay between sampling
  int IADC = analogRead (A4);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3V): and find Power for current iteration
  float voltage = VADC * (3.0 / 1023.0); 
  float current = IADC * (3.0/ 1023.0); 

  constrain (voltage, 0.1, 3);
  constrain (current, 0.1, 3);
  Pnow= voltage* current;
 
 float delP= Pnow- exP;
 delV = voltage- exV;
 delVref= M * (delP/ voltage- exV);
 //Vref= Vref+- delVref; 
 
 if (delP > 0){

if (delV > 0)
      Vref= Vref + delVref;
    
else
      Vref = Vref - delVref;
 }


 else 
{
if (delV < 0)
      Vref= Vref + delVref;
    
else
      Vref = Vref - delVref;
 }

//modulating duty cycle
  dc= dc + (Vref * M);
constrain (dc, 0, 255);
analogWrite(PWM, dc);
    
  // print out the value you read:
  Serial.println("\n");
  Serial.println(delP );
  Serial.println("DUTY CYCLE : \n" );
  Serial.println((dc+1)/256* 100);

  exV= voltage;
  exI= current;
  exP= exV* exI; 
  
  delay(100);
}
