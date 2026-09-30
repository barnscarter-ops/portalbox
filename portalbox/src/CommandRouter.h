#pragma once
#include <Arduino.h>
#include <FS.h>
#include "AppConfig.h"
#include "AppState.h"
#include "PortalStore.h"
#include "CaptureLog.h"
#include "CaptivePortal.h"
#include "DisplayUI.h"

class CommandRouter {
public:
    CommandRouter(CaptivePortal& portal) : _portal(portal) {}

    void begin() {
        _ports[0] = &CMDSER;   // Flipper
        _ports[1] = &Serial;   // USB debug console accepts the same commands
        CMDSER.printf("EVT boot,portalbox,%s,%s\r\n", FW_VERSION, PortalStore::backendName());
    }

    void handle() {
        for (uint8_t i = 0; i < 2; i++) _feed(i);
    }

private:
    CaptivePortal& _portal;
    Stream* _ports[2];
    String  _line[2];

    void _feed(uint8_t idx) {
        Stream& io = *_ports[idx];
        while (io.available()) {
            char c = (char)io.read();
            if (c == '\n') {
                String l = _line[idx];
                _line[idx] = "";
                l.trim();
                if (l.length()) _exec(io, l);
            } else if (c != '\r') {
                if (_line[idx].length() < 512) _line[idx] += c;
            }
        }
    }

    void _exec(Stream& io, String line) {
        int sp = line.indexOf(' ');
        String verb = sp < 0 ? line : line.substring(0, sp);
        String rest = sp < 0 ? "" : line.substring(sp + 1);
        verb.toLowerCase();

        if (verb == "ping") { io.printf("OK pong %s\r\n", FW_VERSION); }
        else if (verb == "help") {
            io.println(F("OK cmds: ping status help | ap ssid <name..> | ap ch <n> | ap start | ap stop | "
                         "portal list|select <n>|delete <n>|push <n> <bytes> | "
                         "capture dump|clear | beep on|off | time set <epoch>"));
        }
        else if (verb == "status") {
            io.printf("OK state=%s ssid=\"%s\" ch=%u ip=%s portal=%s captures=%lu store=%s fs=%lu/%lu\r\n",
                g_state.state == RunState::PORTAL_UP ? "up" : "idle",
                g_state.ssid.c_str(), g_state.channel,
                WiFi.softAPIP().toString().c_str(),
                g_state.portal.length() ? g_state.portal.c_str() : "-",
                (unsigned long)g_state.captures,
                PortalStore::backendName(),
                (unsigned long)PortalStore::usedBytes(),
                (unsigned long)PortalStore::totalBytes());
        }
        else if (verb == "time") {
            long e = (long)rest.toInt();
            g_state.timeOffset = e ? (e - (long)(millis() / 1000)) : 0;
            io.printf("OK epoch=%lu\r\n", (unsigned long)g_state.epoch());
        }
        else if (verb == "beep") {
            if (rest == "on")       { g_state.beep = true;  io.println("OK beep on"); }
            else if (rest == "off") { g_state.beep = false; io.println("OK beep off"); }
            else io.println("ERR beep on|off");
        }
        else if (verb == "ap") {
            int sp2 = rest.indexOf(' ');
            String sub = sp2 < 0 ? rest : rest.substring(0, sp2);
            String arg = sp2 < 0 ? "" : rest.substring(sp2 + 1);
            if (sub == "ssid")      { g_state.ssid = arg; io.println("OK ssid set"); }
            else if (sub == "ch")   { g_state.channel = (uint8_t)arg.toInt(); io.println("OK ch set"); }
            else if (sub == "start") {
                _portal.start(g_state.ssid.length() ? g_state.ssid : DEFAULT_SSID,
                              g_state.channel ? g_state.channel : DEFAULT_CH);
                io.println("OK ap up");
            }
            else if (sub == "stop") { _portal.stop(); io.println("OK ap down"); }
            else io.println("ERR ap subcmd");
        }
        else if (verb == "portal") {
            int sp2 = rest.indexOf(' ');
            String sub = sp2 < 0 ? rest : rest.substring(0, sp2);
            String arg = sp2 < 0 ? "" : rest.substring(sp2 + 1);
            if (sub == "list") {
                PortalStore::list([&](const String& n, size_t s) {
                    io.printf("PORTAL %s %u\r\n", n.c_str(), (unsigned)s);
                });
                io.println("OK");
            }
            else if (sub == "select") {
                if (PortalStore::validName(arg) && PortalStore::exists(arg)) {
                    g_state.portal = arg;
                    DisplayUI::setPortal(arg);
                    io.println("OK selected");
                } else io.println("ERR no such portal");
            }
            else if (sub == "delete") {
                io.println(PortalStore::validName(arg) && PortalStore::remove(arg)
                           ? "OK deleted" : "ERR delete");
            }
            else if (sub == "push") {
                int sp3 = arg.indexOf(' ');
                if (sp3 < 0) { io.println("ERR push args"); return; }
                _push(io, arg.substring(0, sp3), (size_t)arg.substring(sp3 + 1).toInt());
            }
            else io.println("ERR portal subcmd");
        }
        else if (verb == "capture") {
            if (rest == "dump")       CaptureLog::dump(io);
            else if (rest == "clear") { CaptureLog::clear(); io.println("OK cleared"); }
            else io.println("ERR capture subcmd");
        }
        else io.println("ERR unknown");
    }

    // Binary upload: `portal push Name.html <size>` -> READY -> raw bytes -> OK/ERR
    void _push(Stream& io, String name, size_t n) {
        if (!PortalStore::validName(name) || n == 0 || n > MAX_PORTAL_BYTES) {
            io.println("ERR push rejected");
            return;
        }
        io.println("READY");
        File f = PortalStore::store().open(PortalStore::portalPath(name), "w");
        if (!f) { io.println("ERR fs"); return; }
        uint32_t sum = 0;
        size_t got = 0;
        uint32_t deadline = millis() + 15000;
        while (got < n && millis() < deadline) {
            if (io.available()) {
                int b = io.read();
                if (b >= 0) {
                    f.write((uint8_t)b);
                    sum += (uint8_t)b;
                    got++;
                    deadline = millis() + 3000;
                }
            }
            yield();
        }
        f.close();
        if (got == n) io.printf("OK push %s %u %lu\r\n", name.c_str(), (unsigned)got, (unsigned long)sum);
        else {
            PortalStore::store().remove(PortalStore::portalPath(name));
            io.printf("ERR timeout %u/%u\r\n", (unsigned)got, (unsigned)n);
        }
    }
};
