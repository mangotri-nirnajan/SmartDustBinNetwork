#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define RXp2 16
#define TXp2 17

// Insert your network credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
// Insert Firebase project API Key
#define API_KEY ""
// Insert RTDB URL
#define DATABASE_URL "https://iotproject.asia-southeast1.firebasedatabase.app/"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int mun_waste = 0;
int soil_moisture = 0;
int end_waste = 0;
bool signupOK = false;
int intValue;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(2,OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Assign the API key (required)
  config.api_key = API_KEY;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  // Sign up
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // Assign the callback function for the long-running token generation task
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Serial2.available()) {
    String receivedData = Serial2.readStringUntil('\n');
    Serial.println("Received via UART: " + receivedData);

    // Parse the received data
    int firstValue = receivedData.substring(0, receivedData.indexOf(',')).toInt();
    int commaIndex1 = receivedData.indexOf(',');
    int secondValue = receivedData.substring(commaIndex1 + 1, receivedData.indexOf(',', commaIndex1 + 1)).toInt();
    int thirdValue = receivedData.substring(receivedData.indexOf(',', commaIndex1 + 1) + 1).toInt();

    // If the first value is 1, store second and third values accordingly
    if (firstValue == 1) {
      if(secondValue >=100){
        secondValue=100;
        mun_waste = secondValue;
        soil_moisture = thirdValue/10;
        
      }
      else{
        mun_waste = secondValue;
        soil_moisture = thirdValue/10;
      }
      

      Serial.println("Data from first node:");
    }
    // If the first value is 2, store the second value as end_waste
    else if (firstValue == 2) {
      end_waste = secondValue;
      Serial.println("Data from second node:");
    }

    // Print the parsed values
    Serial.print("First value (ID): ");
    Serial.println(firstValue);
    Serial.print("Second value (mun_waste or end_waste): ");
    Serial.println(secondValue);
    Serial.print("Third value (soil_moisture, if applicable): ");
    Serial.println(thirdValue);

    // Write parsed values to Firebase
    if (Firebase.ready() && signupOK) {
      // Send mun_waste value
      if (Firebase.RTDB.setInt(&fbdo, "test/mun_waste_level", mun_waste)) {
        Serial.println("mun_waste value sent successfully");
      } else {
        Serial.println("Failed to send mun_waste value");
      }
      // Send soil_moisture value
      if (Firebase.RTDB.setInt(&fbdo, "test/soil_moisture", soil_moisture)) {
        Serial.println("soil_moisture value sent successfully");
      } else {
        Serial.println("Failed to send soil_moisture value");
      }
      // Send end_waste value
      if (Firebase.RTDB.setInt(&fbdo, "test/int", end_waste)) {
        Serial.println("end_waste value sent successfully");
      } else {
        Serial.println("Failed to send end_waste value");
      }
    }
  }

  // Example code to send a constant string to Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    // Write a string to the database path "test/string"
    if (Firebase.RTDB.setString(&fbdo, "test/string", "Waste level end node 566")) {
      Serial.println("String sent successfully");
    } else {
      Serial.println("Failed to send string");
    }
      if (Firebase.RTDB.getInt(&fbdo, "bin/switchState")) {
      if (fbdo.dataType() == "int") {
        intValue = fbdo.intData();
        Serial.print("Switch value is:");
        Serial.println(intValue);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
  if(intValue==1){
    digitalWrite(2,HIGH);
  }
  else{
    digitalWrite(2,LOW);
  }


}
