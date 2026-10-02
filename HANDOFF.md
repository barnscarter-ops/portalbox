# Handoff — 2026-10-02 (parts ordering + printer prep)

For the next Hermes session. All repos are clean and pushed. Start here.

## THE TASK

Find and order the parts for the CYD enclosure + its battery power system, and
prepare for the Bambu P1S arrival. Work from `enclosure/PARTS.md`.

## Current state (all pushed, trees clean)

- **CYD repo** (`portalbox`) @ `dca41ea` — Claude's full audit done; 6 fixes
  applied by Codex (`9e0c037`); lock screw reduced M3×16→M3×12 (`dca41ea`).
- **Hardware inventory** + **Coding_Practice** mirror synced (flipper protoboard
  added, dev-board count corrected).

## The design (final, as audited)

- Rail-carried battery power: 2× Mill-Max 0947 pogo pins (shell) → 2× Harwin S70
  gold pads (pod shoe). Ground-first / positive-last, 1 mm sequencing margin.
- **Source isolation SOLVED** — the CYD's existing Schottky D1 (B5819W) blocks
  backfeed; inject booster 5 V at **VCC5V (D1 cathode)**, never VBUS. Verify D1
  and Q3 orientation on the real board with a meter.
- Lock: **M3 heat-set insert + M3×12 socket screw** (counterbored; tip z=19.50).
- Physical fit **UNVERIFIED** — needs calipers (`caliper-checklist.md`).

## Parts to order (full detail in `enclosure/PARTS.md`)

- Bambu **PETG-HF** (1 kg)
- Adafruit **328** LiPo (3.7 V 2500 mAh) + Adafruit **PowerBoost 1000C**
- Sunon **MF25100V2-1000U-A99** fan (confirm 3-hole pattern before buying)
- Mill-Max **0947-0-15-20-77-14-11-0** pogo pin ×2 + Harwin **S70-125161545R** pad ×2
- **M3 heat-set inserts** (nominal 4.6 mm OD — confirm actual OD before finalizing seats)
- **M3×12** screws (bezel ×4 + lock ×1) + small assortment (×8/×10/×12)
- USB-C sink breakout (5.1 kΩ CC termination)
- Slide switch (SPDT), heat-shrink, 24 AWG stranded wire
- **Calipers** (150 mm, ±0.05 mm) — needed for physical-fit verification

Open sub-items: Harwin pad carriers are still provisional (settle before ordering
the pads); confirm the insert OD matches the seat before buying inserts.

## Before printing the full shell

1. Caliper-measure the CYD board (`caliper-checklist.md`): port sizes, `port_z`/
   `port_h`, ESP32 module + fan position, button positions, PCB-to-panel offset.
2. Print `fit_coupon` first (PETG), verify rail/screw/stylus fit + lock-screw tip
   clearance (tip z=19.50).
3. Bench-validate the rail contacts (sequence, polarity, no bridging, 3 A)
   before any LiPo is connected.

## Key locations

| Thing | Path |
|---|---|
| CYD repo (`portalbox`) | `C:\Workspace\Active\CYD` — `enclosure/` (PARTS.md, WIRING.md, caliper-checklist.md, cyd-case.scad, README.md) |
| Hardware inventory | `C:\Workspace\Active\Hardware\Inventory` |
| Flipper (not a repo) | `C:\Workspace\Active\Flipper` — `build-1-dht11.md`, `apps/unitemp_2.1.fap` |
| Printer | Bambu P1S + AMS (ordered, $549 Amazon, 4× PLA included) |

## User context

Carter — electrician, owns a multimeter, fluent in electrical fundamentals;
learning CAD / 3D-printing / embedded firmware. Drives Codex + Claude for design;
Hermes reviews and commits. Wants "do it right" + verification. Printer arriving
soon.
