#pragma once
#include <Arduino.h>

enum class RunState { IDLE, PORTAL_UP };

struct AppState {
    RunState state      = RunState::IDLE;
    String   ssid;
    uint8_t  channel    = 0;
    String   portal;               // active portal filename
    uint32_t captures   = 0;
    uint32_t startedAt  = 0;       // millis
    long     timeOffset = 0;       // epoch - millis()/1000, set via `time set`
    bool     beep       = true;
    uint32_t epoch() const {
        return timeOffset ? (uint32_t)(timeOffset + millis() / 1000) : 0;
    }
};
extern AppState g_state;
