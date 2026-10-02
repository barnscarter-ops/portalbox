# Handoff — 2026-10-01 (pre-printer arrival)

State snapshot for Claude (review) and the next Hermes session (resume).

## What's done

- **CYD enclosure rev 0.2** (`enclosure/cyd-case.scad`): bezel + shell + battery
  pod, recessed dovetail rail, stylus tube, fan, PowerBoost cradle.
- **Rail-carried battery contacts** (commit `7200d7f`): two Mill-Max pogo pins
  (shell) → two Harwin gold pads (pod shoe). Guarded, keyed, ground-first /
  positive-last sequencing. No exterior cable.
- **Codex dimension audit** (`audit_dimensions.py` → `dimension-audit.json`):
  CAD internally consistent, all parts watertight.
- **Parts list**: `enclosure/PARTS.md`.
- **Caliper checklist**: `enclosure/caliper-checklist.md` (physical fit still
  unverified).
- **Hardware inventory updated**: `flipper-protoboard` (4 blank boards) added;
  flipper dev-board count corrected (1 Marauder + 2 unflashed). Pushed to
  `hardware-inventory` + `Coding_Practice`.
- **Flipper build 1 (DHT11)**: specced (`Flipper/build-1-dht11.md`), Unitemp FAP
  downloaded (`Flipper/apps/unitemp_2.1.fap`).
- **Printer**: Bambu Lab P1S + AMS ordered ($549, Amazon, arriving soon).

## Open items

- **CYD physical fit UNVERIFIED.** Datasheet (E32R35T) is a family reference.
  Flagged in `caliper-checklist.md`: button position `41.47` vs `13.72`
  (likely coordinate flip), `fan`/`boost` missing the `O` offset,
  `port_z`/`port_h` provisional.
- **Source isolation / power mux**: parts still to be selected (`WIRING.md`).
- **Rail contacts need bench validation before energizing** — 5-step checklist
  in `WIRING.md` (ground-first order, no bridging, no reversed engagement,
  current-limited 3 A test).
- **Flipper build 2** (ESP32-S3 Super Mini WiFi board): planned, not started.

## Next (when the printer arrives)

1. Order PETG + the `PARTS.md` items.
2. P1S first boot → PLA calibration print → print `fit_coupon` in PETG.
3. Caliper-measure the CYD board (checklist) to close the button/fan/port
   questions before committing to the full shell.
4. Continue Flipper build 1 (solder DHT11) + sideload Unitemp.

## Key files / repos

| Thing | Location |
|---|---|
| CYD repo (remote `portalbox`) | `C:\Workspace\Active\CYD` — `enclosure/` (SCAD, PARTS.md, WIRING.md, caliper-checklist.md, audit) |
| Flipper (not a git repo) | `C:\Workspace\Active\Flipper` — `build-1-dht11.md`, `apps/unitemp_2.1.fap`, `portalbox-flipper/PLAN.md` |
| Hardware inventory (authoritative) | `C:\Workspace\Active\Hardware\Inventory` |
| PortalBox plan (Flipper) | `Flipper/portalbox-flipper/PLAN.md` |

## User context

- Carter is an electrician — fluent in continuity/voltage/polarity/grounding,
  owns a multimeter. Learning gap is PCB design + embedded firmware, not
  electrical fundamentals. Wants "complete build" projects, does them properly,
  values verification.
