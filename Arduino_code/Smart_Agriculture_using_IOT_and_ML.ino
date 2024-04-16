// Blynk IOT Smart Agriculture Monitoring System

/* Connections
Relay. D3
Btn.   D7
Soil.  A0
PIR.   D5
SDA.   D2
SCL.   D1
Temp.  D4
*/

//Include the library files
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3QrM6R0ym"
#define BLYNK_TEMPLATE_NAME "Smart Agriculture"
#define BLYNK_AUTH_TOKEN "x_zCFUSrb8Henyq6Ih1PWQgq07XEeRnB"
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <math.h>


char auth[] = "x_zCFUSrb8Henyq6Ih1PWQgq07XEeRnB";  //Blynk Auth token
char ssid[] = "URNANBAN";  //WIFI SSID
char pass[] = "zamil@321";  //WIFI Password

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT sensor setup
DHT dht(D4, DHT11); // DHT11 Temperature Sensor

// Component pins
#define soil A0     // Soil Moisture Sensor
#define PIR D5      // PIR Motion Sensor
#define RELAY_PIN_1 D3   // Relay
#define PUSH_BUTTON_1 D7   // Button
#define VPIN_BUTTON_1 V12 

// Variables
int relay1State = HIGH;
int pushButton1State = LOW;
int PIR_ToggleValue;

// Initialize Blynk timer
BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(PIR, INPUT);

  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, relay1State);
  
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(11, 1);
  lcd.print("W:OFF");

  // Call the functions
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(500L, checkPhysicalButton);
}

void loop() {
  if (PIR_ToggleValue == 1) {
    lcd.setCursor(5, 1);
    lcd.print("M:ON ");
    PIRsensor();
  } else {
    lcd.setCursor(5, 1);
    lcd.print("M:OFF");
    WidgetLED LED(V5);
    LED.off();
  }

  if (relay1State == LOW) {
    lcd.setCursor(11, 1);
    lcd.print("W:ON ");
  } else {
    lcd.setCursor(11, 1);
    lcd.print("W:OFF");
  }
  
  Blynk.run();
  timer.run();
}

//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Round off temperature and humidity to one decimal place
  t = round(t*10)/10;
  h = round(h*10)/10;

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V3, value);
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(value);

}

//Get the PIR sensor values
void PIRsensor() {
  bool value = digitalRead(PIR);
  if (value) {
    Blynk.logEvent("pirmotion","WARNNG! Motion Detected!"); //Enter your Event Name
    WidgetLED LED(V5);
    LED.on();
  } else {
    WidgetLED LED(V5);
    LED.off();
  }  
  }

BLYNK_WRITE(V6) {
  PIR_ToggleValue = param.asInt();  
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}


void checkPhysicalButton() {
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    // Toggle the relay state only if the button state changes
    if (pushButton1State != LOW) {
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}