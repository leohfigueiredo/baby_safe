#include <WiFi.h> // Wi-Fi library inclusion
#include <BlynkSimpleEsp32.h> // Blynk library inclusion

#include <Tone32.h>

#include <EEPROM.h> // EEPROM library inclusion for reading and writing to flash memory

#define EEPROM_SIZE 1 // Define the number of bytes you want to access - 1 in this case

#define BUZZER_PIN 12 // Sound output pin for the buzzer
#define BUZZER_CHANNEL 0 // Buzzer channel

const int buttonPin = 23;    // Button pin definition
const int ledPin = 22;      // Child state indication LED pin - on: child in the car - off: No child seated

const char* ssid = "XXXXXX"; // Include the Wi-Fi SSID
const char* password = "XXXXXXXX"; // Include the Wi-Fi password
char auth[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Blynk Authorization Token

// Driver simulation pins
const int PIN_RED   = 21;
const int PIN_GREEN = 19;
const int PIN_BLUE  = 4;

// Variables will change:
int ledState = HIGH;         // Current output pin state - Child LED
int buttonState;             // Current output pin state - Child button
int lastButtonState = LOW;   // Previous output pin state - Child button

// The following variables are "unsigned long" because the time, measured in
// milliseconds, will quickly become a larger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // The last time the output pin was toggled
unsigned long debounceDelay = 50;    // Debounce time; increase if the output flickers

// Ultrasonic sensor variables
const int trigPin = 5;
const int echoPin = 18;

// Sound speed definition in cm/microseconds
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;

// Program setup
void setup() {

  // Initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Read the last LED state from flash memory
  ledState = EEPROM.read(0);
  // Set the LED to the last stored state
  digitalWrite(ledPin, ledState);

  // Input and output pin definitions
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  Blynk.begin(auth, ssid, password);

  Serial.begin(115200); // Initialize Serial communication
  delay(50);
  Blynk.begin(auth, ssid, password);
  pinMode(trigPin, OUTPUT); // Set ultrasonic sensor pin as output
  pinMode(echoPin, INPUT); // Set ultrasonic sensor pin as input

}

void loop() {

  // Read the button state into a local variable:
  int reading = digitalRead(buttonPin);

  Blynk.run(); // Initialize Blynk

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Set the trigPin to HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  // Convert to inches
  //distanceInch = distanceCm * CM_TO_INCH;

  // Print the

 distance in cm
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  // Check if the button has just been pressed
  // (i.e., the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Whatever the reading is at, it's been there for longer than the debounce delay,
    // so take it as the actual current state:

    // If the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // Only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

  // If the ledState variable is different from the current LED state
  if (digitalRead(ledPin) != ledState) {
    Serial.println("Child state changed");
    // Change the LED state
    digitalWrite(ledPin, ledState);
    // Save the LED state in flash memory
    EEPROM.write(0, ledState);
    EEPROM.commit();
    Serial.println("State saved in memory!");
  }
  delay(5000);

  // Conditionals for:
  // 1 - Child and driver in the car - no alert - Green LED on
  if ((distanceCm < 60) && (ledState == HIGH))
  {
    digitalWrite(PIN_RED,   0);
    digitalWrite(PIN_GREEN, 255); // Turn on GREEN LED
    digitalWrite(PIN_BLUE,  0);
    delay(500);
    Blynk.virtualWrite(V19, distanceCm);
    Blynk.virtualWrite(V21, 0);
    Blynk.virtualWrite(V4, 0);
    delay(1000);

    // 2 - Driver position adjustment indication - possible new driver - turn on yellow color
  }
  else if ((distanceCm > 60 && distanceCm < 70) && (ledState == HIGH))
  {
    digitalWrite(PIN_RED,  0);
    digitalWrite(PIN_GREEN, 255); // Turn on YELLOW LED
    digitalWrite(PIN_BLUE,  255);
  }

  // ALERT - Child left in the car - Red LED on for distance > 70cm and <200cm
  else if ((distanceCm > 70) && (distanceCm < 200) && (ledState == HIGH))
  {
    digitalWrite(PIN_RED,   255); // Turn on RED LED
    digitalWrite(PIN_GREEN, 0);
    digitalWrite(PIN_BLUE,  0);
    delay(1000);
    Blynk.virtualWrite(V19, 0);
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V21, distanceCm);
    delay(1000);

    delay(10000);

    Serial.println("Sending notification to Blynk! Child left behind!"); // Sending notification to the driver's phone
    delay(2500);
    Blynk.notify("CHILD IN THE CAR!!! COME BACK!!"); // Notification text for the Blynk App
    tone(BUZZER_PIN, NOTE_E4, 1000, BUZZER_CHANNEL);
    noTone(B

UZZER_PIN, BUZZER_CHANNEL);
    delay(500);
    tone(BUZZER_PIN, NOTE_E4, 1000, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);
    delay(500);
    tone(BUZZER_PIN, NOTE_E4, 1000, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);
    delay(500);
    tone(BUZZER_PIN, NOTE_E4, 1000, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);
    delay(500);

  }
  // Situation where the child is not in the car - It can be only the driver or no one. Blue LED on
  else if (ledState == LOW)
  {
    digitalWrite(PIN_RED,   0);
    digitalWrite(PIN_GREEN, 0);
    digitalWrite(PIN_BLUE,  255); // Turn on BLUE LED
    delay(1000);
    Blynk.virtualWrite(V19, 0);
    Blynk.virtualWrite(V21, 0);
    Blynk.virtualWrite(V4, distanceCm);
    delay(500);
  }
}
