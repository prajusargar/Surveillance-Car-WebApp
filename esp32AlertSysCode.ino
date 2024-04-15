// Working

// #if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// #define WIFI_SSID "Wokwi-GUEST"
// #define WIFI_PASSWORD ""

String baseURL = "https://api.thingspeak.com/update?api_key=X41Y0XE6GQ043KM1&";


#define WIFI_SSID "MAHAKAL"
#define WIFI_PASSWORD "sateri456"

#include <ESP32Servo.h>
const int servoPin = 15;
Servo servo;
int pos = 0;

#define pir 4
int pirVal = 0;
int pirDetectedVal = 0;

#define buzzerPin 23
#define ledPin 19

// Strob Led
#define redLed 32
#define blueLed 33

int radarThreshold = 14;
int radardata = 0;

// Ultrasonic Part
const int trigPin = 5;
const int echoPin = 18;

float leftDistCm;
float frontDistCm;
float rightDistCm;


//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

int lcddist = 0;
long duration;
float distanceCm;
float distanceInch;

void makeGETRequest() {
  // tempData = String(tempdataval);
  // humData  = String(humdataval);

  // String url = baseURL + String(tempdataval) + field2 + String(humdataval);
  String url = baseURL + "field1=" + String(pirDetectedVal) + "&field2=" + String(leftDistCm) + "&field3=" + String(frontDistCm) + "&field4=" + String(rightDistCm);

  Serial.print("Sending GET request to: ");Serial.println(url);


  HTTPClient http;
  http.begin(url); // Specify the URL here
  http.setTimeout(10000); // Set timeout to 10 seconds


  int httpCode = http.GET();
  
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

int ReadUltra(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  distanceInch = distanceCm * CM_TO_INCH;
  // Serial.print("Distance (cm): ");      Serial.println(distanceCm);
  delay(1000);
  
  return distanceCm;
}

void LcdStatus(int lcddist){
  lcd.setCursor(0, 0);  lcd.print("Radar");
  lcd.setCursor(8, 0);  lcd.print(lcddist);
}

int radarScan() {
  servo.write(0); delay(100);
  lcddist = ReadUltra();
  if (lcddist <= radarThreshold) { return 1; }
  
  servo.write(90); delay(100);
  lcddist = ReadUltra();
  if (lcddist <= radarThreshold) { return 2; }
  
  servo.write(180); delay(100);
  lcddist = ReadUltra();
  if (lcddist <= radarThreshold) { return 3; }

  return 0; // If no obstacle detected
}


int PIRScan(){
    // ----------------- PIR ----------------- 
  pirVal = digitalRead(pir);
  if (pirVal == 1){
    Serial.println("Alert : Motion detected");
    lcd.setCursor(0, 1);  lcd.print("Motion: Detected");
    delay(100);
    // make the server request
  }
  else{
    lcd.setCursor(0, 1);  lcd.print("Motion: --------");
  }
  return pirVal;
}

void blinkTwice() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin, HIGH); // Turn the LED on
    delay(500); // Wait for 500 milliseconds
    digitalWrite(ledPin, LOW); // Turn the LED off
    delay(500); // Wait for 500 milliseconds
  }
}

void strobeLed() {
  digitalWrite(redLed, LOW); // Turn on the red LED
  digitalWrite(buzzerPin, HIGH); // Turn off the blue LED
  delay(100); // Wait for a short duration
  digitalWrite(blueLed, LOW); // Turn on the blue LED
  digitalWrite(buzzerPin, LOW); // Turn off the blue LED
  delay(200); // Wait for a short duration
  
  digitalWrite(redLed, HIGH); // Turn off the red LED
  digitalWrite(buzzerPin, HIGH); // Turn off the blue LED
  delay(100); // Wait for a short duration
  digitalWrite(blueLed, HIGH); // Turn off the blue LED
  digitalWrite(buzzerPin, LOW); // Turn off the blue LED
  delay(100); // Wait for a short duration
}

void setup()
{
  Serial.begin(115200);delay(2000);
  lcd.init();  lcd.backlight();
  
  // Main Setup...
  servo.attach(servoPin, 500, 2400);
  pinMode(pir, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(redLed, OUTPUT); // Initialize the red LED pin as an output
  pinMode(blueLed, OUTPUT); // Initialize the blue LED pin as an output
  pinMode(ledPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(buzzerPin, OUTPUT); // Sets the trigPin as an Output

  digitalWrite(redLed, HIGH); // Turn off the red LED
  digitalWrite(blueLed, HIGH); // Turn off the blue LED

  digitalWrite(ledPin, HIGH); // Turn on the Wifi LED
  delay(300);
  digitalWrite(ledPin, LOW); // Turn off the Wifi LED
  delay(100);

  // Internet Setup...
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  lcd.setCursor(0, 0);lcd.print("Wifi Connecting...");
  delay(1500);

  while (WiFi.status() != WL_CONNECTED){Serial.print(".");delay(300);}
  Serial.println();Serial.print("Connected with IP: ");Serial.println(WiFi.localIP());Serial.println();
  blinkTwice();delay(100);blinkTwice();

  // System Setup
  lcd.setCursor(0, 0);lcd.print("  Surveillance  ");
  lcd.setCursor(0, 1);lcd.print("   Car System    ");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);lcd.print("Radar Started");
}

void loop(){
  // x=random(0,9);// y=random(10,19);
  // lcddist = ReadUltra(); // read slot3


  pirDetectedVal = PIRScan();
  if (pirDetectedVal == 1){
    Serial.println("PIR --- " + String(pirDetectedVal));
    strobeLed();strobeLed();strobeLed();strobeLed();
  }
  lcd.setCursor(0, 0);  lcd.print("Radar : --------");
  

  
  // ----------------- Ultrasonic Sensor ----------------- 
  // radardata = radarScan();
  servo.write(0); delay(100);  lcddist = ReadUltra(); leftDistCm = lcddist; 
  if (lcddist <= radarThreshold) {
      Serial.println("Alert : Object at Left  Side " + String(distanceCm)); 
      // String displayText = "Radar: Left " + String(distanceCm);
      lcd.clear();lcd.setCursor(0, 0);  lcd.print("Radar: Left " + String(distanceCm));
    }

  servo.write(90); delay(100);  lcddist = ReadUltra();  frontDistCm = lcddist;
  if (lcddist <= radarThreshold) {
      Serial.println("Alert : Object at Front Side "  + String(distanceCm)); 
      String displayText = "Radar: Front " + String(distanceCm);
      lcd.clear();lcd.setCursor(0, 0);  lcd.print(displayText);
    }

  servo.write(180); delay(100);  lcddist = ReadUltra();  rightDistCm = lcddist;
  if (lcddist <= radarThreshold) {
      Serial.println("Alert : Object at Right Side "  + String(distanceCm)); 
      String displayText = "Radar: Right " + String(distanceCm);
      lcd.clear();lcd.setCursor(0, 0);  lcd.print(displayText);
    }


  // Update the server code
  // blinkTwice(); // data updated on server 

    
  else{lcd.clear();     lcd.setCursor(0, 0);  lcd.print("Radar : --------");lcd.setCursor(0, 1);  lcd.print("Motion: --------");}
  Serial.println();Serial.print("------------------------------------");Serial.println();delay(500);
  // lcd.clear();

  
  // Make the URL request
  makeGETRequest();
  blinkTwice();delay(100);blinkTwice();

  servo.write(90); delay(100);

  delay(10000);
}
