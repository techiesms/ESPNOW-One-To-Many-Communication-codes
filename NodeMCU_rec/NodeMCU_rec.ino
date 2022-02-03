

#include <ESP8266WiFi.h>
#include <espnow.h>

#define Relay1            D6
#define Relay2            D2
#define Relay3            D1
#define Relay4            D5


// Define variables to store DHT readings to be sent
bool sw;

// Define variables to store incoming readings
bool incomingSW1;
bool incomingSW2;
bool incomingSW3;
bool incomingSW4;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    bool sw1;
    bool sw2;
    bool sw3;
    bool sw4;
} struct_message;


// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;


// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Switch 1: ");Serial.println(incomingReadings.sw1);
  Serial.print("Switch 2: ");Serial.println(incomingReadings.sw2);
  Serial.print("Switch 3: ");Serial.println(incomingReadings.sw3);
  Serial.print("Switch 4: ");Serial.println(incomingReadings.sw4);
  digitalWrite(Relay1,incomingReadings.sw1);
  digitalWrite(Relay2,incomingReadings.sw2);
  digitalWrite(Relay3,incomingReadings.sw3);
  digitalWrite(Relay4,incomingReadings.sw4);
}



 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  
}
