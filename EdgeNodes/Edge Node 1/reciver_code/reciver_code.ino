#include <esp_now.h>
#include <WiFi.h>

#define RXp0 16
#define TXp0 17

// Structure example to receive data
typedef struct struct_message {
  int id;
  int distance;      // Int to hold distance value
  int soilMoisture;  // Int to hold soil moisture value
} struct_message;

// Create structures
struct_message myData;
struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2}; // Correcting the size to 2

esp_now_peer_info_t peerInfo;

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  // Copy incoming data into myData structure
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

  // Update the appropriate board structure
  if (myData.id >= 1 && myData.id <= 2) { // Ensure the id is within valid range
    boardsStruct[myData.id - 1].distance = myData.distance;
    boardsStruct[myData.id - 1].soilMoisture = myData.soilMoisture;
    Serial.printf("Distance: %d \n", boardsStruct[myData.id - 1].distance);
    Serial.printf("Soil Moisture: %d \n", boardsStruct[myData.id - 1].soilMoisture);
    Serial.println();
  }

  // Forward the received data to the final node via UART
  String dataToSend = String(myData.id) + "," + String(myData.distance) + "," + String(myData.soilMoisture);
  Serial.println(dataToSend);
  Serial2.print(dataToSend); // Send the formatted string over UART
}

uint8_t broadcastAddress[] = {0xC4, 0x5B, 0xBE, 0x63, 0x58, 0x34}; // Example MAC address

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize UART
  Serial2.begin(9600, SERIAL_8N1, RXp0, TXp0);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Placeholder for any additional logic
  delay(10000);
}
