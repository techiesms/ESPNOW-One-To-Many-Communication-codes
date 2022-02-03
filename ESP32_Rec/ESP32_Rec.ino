
#include <esp_now.h>
#include <WiFi.h>


#define R1 15
#define R2 2
#define R3 4
#define R4 22


//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  bool sw1;
  bool sw2;
  bool sw3;
  bool sw4;
} struct_message;

struct_message incomingReadings;

// Variable to store if sending data was successful
String success;


// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Switch 1: ");Serial.println(incomingReadings.sw1);
  Serial.print("Switch 2: ");Serial.println(incomingReadings.sw2);
  Serial.print("Switch 3: ");Serial.println(incomingReadings.sw3);
  Serial.print("Switch 4: ");Serial.println(incomingReadings.sw4);
  digitalWrite(R1,!incomingReadings.sw1);
  digitalWrite(R2,!incomingReadings.sw2);
  digitalWrite(R3,!incomingReadings.sw3);
  digitalWrite(R4,!incomingReadings.sw4);
}

void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.println("Code start");

  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(R3,OUTPUT);
  pinMode(R4,OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  for (int ii = 0; ii < 6; ++ii )
  {
    peerInfo.peer_addr[ii] = (uint8_t) broadcastAddress[ii];
  }
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

}

void loop()
{
  
}
