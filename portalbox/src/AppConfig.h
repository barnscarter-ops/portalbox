#pragma once
#include <Arduino.h>

#define FW_VERSION "0.2"

// Flipper command link - UART2 via the IO21 JST connector
#define CMDSER Serial2
static const uint32_t CMD_BAUD   = 115200;
static const int      CMD_RX_PIN = 22;   // ESP RX2  <- Flipper pin 13 (TX)
static const int      CMD_TX_PIN = 21;   // ESP TX2  -> Flipper pin 14 (RX)

// Onboard microSD slot (VSPI: SCK=18, MISO=19, MOSI=23)
static const int SD_CS = 5;

// Onboard speaker amp
static const int SPKR_PIN = 26;

static const char*   PORTAL_DIR       = "/portals";
static const char*   CAPTURE_DIR      = "/captures";
static const size_t  MAX_PORTAL_BYTES = 64 * 1024;  // streamed from disk, no 20k rule
static const char*   DEFAULT_SSID     = "Free WiFi";
static const uint8_t DEFAULT_CH       = 6;
