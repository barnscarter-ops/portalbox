#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "AppState.h"

// 480x320 landscape status UI for the CYD 3.5" (ST7796)
namespace DisplayUI {

inline TFT_eSPI& T() { static TFT_eSPI t; return t; }

inline void _label(int x, int y, const char* s) {
    T().setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    T().setTextSize(2);
    T().setCursor(x, y);
    T().print(s);
}

inline void _value(int x, int y, int w, const String& s, uint16_t col = TFT_WHITE) {
    T().fillRect(x, y, w, 18, TFT_BLACK);
    T().setTextColor(col, TFT_BLACK);
    T().setTextSize(2);
    T().setCursor(x, y);
    T().print(s);
}

inline void _pill(bool up) {
    uint16_t c = up ? TFT_DARKGREEN : TFT_MAROON;
    T().fillRoundRect(352, 6, 116, 24, 5, c);
    T().setTextColor(TFT_WHITE, c);
    T().setTextSize(2);
    T().setCursor(368, 10);
    T().print(up ? "RUNNING" : "IDLE");
}

inline void begin() {
    T().init();
    T().setRotation(1);            // 480 x 320 landscape
    T().fillScreen(TFT_BLACK);

    T().fillRect(0, 0, 480, 34, TFT_NAVY);
    T().setTextColor(TFT_WHITE, TFT_NAVY);
    T().setTextSize(2);
    T().setCursor(12, 9);
    T().print("portalbox");
    _pill(false);

    _label(16,  52, "ssid");
    _label(16,  90, "ap ip / ch");
    _label(16, 128, "portal");
    _label(16, 166, "clients");
    _label(250, 166, "captures");
    _label(16, 204, "uptime");

    T().drawFastHLine(16, 240, 448, TFT_DARKGREY);
    _label(16, 252, "last capture");

    T().setTextColor(TFT_DARKGREY, TFT_BLACK);
    T().setTextSize(1);
    T().setCursor(16, 304);
    T().print("uart 115200 8n1  rx=22 tx=21   store:");
}

inline void setBackend(const char* b) {
    T().setTextColor(TFT_DARKGREY, TFT_BLACK);
    T().setTextSize(1);
    T().setCursor(300, 304);
    T().print(b);
}

inline void setClients(int n);

inline void onState(bool up, const String& ssid, uint8_t ch, const String& ip) {
    _pill(up);
    _value(140, 52, 320, up ? ssid : "-", up ? TFT_GREEN : TFT_LIGHTGREY);
    _value(140, 90, 320, up ? (ip + "  ch " + String(ch)) : "-");
    if (!up) setClients(0);
}

inline void setPortal(const String& p)  { _value(140, 128, 320, p.length() ? p : "-", TFT_CYAN); }
inline void setClients(int n)           { _value(140, 166, 90, String(n), TFT_YELLOW); }
inline void setCaptures(uint32_t n)     { _value(390, 166, 80, String(n), TFT_ORANGE); }

inline void setUptime(uint32_t s) {
    char buf[20];
    snprintf(buf, sizeof buf, "%02lu:%02lu:%02lu",
             (unsigned long)(s / 3600), (unsigned long)((s / 60) % 60), (unsigned long)(s % 60));
    _value(140, 204, 200, buf);
}

inline void onCapture(const String& email) {
    String e = email;
    if (e.length() > 30) e = e.substring(0, 30) + "..";
    _value(140, 252, 330, e, TFT_GREENYELLOW);
}
}
