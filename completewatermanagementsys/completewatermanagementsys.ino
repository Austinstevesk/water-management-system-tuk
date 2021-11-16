#define SENSOR  6
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;     
float totalLitres;
#define pass_distance 80
bool trying_to_get_in = false;
bool trying_to_get_out = false;
int count = 0;
bool countmod = false;
int x;
int led = 9;
long desired_stop_millis;


//LCD_I2C lcd(0x27); // Default address of most PCF8574 modules, change accordingly
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 5; // Echo Pin of Ultrasonic Sensor

const int pingPin2 = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 8; // Echo Pin of Ultrasonic Sensor

void setup() {
  // put your setup code here, to run once:
  //  lcd.backlight();
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  
  pinMode(SENSOR, INPUT_PULLUP);
  Serial.print("initializing");
  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("--------------------------");

             currentMillis = millis();
  if (currentMillis - previousMillis > interval) 
  {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
 
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
 
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    flowLitres = (flowRate / 60);
 
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    totalLitres += flowLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(float(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
 
 
    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalLitres);
    Serial.println("L");
//    lcd.setCursor(0,1);
//    lcd.print("Total litres");
//    lcd.setCursor(1,5);
//    lcd.print(totalLitres);
 
  }

Serial.println("Count: "+ count);
  
  if(count < 0 ) count = 0;
  //--------------------------------------------------------
//  lcd.setCursor(0,0);
//  lcd.print("Total people");
//  lcd.setCursor(0,1);
//  lcd.print(count);
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   int d1 = microsecondsToCentimeters(duration);
   //---------------------------------------------------
          long duration2, inches2, cm2;
   pinMode(pingPin2, OUTPUT);
   digitalWrite(pingPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin2, LOW);
   pinMode(echoPin2, INPUT);
   duration2= pulseIn(echoPin2, HIGH);
   inches2 = microsecondsToInches(duration2);
   int d2 = microsecondsToCentimeters(duration2);

  
  
   //-------------------------------------------

   Serial.print("ultrasonic1 => ");
   Serial.print(d1);
   Serial.print("  Ultrasonic2 => ");
   Serial.println(d2);


   //Print count value
   Serial.print("Count: ");
   Serial.println(count);
  if((count%10) == 0){
    countmod = true;
    x= getTimeAddSecs();
  }

  if (countmod){
    if(x>millis()){
      digitalWrite(led, HIGH);
      Serial.println("Led On");
    }
    else{
      digitalWrite(led, LOW);
      Serial.println("Led Off");
      countmod = false;
    }
  }

 
   
   

 if (d1 < pass_distance && trying_to_get_out == false){
  trying_to_get_in = true;
 }
 else if (d2 < pass_distance && trying_to_get_in == false){
  trying_to_get_out = true;
 }
 if (d2 < pass_distance && trying_to_get_in == true){
  count++;
  trying_to_get_in = false; 
  delay(500);
 }


 else if (d1 < pass_distance && trying_to_get_out == true){
  count--;
  trying_to_get_out = false;
  delay(500);
 }
}


long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

int getTimeAddSecs(){
  desired_stop_millis = millis() + 3000;
  return desired_stop_millis;
}
