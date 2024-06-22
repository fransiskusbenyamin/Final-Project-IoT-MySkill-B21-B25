#define BLYNK_TEMPLATE_ID "TMPL6rvymfhhh"
#define BLYNK_TEMPLATE_NAME "Air Immortal"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Define the pin connected to the MQ135 sensor and buzzer
#define MQ135_PIN 34
#define BUZZER_PIN 14

// Define the pins connected to the RGB LED
#define RED_PIN 27
#define GREEN_PIN 26
#define BLUE_PIN 25

// Thresholds for air quality
#define POOR_AIR_THRESHOLD 1200
#define DANGEROUS_AIR_THRESHOLD 2000

// Blynk Auth Token and Wi-Fi credentials
char auth[] = "kYqMSqdbls2uN7MEpltE6nuf8D6KqOAq"; // Replace with your Blynk Auth Token
char ssid[] = "Departemen Tanpa Internet";      // Replace with your Wi-Fi SSID
char pass[] = "fqkj4876";  // Replace with your Wi-Fi Password

// Blynk Timer
BlynkTimer timer;

// Function to send sensor value and air quality status to Blynk
void sendSensorData() {
  int sensorValue = analogRead(MQ135_PIN);
  Blynk.virtualWrite(V5, sensorValue);

  String airQuality;
  if (sensorValue > DANGEROUS_AIR_THRESHOLD) {
    // Dangerous air quality: Play constant tone and show red light
    digitalWrite(BUZZER_PIN, LOW); // Turn on the buzzer (low level trigger)
    setColor(255, 0, 0);           // Red color
    airQuality = "Dangerous";
  } else if (sensorValue > POOR_AIR_THRESHOLD) {
    // Poor air quality: Play alternating tone and show yellow light
    digitalWrite(BUZZER_PIN, LOW); // Turn on the buzzer (low level trigger)
    setColor(255, 255, 0);         // Yellow color
    delay(500);
    digitalWrite(BUZZER_PIN, HIGH); // Turn off the buzzer
    delay(500);
    airQuality = "Poor";
  } else {
    // Good air quality: Turn off the buzzer and show green light
    digitalWrite(BUZZER_PIN, HIGH); // Turn off the buzzer
    setColor(0, 255, 0);            // Green color
    airQuality = "Safe";
  }

  // Send air quality status to Blynk
  Blynk.virtualWrite(V6, airQuality);

  // Print sensor value and air quality status to the Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("\tAir Quality: ");
  Serial.println(airQuality); 
}

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Initialize the analog pin as input and buzzer pin as output
  pinMode(MQ135_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize the RGB LED pins as outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Print a message to indicate the start of the program
  Serial.println("MQ135 Gas Sensor Data Collection with Buzzer and RGB LED Alert");

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  // Set a timer to send data to Blynk every second (1000ms)
  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Run Blynk Timer
  timer.run();
}

// Function to set the color of the RGB LED
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
