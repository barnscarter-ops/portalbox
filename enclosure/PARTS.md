# CYD enclosure — parts & procurement list

Buy list for the 3D-printed CYD enclosure + its battery power system.
Updated 2026-10-04 (Amazon orders reconciled, wire/heat-shrink owned, Amazon power parts chosen, Digi-Key cart staged).

## Order status (2026-10-04)

| Item | Status | Where |
|---|---|---|
| M3 heat-set inserts (5) | **Ordered** — INCLY 720 pc kit, delivered 10-03. Confirm the M3 size is 4.6 mm OD (not slim) when opened | Amazon 114-8967031-6625069 |
| M3×12 screws (5) | **Ordered** — 2280 pc M2–M5 button-head hex kit; Carter confirmed 12 mm length present. Button head is shallower than the ISO 4762 counterbore (Ø5.8 × 3.2), so it sits fine | Amazon 114-8967031-6625069 |
| SPDT slide switch | **Ordered** — Tnuocke 120 pc kit | Amazon 114-8967031-6625069 |
| PETG filament | **Ordered** — ELEGOO PETG 4 kg + 1 kg black PETG (substitute for Bambu PETG-HF) | Amazon 114-8967031-6625069 |
| Printer, caliper, deburring tool | **Ordered / delivered** — P1S + AMS, digital caliper, deburring set | Amazon |
| 24 AWG stranded wire + heat-shrink | **Complete** — owned (Grizzly stock) | — |
| LiPo battery 3.7 V ~2500 mAh | **To buy** — Amazon B0C2KP2PP6, 605060, 6 × 50 × 61 mm (fits 60 × 50 × 7.3 pocket), $9.99. Listing does not state a protection circuit — check for a PCM board at the leads before use; cut JST, leads go to pad carriers | Amazon cart link |
| Charger/booster (replaces PowerBoost 1000C) | **To buy** — Amazon B0GRV7F1LJ, 5 pc Type-C 5V 2A lithium charge + boost module, $7.59, arrives 2026-10-05. The PowerBoost listing (B01BMRBTH2) ships in ~2 weeks, so it was dropped. Alt: B0BYMC7XBZ, 4 pc IP5310 3.1A with key switch, $11.89, arrives 10-08. **Not a drop-in:** onboard USB-C replaces the separate USB-C breakout (no 5.1 kΩ breakout needed) and there is no EN pin, so the slide switch and WIRING.md EN/BAT/USB pad plan must be re-checked against the board's actual pads and the SCAD pocket (was 45 × 23 × 10). Verify pin labels, charge-while-running and LiPo-absent behaviour on the bench | Amazon cart link |
| USB-C sink breakout (5.1 kΩ CC) | **Dropped** — module has its own USB-C port. Fallback if its pads are unusable: Amazon B0GR8SNSCQ, $8.45 | — |
| Sunon MF25100V2-1000U-A99 fan ×1 | **In Digi-Key cart** — 259-1831-ND, $10.69, 2,051 in stock (the 26-wk lead is factory only) | Digi-Key |
| Mill-Max 0947 pogo pin ×4 | **In Digi-Key cart** — ED11275-ND, $1.78 ea, 20k in stock | Digi-Key |
| Harwin S70-125161545R pad ×4 | **In Digi-Key cart** — 952-3138-1-ND (cut tape), $0.52 ea, 70k in stock | Digi-Key |
| Pad carriers | **Build step** — see "Pad carriers" below. Not purchased | — |

Digi-Key cart total: $19.89 parts + ~$8.49 FedEx Ground + ~$2.01 tariff ≈ $30.39.
Generic Amazon pogo pins and fans were not substituted: no 5.2 A rating, and the SCAD needs
the Sunon Ø2.8 mm 3-hole pattern.

## Pad carriers

The Harwin S70 is a bare SMT gold pad (2.5 × 1.6 × 0.15 mm) with no leads, so nothing can
be soldered to it directly except on a flat surface. A "carrier" is a tiny piece of
copper-clad FR4 or perfboard (about 1.2 mm × 2.7 mm envelope in the printed pad well) with
two copper lands: one the pad is soldered flat onto, and one where the 24 AWG lead is soldered.
The carrier sits in the recessed shoe on the battery pod; the shell pogo pin presses on the
pad. Cut four from scrap copper-clad or perfboard after the pads arrive.

## Resolved sub-items (were "open" as of 2026-10-01)

