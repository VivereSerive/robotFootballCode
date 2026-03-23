#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

/*
! Controller Code for the Football Robot
* Uses senderESPNOW code
! ESP32 Board MAC Address: d8:13:2a:42:a6:20
! Note: read value is mapped to [0-255] range to represent it with only 1 byte. 
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

struct buttonStruct{
  byte pressed = 0;
};

struct analog{
  short x, y;
  buttonStruct button;
};

byte readAnalogAxisLevel(int pin){
  return map(analogRead(pin), 0, 1023, 0, 235);
}

bool isAnalogButtonPressed(int pin){
  return digitalRead(pin) == 0;
}

//* ESPNOW Variables

// // Replace with reciever MAC address
// uint8_t broadcastAddress[] = {0xD8, 0x13, 0x2A, 0x42, 0xA6, 0x20};

// // Structure example to send data
// // Must match the receiver structure
// typedef struct structMessage {
//   /*
//   EXAMPLES:
//   char a[32];
//   int b;
//   float c;
//   bool d;
//   */
// } structMessage;

// // Create a struct_message called myData
// structMessage myData;

// esp_now_peer_info_t peerInfo;

//! Code Starts Here 

// // callback when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

void setup() {
  Serial.begin(115200); // Init Serial Monitor
  //* ESPNOW
  // WiFi.mode(WIFI_STA); // Set device as a Wi-Fi Station

  // // Init ESP-NOW
  // if (esp_now_init() != ESP_OK) {
  //   Serial.println("Error initializing ESP-NOW");
  //   return;
  // }

  // // Once ESPNow is successfully Init, we will register for Send CB to
  // // get the status of Trasnmitted packet
  // esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // // Register peer
  // memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  // peerInfo.channel = 0;  
  // peerInfo.encrypt = false;
  
  // // Add peer        
  // if (esp_now_add_peer(&peerInfo) != ESP_OK){
  //   Serial.println("Failed to add peer");
  //   return;
  // }

  //* Joystick
  pinMode(analogButtonPin, INPUT_PULLUP);
}

void loop() {
  //* Read Joystick State
  analog analog;

  analog.x = readAnalogAxisLevel(analogXPin) - analogXCorrection;
  analog.y = readAnalogAxisLevel(analogYPin) - analogYCorrection;

  analog.button.pressed = isAnalogButtonPressed(analogButtonPin);

  if (analog.button.pressed){
    Serial.println("Button Pressed");
  } else {
    Serial.println("Button Not Pressed");
  }

  Serial.print("Y: ");
  Serial.println(analog.y);
  Serial.print("X: ");
  Serial.println(analog.x);

  //* Assign Joystick Variables to message structure
  /*  
  Set values to send examples:
  Refer to your struct variable

  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  */
  
  // //* Send message via ESP-NOW
  // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  // //* Error Checking
  // if (result == ESP_OK) {
  //   Serial.println("Sent with success");
  // }
  // else {
  //   Serial.println("Error sending the data");
  // }

  delay(200); //TODO: Delay, ought to remove.
}