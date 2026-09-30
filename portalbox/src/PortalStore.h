#pragma once
#include <FS.h>
#include <LittleFS.h>
#include <SD.h>
#include "AppConfig.h"

namespace PortalStore {

// function-local static: one shared instance across all translation units
inline bool& _sdFlag() { static bool v = false; return v; }

inline bool        usingSD()     { return _sdFlag(); }
inline const char* backendName() { return _sdFlag() ? "sd" : "flash"; }
inline fs::FS&     store()       { return _sdFlag() ? (fs::FS&)SD : (fs::FS&)LittleFS; }

inline String baseName(const String& p) {
    int s = p.lastIndexOf('/');
    return s >= 0 ? p.substring(s + 1) : p;
}

inline String portalPath(const String& n) { return String(PORTAL_DIR) + "/" + n; }
inline bool   exists(const String& n)     { return store().exists(portalPath(n)); }
inline bool   remove(const String& n)     { return store().remove(portalPath(n)); }

inline size_t size(const String& n) {
    File f = store().open(portalPath(n), "r");
    if (!f) return 0;
    size_t s = f.size();
    f.close();
    return s;
}

inline uint64_t totalBytes() { return _sdFlag() ? SD.totalBytes() : LittleFS.totalBytes(); }
inline uint64_t usedBytes()  { return _sdFlag() ? SD.usedBytes()  : LittleFS.usedBytes(); }

template <typename Fn>
inline void list(Fn emit) {
    File dir = store().open(PORTAL_DIR);
    if (!dir) return;
    for (File f = dir.openNextFile(); f; f = dir.openNextFile()) {
        if (!f.isDirectory()) emit(baseName(f.name()), f.size());
        f.close();
    }
    dir.close();
}

inline bool validName(const String& n) {
    if (n.length() == 0 || n.length() > 40) return false;
    if (n.indexOf("..") >= 0) return false;
    for (unsigned i = 0; i < n.length(); i++) {
        char c = n[i];
        if (!(isalnum(c) || c == '.' || c == '-' || c == '_')) return false;
    }
    return true;
}

inline void _seedFromFlash() {
    // first boot with a card: copy the LittleFS portal library onto the SD
    File dir = store().open(PORTAL_DIR);
    bool empty = true;
    if (dir) {
        File f = dir.openNextFile();
        empty = !f;
        if (f) f.close();
        dir.close();
    }
    if (!empty) return;

    File src = LittleFS.open(PORTAL_DIR);
    if (!src) return;
    for (File f = src.openNextFile(); f; f = src.openNextFile()) {
        if (f.isDirectory()) { f.close(); continue; }
        String dstPath = String(PORTAL_DIR) + "/" + baseName(f.name());
        File out = SD.open(dstPath, "w");
        if (out) {
            while (f.available()) out.write(f.read());
            out.close();
        }
        f.close();
    }
    src.close();
}

inline bool begin() {
    LittleFS.begin(true);            // flash always mounts: fallback + seed source
    _sdFlag() = SD.begin(SD_CS);     // onboard slot on VSPI
    if (!store().exists(PORTAL_DIR))  store().mkdir(PORTAL_DIR);
    if (!store().exists(CAPTURE_DIR)) store().mkdir(CAPTURE_DIR);
    if (_sdFlag()) _seedFromFlash();
    return true;
}
}
