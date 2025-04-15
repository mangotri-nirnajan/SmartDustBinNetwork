// Working code for End 2

#include <ESP8266WiFi.h>
#include <espnow.h>

const int trigPin = D1; // Define the trigger pin for the first ultrasonic sensor
const int echoPin = D2; // Define the echo pin for the first ultrasonic sensor
const int soilPin = A0; // Define the Analog pin for the Soil Moisture Sensor
int distance1; // Variable to store distance measured by the first ultrasonic sensor
float soilValue; // Variable to store moisture measured by the Soil Moisture Sensor


// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xE4, 0x1D, 0x8C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int b;
  float c;
  String d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully initialized, we will register for Send and Receive callbacks
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Initialize serial communication
  delay(10);
}

void loop() {
  measureDistance(trigPin, echoPin, distance1); // Measure distance with the first ultrasonic sensor
  soilValue = 1024 - analogRead(soilPin); // Invert the soil moisture value
  
  Serial.print("Soil Moisture:");
  Serial.println(soilValue);
  // Print the distances to the serial monitor
  printDistance("Sensor 1", distance1);
  
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    myData.b = distance1;
    myData.c = soilValue;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    lastTime = millis();
  }
}

void measureDistance(int trigPin, int echoPin, int &distance) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  float duration = pulseIn(echoPin, HIGH);
  float cm = (duration / 58.82);
  distance = cm;
}

void printDistance(const char *sensorName, int distance) {
  Serial.print("Distance from ");
  Serial.print(sensorName);
  Serial.print(": ");
  Serial.print(distance);
  Serial.println(" cm");
}
