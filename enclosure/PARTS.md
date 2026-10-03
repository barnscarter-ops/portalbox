# CYD enclosure — parts & procurement list

Buy list for the 3D-printed CYD enclosure + its battery power system.
Updated 2026-10-02 (sub-items resolved, pricing/links added, Mill-Max rating corrected).

## Resolved sub-items (were "open" as of 2026-10-01)

- **Fan 3-hole pattern — CONFIRMED.** Sunon MF25100V2-1000U-A99 datasheet:
  "Mounting holes Ø2.8 mm, 3 holes." The SCAD 3-hole fan mount is correct.
  Note: Digi-Key tags this as "VAPO" bearing while the datasheet says "MagLev" —
  same part number either way; confirm the exact bearing variant before the fan
  mount hole positions are locked in the SCAD.
- **Harwin pad "carrier" — the carrier is made, not bought.** The S70-125161545R
  is a bare SMT pad (2.5 × 1.6 × 0.15 mm, BeCu, gold flash, 6 A) with no leads.
  It solders onto a small carrier substrate (tiny PCB or copper-clad FR4 island)
  that also carries the solder land for the 24 AWG lead. That carrier is a build
  step, not a Harwin SKU, so it does **not** block ordering the pads.
- **Heat-set insert OD — 4.6 mm is standard; seat is correct.** Standard M3 brass
  heat-set insert = 4.6 mm OD, 4.0 mm pilot hole (McMaster / Albany County
  Fasteners), min boss OD 6.5 mm (our 10 × 10 mm boss is fine). `insert_seat_d`
  4.1 mm × 4.2 mm deep matches a standard 4.6 mm OD × ~4.0 mm "short" insert.
  **Buy the standard 4.6 mm OD insert, not the "slim" 4.0 mm OD variety.**

## ⚠️ Mill-Max 0947 pin — current rating corrected

The earlier "7 A → 5.6 A derated" claim was wrong on both numbers. Actual sources:

| Source | Rating |
|---|---|
| Digi-Key (standalone 0947) | **5.2 A** |
| Mill-Max pre-wired pin page | **6.5 A max / 5.2 A de-rated** |
| Mill-Max 824/826 header page (same pin in a connector) | **2 A continuous / 3 A max** |
| ~~Design docs (removed)~~ | ~~7 A → 5.6 A~~ |

"7 A" belongs to a *different* Mill-Max connector (827-22-010-10-020101). For
this discrete pin the defensible figures are **6.5 A max / 5.2 A de-rated**, but
Mill-Max's own statement for the pin mounted in a connector is **2 A continuous /
3 A max**. The design load is **2 A** — so treat 2 A as the ceiling and
bench-validate at 3 A (current-limited supply + dummy load) before trusting any
headroom. Contact resistance 20 mΩ max; ~60 gf mid-stroke force.

## Filament

- Bambu Lab **PETG-HF**, 1 kg — the enclosure parts. Order direct
  (us.store.bambulab.com) or Amazon. Printer ships with 4× PLA (supports + test).

## Power system

| Part | Spec | Link / price |
|---|---|---|
| Adafruit **328** LiPo | 3.7 V, 2500 mAh, 50 × 60 × 7.3 mm | adafruit.com/product/328 — **$14.95** (ground-ship only) |
| Adafruit **PowerBoost 1000C** | booster/charger, 45 × 23 × 10 mm | adafruit.com/product/2465 — **$19.95** (24 in stock) |
| Sunon **MF25100V2-1000U-A99** fan | 25 × 25 × 10 mm, 5 V, 65 mA, Ø2.8 mm × 3 holes | Digi-Key 7805270 — **$9.19** (21-wk lead); xonelec ~**$7.59** in stock |
| USB-C sink breakout | 5.1 kΩ CC termination (charge input) | adafruit.com/product/4090 — **$2.95** |

## Rail battery contacts (Codex design, commit `7200d7f`)

| Part | Qty | Rating / spec | Link / price |
|---|---|---|---|
| Mill-Max **0947-0-15-20-77-14-11-0** solder-cup pogo pin | 2 (+2 spare) | 6.5 A max / 5.2 A de-rated (see flag); 2.286 mm stroke; 24 AWG cup | Digi-Key 7402798 — **$1.78** |
| Harwin **S70-125161545R** gold contact pad | 2 (+2 spare) | 6 A, 2.5 × 1.6 × 0.15 mm | Digi-Key 6211247 — **$0.52** (74k stock) |
| ISO 4762 **M3×12** socket-head screw | 5 (bezel ×4 + lock ×1) | Ø5.8 × 3.2 mm head counterbore | Amazon assortment below |
| **M3 heat-set insert** | 1 (lock) + 4 (bezel columns) | 4.6 mm OD × ~4.0 mm, standard | Amazon pack below |
| 24 AWG stranded insulated wire | 4 runs | internal leads | Amazon kit below |

## Fasteners (Amazon)

- **M3 heat-set inserts, 4.6 mm OD × 4.0 mm** — e.g. ruthex RX-M3Sx4.0 (100 pc)
  or BIFROST "M3 × 4.0 mm, OD 4.6 mm" — **~$11–12 / 100 pc**. Standard OD, not slim.
- **M3 socket-head screw assortment** (×4/×6/×8/×10/×12, 304 SS, 250 pc,
  head Ø5.5 × 3.0) — **~$12.59** (Amazon B0BJQ37JQT). Covers the bezel ×4 + lock ×1
  M3×12 plus spares.

## Connectors / misc (Amazon)

- SPDT slide switch (for PowerBoost EN — low-current signal) — Chanzon 25 pc ~**$4.99**
- 24 AWG stranded silicone wire, red+black, 10 ft ea + heat-shrink — **~$8.59** (Fermerry)
- Heat-shrink assortment (comes with most wire kits)
- *(optional)* silicone stylus tether

## Blocked / TBD

- **Pad carriers** — custom (tiny PCB or copper-clad FR4 island + solder land).
  Build step; fabricate to the 1.2 mm axial × 2.7 mm width envelope after the
  pads arrive. Not an ordered part.
- **Source isolation** — SOLVED per handoff: CYD's Schottky D1 (B5819W) blocks
  backfeed; inject booster 5 V at VCC5V (D1 cathode), never VBUS. Verify D1/Q3
  orientation on the real board with a meter before wiring.

## Already owned (don't buy)

22 AWG solid wire · solder · iron · multimeter · DHT11 (Keyestudio kit) ·
ESP32-S3 Super Mini ×5 · Bambu P1S + AMS + 4× PLA.
