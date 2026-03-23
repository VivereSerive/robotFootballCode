#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

/*
! Controller Code for the Football Robot
* Uses senderESPNOW code
! ESP32 Board MAC Address: d8:13:2a:42:a6:20
TODO: https://projecthub.arduino.cc/hibit/using-joystick-module-with-arduino-0ffdd4
TODO: https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
*/

//! Variable Declerations

//* Joystick Variables
#define analogXPin 14 
#define analogYPin 27
#define analogButtonPin 26

// Default values when axis not actioned 
#define analogXCorrection 128 
#define analogYCorrection 128 

struct button{
  byte pressed = 0;
};

struct analog{
  short x, y;
  button button;
};

//* espNOW Variables

// Replace with reciever MAC address
uint8_t broadcastAddress[] = {0xD8, 0x13, 0x2A, 0x42, 0xA6, 0x20};

// Structure example to send data
// Must match the receiver structure
typedef struct structMessage {
  /*
  EXAMPLES:
  char a[32];
  int b;
  float c;
  bool d;
  */
} structMessage;

// Create a struct_message called myData
structMessage myData;

esp_now_peer_info_t peerInfo;

//! Code Starts Here 

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  //* ESPNOW
  Serial.begin(115200); // Init Serial Monitor
  WiFi.mode(WIFI_STA); // Set device as a Wi-Fi Station

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  //* Joystick
  pinMode(analogButtonPin, INPUT_PULLUP);
}

void loop() {
  /*
  Set values to send examples:
  Refer to your struct variable

  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  */
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  // Error Checking
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  delay(2000); // Delay, ought to remove.
}