#include <esp_now.h>
#include <WiFi.h>
#include "xbm.h"
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress0[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Universal
uint8_t broadcastAddress1[] = {0x80, 0x7D, 0x3A, 0x6E, 0x8D, 0x23}; // NodeMCU
uint8_t broadcastAddress2[] = {0x84, 0xCC, 0xA8, 0x5E, 0x5D, 0x3C}; // ESP32

uint16_t calData[5] = { 520, 3372, 831, 2476, 7 }; // callibration point for touch screen

int board = 0; // Setting board settings to both devices

// Switch position and size
#define FRAME_X 20
#define FRAME_Y 10
#define FRAME_W 120
#define FRAME_H 50

// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

// Green zone size
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

#define X_Button_Margin 150
#define Y_Button_Margin 100


// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message 
{
  bool sw1;
  bool sw2;
  bool sw3;
  bool sw4;
} struct_message;

// Create a struct_message called switch_reading to hold sensor readings
struct_message both;
struct_message nodemcu;
struct_message esp32;


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  }
  else {
    success = "Delivery Fail :(";
  }
}


void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.println("Code start");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  for (int ii = 0; ii < 6; ++ii )
  {
    peerInfo.peer_addr[ii] = (uint8_t) broadcastAddress0[ii];
  }
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  for (int ii = 0; ii < 6; ++ii )
  {
    peerInfo.peer_addr[ii] = (uint8_t) broadcastAddress1[ii];
  }
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  for (int ii = 0; ii < 6; ++ii )
  {
    peerInfo.peer_addr[ii] = (uint8_t) broadcastAddress2[ii];
  }

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  tft.init();

  tft.setRotation(3);

  // clear screen
  tft.fillScreen(TFT_BLACK);

  buttons(); // print buttons on screen
}

void loop() {

  uint16_t x, y;

  // See if there's any touch data for us
  if (tft.getTouch(&x, &y))
  {


    Serial.print("x - "); Serial.println(x);
    Serial.println("y - "); Serial.println(y);


    if ((x > REDBUTTON_X + X_Button_Margin) && (x < (REDBUTTON_X + REDBUTTON_W + X_Button_Margin ) )) {
      if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
        Serial.println("Switch 1 OFF");

        // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw1 = 0;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw1 = 0;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw1 = 0;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }
      }
    }

    if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
      if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
        Serial.println("Switch 2 OFF");
        // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw2 = 0;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw2 = 0;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw2 = 0;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }
      }
    }

    if ((x > REDBUTTON_X + X_Button_Margin) && (x < (REDBUTTON_X + REDBUTTON_W + X_Button_Margin ) )) {
      if ((y > REDBUTTON_Y + Y_Button_Margin ) && (y <= ((REDBUTTON_Y + Y_Button_Margin) + REDBUTTON_H))) {
        Serial.println("Switch 3 OFF");
        // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw3 = 0;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw3 = 0;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw3 = 0;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }
      }
    }

    if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
      if ((y > REDBUTTON_Y + Y_Button_Margin ) && (y <= (REDBUTTON_Y + REDBUTTON_H + Y_Button_Margin))) {
        Serial.println("Switch 4 OFF");
        // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw4 = 0;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw4 = 0;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw4 = 0;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }
      }
    }

    {
      if ((x > GREENBUTTON_X + X_Button_Margin ) && (x < (GREENBUTTON_X + GREENBUTTON_W + X_Button_Margin ))) {
        if ((y > GREENBUTTON_Y) && (y <= (REDBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Switch 1 ON");
          // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw1 = 1;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw1 = 1;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw1 = 1;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }

        }
      }

      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (REDBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Switch 2 ON");
           // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw2 = 1;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw2 = 1;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw2 = 1;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }

        }
      }

      if ((x > GREENBUTTON_X + X_Button_Margin ) && (x < (GREENBUTTON_X + GREENBUTTON_W + X_Button_Margin ))) {
        if ((y > GREENBUTTON_Y + Y_Button_Margin) && (y <= (REDBUTTON_Y + GREENBUTTON_H + Y_Button_Margin))) {
          Serial.println("Switch 3 ON");
            // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw3 = 1;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw3 = 1;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw3 = 1;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }

        }
      }
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y + Y_Button_Margin) && (y <= (REDBUTTON_Y + GREENBUTTON_H + Y_Button_Margin))) {
          Serial.println("Switch 4 ON");
            // Send message via ESP-NOW
        if (board == 0)
        {
          both.sw4 = 1;
          esp_err_t result = esp_now_send(broadcastAddress0, (uint8_t *) &both, sizeof(struct_message));
        }
        if (board == 1)
        {
          nodemcu.sw4 = 1;
          esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &nodemcu, sizeof(struct_message));
        }
        if (board == 2)
        {
          esp32.sw4 = 1;
          esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &esp32, sizeof(struct_message));
        }

        }
      }

      if ((x > 0) && (x < 107))
      {
        if ((y > 160) && (y <= 240)) {
          Serial.println("Both Selected");
          tft.fillRect(70, 60, 120, 30, TFT_BLACK);
          tft.setTextColor(TFT_WHITE);
          tft.drawString("Both", 150, 80);
          board = 0; // All devices Selected
        }
      }

      if ((x > 107) && (x < 210))
      {
        if ((y > 160) && (y <= 240)) {
          Serial.println("NodeMCU Selected");
          tft.fillRect(70, 60, 120, 30, TFT_BLACK);
          tft.setTextColor(TFT_WHITE);
          tft.drawString("NodeMCU", 140, 80);
          board = 1; // NodeMCU Selected
        }
      }

      if ((x > 210) && (x < 320))
      {
        if ((y > 160) && (y <= 240)) {
          Serial.println("ESP32 Selected");
          tft.fillRect(70, 60, 120, 30, TFT_BLACK);
          tft.setTextColor(TFT_WHITE);
          tft.drawString("ESP32", 150, 80);
          board = 2; // ESP32 Selected
        }
      }
    }

  }
  else
  {
    print_data_on_screen();
  }
}



