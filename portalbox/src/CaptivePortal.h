#pragma once
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include "AppState.h"
#include "PortalStore.h"
#include "CaptureLog.h"

class CaptivePortal {
public:
    bool start(const String& ssid, uint8_t ch);
    void stop();
    void handle();  // call every loop
private:
    WebServer _server{80};
    DNSServer _dns;
    bool      _eventsRegistered = false;

    void _routes();
    void _servePortal();
    void _handleGet();
    void _redirectToPortal();
    void _registerEvents();
};