- **Fan 3-hole pattern — CONFIRMED.** Sunon MF25100V2-1000U-A99 datasheet:
  "Mounting holes Ø2.8 mm, 3 holes." The SCAD 3-hole fan mount is correct.
  Bearing clarification: series is **MagLev® MF**, bearing type is **Vapo-Bearing™**
  (both appear in the Digi-Key listing — same part, not a conflict).
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

## ⚠️ Mill-Max 0947 pin — current rating (corrected)

Mill-Max's **own 0947 product page** states the discrete-pin ratings directly:

| Source | Rating |
|---|---|
| Mill-Max 0947 product page (discrete pin) | **7 A max @ 30°C rise / 5.6 A derated** |
| Digi-Key datasheet (standalone 0947) | 5.2 A |
| Mill-Max 824/826 **header** page (pin ganged in a multi-pin connector) | 2 A continuous / 3 A max |

So the original design figure (7 A → 5.6 A derated) was correct for the discrete
pin. The "2 A / 3 A" number applies only to the ganged multi-pin header, not to
our two discrete solder-cup pins in a printed shell. Design load is 2 A — inside
the 5.6 A derated discrete rating with margin. Still bench-validate at 3 A
(current-limited supply + dummy load) as planned. Contact resistance 20 mΩ max;
~60 gf mid-stroke force.

## Filament

- Bambu Lab **PETG-HF**, 1 kg — the enclosure parts. Order direct
  (us.store.bambulab.com) or Amazon. Printer shipped with 3× PLA (orange,
  green, + white separator/support) for test prints.

## Power system

| Part | Spec | Link / price |
|---|---|---|
| Adafruit **328** LiPo | 3.7 V, 2500 mAh, 50 × 60 × 7.3 mm | adafruit.com/product/328 — **$14.95** (ground-ship only) |
| Adafruit **PowerBoost 1000C** | booster/charger, 45 × 23 × 10 mm | adafruit.com/product/2465 — **$19.95** (24 in stock) |
| Sunon **MF25100V2-1000U-A99** fan | 25 × 25 × 10 mm, 5 V, 65 mA, Ø2.8 mm × 3 holes, 2-wire | **Not on Amazon.** Digi-Key 7805270 — **$10.09** (1,129 in stock now); TME — **$5.35**; Mouser — in stock; xonelec — **$7.59**; Future Electronics — **$10.01** |
| USB-C sink breakout | 5.1 kΩ CC termination (charge input) | adafruit.com/product/4090 — **$2.95** |

## Rail battery contacts (Codex design, commit `7200d7f`)

| Part | Qty | Rating / spec | Link / price |
|---|---|---|---|
| Mill-Max **0947-0-15-20-77-14-11-0** solder-cup pogo pin | 2 (+2 spare) | 7 A max / 5.6 A derated (discrete pin, see flag); 2.286 mm stroke; 24 AWG cup | Digi-Key 7402798 — **$1.78** (10,215 stock) |
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
- **BIQU Panda Lux LED light bar** (P1S chamber light upgrade, improves camera
  feed) — 5 V 0.3 A, magnetic mount, plugs into the stock light-bar connector.
  **$12.99** (amazon.com/dp/B0D8B28DH6). Note: the stock AP-board light connector
  is rated only ~300 mA — the Panda Lux is a *replacement*, not an add-on, so
  don't run two bars on that connector (risk to the AP board). For *extra* light
  beyond the stock slot, use the P1 USB port (1.5 A) instead.
- *(optional)* silicone stylus tether

## Blocked / TBD

- **Pad carriers** — custom (tiny PCB or copper-clad FR4 island + solder land).
  Build step; fabricate to the 1.2 mm axial × 2.7 mm width envelope after the
  pads arrive. Not an ordered part.
- **Source isolation** — SOLVED per handoff: CYD's Schottky D1 (B5819W) blocks
  backfeed; inject booster 5 V at VCC5V (D1 cathode), never VBUS. Verify D1/Q3
  orientation on the real board with a meter before wiring.

## Already owned (don't buy)

22 AWG solid wire · 24 AWG stranded wire + heat-shrink (Grizzly stock) · solder · iron ·
multimeter · DHT11 (Keyestudio kit) · ESP32-S3 Super Mini ×5 · Bambu P1S + AMS + 3× PLA
(orange, green, white separator) · digital caliper · deburring tool · PETG 5 kg total.
