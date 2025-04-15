
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

Servo servoMain; // Define our Servo
const int trigPin1 = D1; // Define the trigger pin for the first ultrasonic sensor
const int echoPin1 = D2; // Define the echo pin for the first ultrasonic sensor
const int trigPin2 = D3; // Define the trigger pin for the second ultrasonic sensor
const int echoPin2 = D4; // Define the echo pin for the second ultrasonic sensor
int distance1; // Variable to store distance measured by the first ultrasonic sensor
int distance2; // Variable to store distance measured by the second ultrasonic sensor


// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xE4, 0x1D, 0x8C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  //bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
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
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  
  servoMain.attach(D5); // Attach servo to pin D5 on ESP8266
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Initialize serial communication
  delay(10);

}
 
void loop() {
  measureDistance(trigPin1, echoPin1, distance1); // Measure distance with the first ultrasonic sensor
  measureDistance(trigPin2, echoPin2, distance2); // Measure distance with the second ultrasonic sensor
  // Print the distances to the serial monitor
  printDistance("Sensor 1", distance1);
  printDistance("Sensor 2", distance2);
  // Example servo control based on distance from Sensor 1
  controlServo(distance1);



  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    strcpy(myData.a, "Waste level in Dustbin is");
    myData.b = distance2;
    myData.c = 9.0;
    //myData.d = "Hello";
    //myData.e = false;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}

void measureDistance(int trigPin, int echoPin, int &distance)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float cm = (duration / 58.82);
  distance = cm;
}

void printDistance(const char *sensorName, int distance)
{
  Serial.print("Distance from ");
  Serial.print(sensorName);
  Serial.print(": ");
  Serial.print(distance);
  Serial.println(" cm");
}

void controlServo(int distance)
{
  if (distance < 30)
  {
    servoMain.write(90); // Turn Servo to 90 degrees
    delay(1500);
  }
  else
  {
    servoMain.write(0); // Turn Servo to 0 degrees
    delay(1500);
  }
}