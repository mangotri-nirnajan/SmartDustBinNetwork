#include <SPI.h>
#include <LoRa.h>

#define laser 4
#define sensor 3

#define buzzer 5
#define POWER_PIN  7
#define SIGNAL_PIN A5

int plastic = 0;
int wetwaste = 0;

void rain()
{
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  int value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("Sensor value: ");
  Serial.println(value);
  if (value >= 200)
  {
    wetwaste += 1;
  }
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(laser, OUTPUT);
  pinMode(sensor, INPUT);
  //pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(laser, HIGH);
  pinMode(POWER_PIN, OUTPUT);   // configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  bool value = digitalRead(sensor);

  if (value == 0) {
    //digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);

    // Print a message in the serial monitor when sensor is detected
    Serial.println("Sensor Detected!");
    plastic += 1;
  } else {
    //digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }

  rain();

  // send data through LoRa
  Serial.print("Sending packet - Plastic: ");
  Serial.print(plastic);
  Serial.print(", Wet Waste: ");
  Serial.println(wetwaste);

  // send packet
  LoRa.beginPacket();
  LoRa.print("Plastic: ");
  LoRa.print(plastic);
  LoRa.print(", Wet Waste: ");
  LoRa.print(wetwaste);
  LoRa.endPacket();

  delay(5000);
}
