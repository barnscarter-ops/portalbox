# CYD enclosure — parts & procurement list

Buy list for the 3D-printed CYD enclosure + its battery power system.
Updated 2026-10-01 (rail-carried battery contacts integrated, commit `7200d7f`).

## Filament

- Bambu Lab **PETG-HF**, 1 kg — the enclosure parts. Printer ships with 4× PLA
  (supports + test prints).

## Power system

| Part | Spec | Link |
|---|---|---|
| Adafruit **328** LiPo | 3.7 V, 2500 mAh, 60×50×7.3 mm | adafruit.com/product/328 |
| Adafruit **PowerBoost 1000C** | booster/charger, 45×23×10 mm | adafruit.com/product/2465 |
| Sunon **MF25100V2-1000U-A99** fan | 25×25×10 mm, 5 V, 65 mA | Digi-Key 7805270 *(candidate — confirm 3-hole pattern)* |
| USB-C sink breakout | 5.1 kΩ CC termination (charge input) | Adafruit 4090 or generic |

## Rail battery contacts (Codex design, commit `7200d7f`)

| Part | Qty | Rating / spec |
|---|---|---|
| Mill-Max **0947-0-15-20-77-14-11-0** solder-cup pogo pin | 2 | 7 A (derated 5.6 A), 2.286 mm stroke |
| Harwin **S70-125161545R** gold contact pad | 2 | 6 A, 2.5×1.6×0.15 mm |
| ISO 4762 **M3×12** socket-head screw | 1 | pod lock-tab |
| DIN 934 **M3** hex nut | 1 | captive nut |
| 24 AWG stranded insulated wire | 4 runs | internal leads |

## Fasteners

- M3 heat-set inserts (4.2 mm OD × ~4 mm) — one pack
- M3 screws — small assortment (M3×8 / ×10 / ×12)

## Connectors / misc

- Slide switch (SPDT)
- Heat-shrink assortment
- *(optional)* silicone stylus tether

## Blocked / TBD

- Harwin **pad carriers** — provisional, match the delivered pads + harness.
- **Source isolation / power mux** — circuit + parts still to be selected
  (reverse-current blocking between boosted 5 V and CYD USB VBUS).

## Already owned (don't buy)

22 AWG solid wire · solder · iron · multimeter · DHT11 (Keyestudio kit) ·
ESP32-S3 Super Mini ×5 · Bambu P1S + AMS + 4× PLA.
