#include <ESP32Servo.h>
#include<Time.h>
#include <TimeAlarms.h>
#include <WiFi.h>


// Define first box pins
const int buzzerPin = 26;
const int ultrasonicechoPin = 33;
const int ultrasonictriggerPin=25;
const int servoPin = 12;
const int reedPin = 32;
const int irPin = 13;
const int redLightPin = 27;
const int greenWhiteLightPin = 14;

//Define second box pins
const int servoPin1 = 4;
const int reedPin1 = 22;
const int irPin1= 5;
const int redLightPin1 = 21;
const int greenWhiteLightPin1= 19;

//wifi
const char* ssid = "vivo Y22";
const char* password = "pavan@123rrpv";
const int serverPort = 80; // Port number the receiver is listening on
WiFiServer server(serverPort);
WiFiClient client;



// Servo first object
Servo servo;

//Servo second object
Servo myservo;

// Variables
bool isOpen = false; // Box open state
bool medicineTaken = false; // Medicine taken flag
bool irMotion1Detected = false; // First IR motion flag
bool irMotion2Detected = false; // Second IR motion flag
bool medcinetakendetected=false;
bool medcinedropdetected=false;
int count = 0; 



void setup() {
Serial.begin(115200);

 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set box3 pin modes
  pinMode(buzzerPin, OUTPUT);
  servo.attach(servoPin);
  pinMode(reedPin, INPUT_PULLUP);
  pinMode(irPin, INPUT);
  pinMode(redLightPin, OUTPUT);
  pinMode(greenWhiteLightPin, OUTPUT);
  pinMode(ultrasonictriggerPin, OUTPUT);
  pinMode(ultrasonicechoPin, INPUT);
  
  //Set box4 pinmodes
  myservo.attach(servoPin1);
  pinMode(reedPin1, INPUT_PULLUP);
  pinMode(irPin1, INPUT);
  pinMode(redLightPin1, OUTPUT);
  pinMode(greenWhiteLightPin1, OUTPUT);

  setTime(8,59,55,02,12,2023);

  server.begin();

}

void loop() {
  client = server.available();
  digitalClockDisplay();

  // Active only during specified time range morning time
  if (hour()==9 && minute() <2 ) {

  // Check for box 1 medicine intake
  if (!medicineTaken && hour()==9 && minute()%1==0 && second()==0) {
        // Interval alarm
        digitalWrite(buzzerPin,HIGH); // Play buzzer for 1 minute
        delay(5000);
        digitalWrite(buzzerPin,LOW);
      
    }

  if (client) { 
     int distance = 0;
    client.read((uint8_t*)&distance, sizeof(distance));
  // Check ultrasonic sensor for box opening
    if (!isOpen && distance <= 20 && !medicineTaken) {
      Serial.println(distance);
      servo.write(90); // Open box
      isOpen = true;
      
    } else if (isOpen && distance > 20) {
      Serial.println(distance);
      servo.write(0); // Close box
      isOpen = false;
    }
    client.stop();
  }

  if(!medicineTaken){
  
   if((!digitalRead(irPin)) && count ==0){
    irMotion1Detected=true;
    Serial.println("Medicine taken detected!");
    delay(5000);
    count++;
   }
   if(!digitalRead(irPin)&&irMotion1Detected && count==1){
    irMotion2Detected=true;
    Serial.println("Medicine drop detected!");
    delay(3000);
    count++;
   }
  }
  if(irMotion1Detected&irMotion2Detected){ 
    medicineTaken=true;
  }
//box3 code completed
  }
//Resetting the flags 
if (hour()==9 && minute()==2 && second()== 1) {
 resetFlags();
 delay(1000);
 Serial.println(count);
 delay(4000);
}


  // Active only during specified time range afternoon time
  if (hour()==9 && minute()>2 && minute()<=5 ) {

  
  if (client) { 
     int distance = 0;
    client.read((uint8_t*)&distance, sizeof(distance));
  // Check ultrasonic sensor for box opening
    if (!isOpen && distance <= 20 && !medicineTaken) {
      Serial.println(distance);
      myservo.write(90); // Open box
      isOpen = true;
      
    } else if (isOpen && distance > 20) {
      Serial.println(distance);
      myservo.write(0); // Close box
      isOpen = false;
    }
    client.stop();
  }

  if(!medicineTaken){
  
   if(!(digitalRead(irPin1)) && (count==0)){
    irMotion1Detected=true;
    Serial.println("Medicine taken detected!");
    delay(5000);
    count++;
   }
   if(!(digitalRead(irPin1))&&(irMotion1Detected) && (count==1)){
    irMotion2Detected=true;
    Serial.println("Medicine drop detected!");
    delay(3000);
    count++;
   }
  }
  if(irMotion1Detected&irMotion2Detected){
    medicineTaken=true;
  }


  }
// Check reed switch for box 1 lid closed
    if (digitalRead(reedPin)) {
      turnOffRedLight();
      turnOnGreenWhiteLight();
    } else {
      turnOnRedLight();
      turnOffGreenWhiteLight();
    }

 // Check reed switch for box 2 lid closed
    if (digitalRead(reedPin1)) {
      turnOffRedLight2();
      turnOnGreenWhiteLight2();
    } else {
      turnOnRedLight2();
      turnOffGreenWhiteLight2();
    }

}


void turnOnRedLight() {
  digitalWrite(redLightPin, HIGH);
}

void turnOffRedLight() {
  digitalWrite(redLightPin, LOW);
}

void turnOnGreenWhiteLight() {
  digitalWrite(greenWhiteLightPin, HIGH);
}

void turnOffGreenWhiteLight() {
  digitalWrite(greenWhiteLightPin, LOW);
}
void turnOnRedLight2() {
  digitalWrite(redLightPin1, HIGH);
}

void turnOffRedLight2() {
  digitalWrite(redLightPin1, LOW);
}

void turnOnGreenWhiteLight2() {
  digitalWrite(greenWhiteLightPin1, HIGH);
}

void turnOffGreenWhiteLight2() {
  digitalWrite(greenWhiteLightPin1, LOW);
}

void resetFlags() {
 isOpen = false; 
 medicineTaken = false; 
 irMotion1Detected = false; 
 irMotion2Detected = false; 
 medcinetakendetected=false;
 medcinedropdetected=false;
count = 0; 
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}