#include <Arduino.h>
#include "AppConfig.h"
#include "AppState.h"
#include "PortalStore.h"
#include "DisplayUI.h"
#include "CaptivePortal.h"
#include "CommandRouter.h"

AppState g_state;
CaptivePortal portal;
CommandRouter cmds(portal);

void setup() {
    Serial.begin(115200);                                            // USB-C debug console
    CMDSER.begin(CMD_BAUD, SERIAL_8N1, CMD_RX_PIN, CMD_TX_PIN);      // Flipper link (IO21 JST)

    DisplayUI::begin();
    PortalStore::begin();                                            // SD if present, else flash
    DisplayUI::setBackend(PortalStore::backendName());

    cmds.begin();
    Serial.println("portalbox up");
}

void loop() {
    static uint32_t lastTick = 0;
    if (millis() - lastTick >= 1000) {
        lastTick = millis();
        DisplayUI::setUptime(millis() / 1000);
        if (g_state.state == RunState::PORTAL_UP)
            DisplayUI::setClients(WiFi.softAPgetStationNum());
    }
    cmds.handle();
    portal.handle();
}
