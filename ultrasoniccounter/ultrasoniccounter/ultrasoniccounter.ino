#include <LCD_I2C.h> 
#define pass_distance 80
bool trying_to_get_in = false;
bool trying_to_get_out = false;
unsigned long time_now = 0;
int count = 0;
bool alreadyrun, countmod = false;
int x;
int relay = 9;
long time_add_3;


LCD_I2C lcd(0x27); // Default address of most PCF8574 modules, change accordingly
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 5; // Echo Pin of Ultrasonic Sensor

const int pingPin2 = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 8; // Echo Pin of Ultrasonic Sensor
void setup() {
  // put your setup code here, to run once:
//  lcd.backlight();
  Serial.begin(9600);
   pinMode(pingPin2, OUTPUT);
   digitalWrite(pingPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin2, LOW);
   pinMode(echoPin2, INPUT);
   pinMode(relay, OUTPUT);
  Serial.print("initializing");
  delay(2000);
//  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
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
    x= getTimeAdd3();
  }

  if (countmod){
    if(x>millis()){
      digitalWrite(relay, HIGH);
      Serial.println("Led On");
    }
    else{
      digitalWrite(relay, LOW);
      Serial.println("Led Off");
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
 count ++;
}


long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

int getTimeAdd3(){
  time_add_3 = millis() + 3000;
  alreadyrun = true;
  return time_add_3;
}
