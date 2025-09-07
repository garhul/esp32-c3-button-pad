#include <Arduino.h>

// #include "Network.h"

#include "Pad.h"
#include "WiFi.h"
#include "esp_now.h"
// #include "Server.h"
// #include "Settings.h"

enum CMD {
  CMD_OFF,
  CMD_SET_FX,
  CMD_SET_FX_BR,
  CMD_SET_FX_SPEED,
  CMD_PAUSE,
  CMD_RESUME,
  CMD_SET_HSB,
  CMD_SET_RGB,
};

// PCF8575 pcf8575(0x27, INTERRUPT_PIN, onInterrupt);

// 5C:CF:7F:AC:B6:2D
// 10:52:1C:02:46:4E
// 5C:CF:7F:AC:AB:16
// 60:01:94:5E:2E:BE
uint8_t peers[][6] = {
    // {0x5C, 0xCF, 0x7F, 0xAC, 0xB6, 0x2D},
    // {0x10, 0x52, 0x1C, 0x02, 0x46, 0x4E},
    {0x5C, 0xCF, 0x7F, 0xAC, 0xAB, 0x16},
    {0x60, 0x01, 0x94, 0x5E, 0x2E, 0xBE}};

esp_now_peer_info_t peerInfo;

typedef struct auroraMsg {
  uint8_t cmd;
  uint8_t payload[32];
} auroraMsg;

auroraMsg data;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) {
  Serial.printf("Last Packet Send Status: [mac:%02x:%02x:%02x:%02x:%02x:%02x] -> ", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

  if (sendStatus == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Ok");
  } else {
    Serial.println("Fail");
  }
}

void setup() {
  Serial.begin(460800);
  // Settings::init();
  delay(1000);
  Pad::init();
  Pad::clear(5, 0, 0);
  Serial.println("Pad ready");

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("ESP-NOW initialized");
  }

  esp_now_register_send_cb(OnDataSent);

  for (uint8_t i = 0; i < sizeof(peers) / 6; i++) {
    memcpy(peerInfo.peer_addr, peers[i], 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    } else {
      Serial.println("Peer added");
    }
  }

  // Network::init();
  // Serial.println("Network ready");

  // MyServer::init();
  // Serial.println("Server ready");
}

void loop() {
  static uint16_t padState = 0;

  if (Pad::isDirty()) {
    padState = Pad::getState();
    Serial.println(padState, BIN);

    if (~padState & 0b0000000000000001) {
      data.cmd = CMD_OFF;
      Serial.println("Off");
    }

    if (~padState & 0b0000000000000010) {
      data.cmd = CMD_SET_FX;
      data.payload[0] = 0x01;
      Serial.println("Fx 1");
    }

    if (~padState & 0b0000000000000100) {
      data.cmd = CMD_SET_FX;
      data.payload[0] = 0x02;
      Serial.println("Fx 2");
    }

    if (~padState & 0b0000000000001000) {
      data.cmd = CMD_SET_FX;
      data.payload[0] = 0x03;
      Serial.println("Fx 3");
    }

    if (~padState & 0b0000000000010000) {
      data.cmd = CMD_SET_FX;
      data.payload[0] = 0x04;
      Serial.println("Fx 4");
    }

    if (~padState & 0b0000000000100000) {
      data.cmd = CMD_SET_FX;
      data.payload[0] = 0x05;
      Serial.println("Fx 5");
    }

    if (~padState & 0b0000000001000000) {
      data.cmd = CMD_SET_FX;
      data.payload[0] = 0x06;
      Serial.println("Fx 6");
    }

    if (~padState & 0b0000000010000000) {
      data.cmd = CMD_SET_HSB;
      data.payload[0] = 0xFF;
      data.payload[1] = 0xFF;
      data.payload[2] = 0x10;

      Serial.println("WHITE");
    }

    // Send message via ESP-NOW
    // for (uint8_t i = 0; i < 32; i++) {
    //   Serial.printf("%02X ", data.payload[i]);
    // }

    esp_now_send(NULL, (uint8_t *)&data, sizeof(data));  // broadcast to all registered peers
  }
}