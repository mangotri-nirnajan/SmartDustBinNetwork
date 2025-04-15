#include <SPI.h>
#include <LoRa.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define ss 15
#define rst 4
#define dio0 2

#define RXp2 16
#define TXp2 17

// Insert your network credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
// Insert Firebase project API Key
#define API_KEY ""
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iotproject-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(ss,rst,dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial2.begin(9600,SERIAL_8N1,RXp2,TXp2);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // try to parse LoRa packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a LoRa packet
    Serial.print("Received LoRa packet '");

    // read LoRa packet
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    // print received LoRa data
    Serial.println(receivedData);

    // Find the position of the delimiter ":"
    int delimiterPos = receivedData.indexOf(":");
    if (delimiterPos != -1) {
      // Extract the substrings before and after the delimiter
      String plasticStr = receivedData.substring(0, delimiterPos);
      String wetWasteStr = receivedData.substring(delimiterPos + 1);

      // Convert the substrings to integers
      int plastic = plasticStr.toInt();
      int wetWaste = wetWasteStr.toInt();

      // Now you have the separate variables for plastic and wet waste
      Serial.print("Plastic: ");
      Serial.println(plastic);
      Serial.print("Wet Waste: ");
      Serial.println(wetWaste);
    } else {
      Serial.println("Invalid data format");
    }

    // send received LoRa data to Firebase
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
      sendDataPrevMillis = millis();

      // Write received LoRa data to Firebase
      if (Firebase.RTDB.setString(&fbdo, "test/LoRaData", receivedData)){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }

    // print RSSI of LoRa packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
