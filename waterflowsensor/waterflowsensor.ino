#include <SPI.h>
#include <Wire.h>
#include <LCD_I2C.h> 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define SENSOR  D6
LCD_I2C lcd(0x27); // Default address of most PCF8574 modules, change accordingly
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
unsigned long time_now = 0;

//Setting wifi and server Ids
const char* ssid = "tuk-wifi";// Wifi name
const char* password = "P@ssword@123"; //Wifi password
//WiFiClient client;
char server[] = "192.168.217.236";   //eg: 192.168.0.222


WiFiClient client;    

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SENSOR, INPUT_PULLUP);
  lcd.backlight();
  Serial.println("Initializing");
  //lcd.setCursor(0,2);
  //lcd.print("Starting..");
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  
 
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
// 
  // Start the server
//  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");

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
    lcd.setCursor(0,1);
    lcd.print("Total litres");
    lcd.setCursor(1,5);
    lcd.print(totalLitres);
    delay(1000);
    if (totalLitres>0){
      Senddata();
    }
 
  }

}

void Senddata()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /water/senddata.php?litres=");
    client.print("GET /water/senddata.php?litres=");     //YOUR URL
    Serial.println(totalLitres);
    client.print(totalLitres);
    
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.217.236"); //Your local IP
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