void buttons()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", GREENBUTTON_X + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X) + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));

  tft.fillRect((REDBUTTON_X + X_Button_Margin ), REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect((GREENBUTTON_X + X_Button_Margin), GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", (GREENBUTTON_X + X_Button_Margin) + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X + X_Button_Margin) + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));

  tft.fillRect(REDBUTTON_X, (REDBUTTON_Y + Y_Button_Margin) , REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect(GREENBUTTON_X, (GREENBUTTON_Y + Y_Button_Margin) , GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", GREENBUTTON_X + (GREENBUTTON_W / 2), ( GREENBUTTON_Y + Y_Button_Margin) + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X) + (REDBUTTON_W / 2) + 1, (REDBUTTON_Y + Y_Button_Margin) + (REDBUTTON_H / 2));


  tft.fillRect((REDBUTTON_X + X_Button_Margin ), (REDBUTTON_Y + Y_Button_Margin), REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect((GREENBUTTON_X + X_Button_Margin), (GREENBUTTON_Y + Y_Button_Margin), GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", (GREENBUTTON_X + X_Button_Margin) + (GREENBUTTON_W / 2), (GREENBUTTON_Y + Y_Button_Margin) + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X + X_Button_Margin) + (REDBUTTON_W / 2) + 1, (REDBUTTON_Y + Y_Button_Margin) + (REDBUTTON_H / 2));

  tft.setTextColor(TFT_WHITE);
  tft.drawString(" ", 150, 80);




}

void print_data_on_screen()
{
  tft.fillRect(0, 180, 100, 60, TFT_GREEN);
  tft.fillRect(107, 180, 100, 60, TFT_RED);
  tft.fillRect(210, 180, 100, 60, TFT_BLUE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ESP32 " , 50, 200);
  tft.drawString("NodeMCU " , 165, 200);
  tft.drawString("Both " , 265, 200);
}
