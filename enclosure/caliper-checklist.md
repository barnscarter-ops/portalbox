# CYD enclosure — physical measurement checklist (calipers)

**Why this exists:** the CAD geometry is internally consistent and exports
watertight — verified by `audit_dimensions.py` → `dimension-audit.json`
(commit `b831f96`). But *internally consistent ≠ fits the real board*. The
datasheet (E32R35T) is a **family reference, not your exact SKU**, and several
values in `cyd-case.scad` are still provisional or unverified. A $15–25 digital
caliper (150 mm, ±0.05 mm) closes this gap definitively.

**Orientation:** rear view, USB-C on the LEFT. Units mm. **PCB-relative** means
measured from the board's own corner — `cyd-case.scad` adds `O = gap + wall = 2.9`
to convert to enclosure coordinates.

## Essential — blocks the shell print

| # | Measure | SCAD var (current value) | Reference | Method |
|---|---|---|---|---|
| 1 | PCB outline | `pcb=[101.5,55.5]` | 101.50 × 55.50 | length + width, edge to edge |
| 2 | Mount holes | `mounts` (centers 94.5 × 47.9) | Ø 3.20, 4× | see "hole centers" note below |
| 3 | **BOOT/RESET buttons** | `buttons` → (3.26, 41.47) & (3.26, 14.03) | (3.26, 14.03) & (13.72, 14.03) | button-body center from left + top edges |
| 4 | Upper-edge connectors | `upper_ports` = 29.44 / 64.1 / 84.1 | read off E32R35T drawing | center + width of each (GPIO39/35, SPI, I2C) |
| 5 | Lower-edge connectors | `lower_ports` = 40.02 / 57.89 | read off E32R35T drawing | center + width of each (UART, microSD) |
| 6 | Connector height | `port_z=11`, `port_h=6.5` (provisional) | — no 2D source exists | PCB surface → top of connector (use the depth rod); plus connector body height |
| 7 | ESP32 module position | `fan=[29,44]` | — | module corners from board edges → center; confirm the fan exhaust lands over the chip |
| 8 | PowerBoost mount area | `boost=[52,34]` | 45 × 23 × 10 (Adafruit 1000C) | only if the physical board is in hand |

## Nice-to-have — refine existing data

| # | Measure | SCAD var (current value) | Method |
|---|---|---|---|
| 9 | Stylus | `stylus_len=87.31`, `stylus_bore=8.8` | length + handle/shaft Ø (upgrades the tape measurements) |
| 10 | Touch panel | VA 77.84 × 50.56 / OD 84.36 × 55.0 | confirm active + outer dims if accessible |

## Open coordinate-frame questions — resolve these with the caliper

1. **Buttons (#3) — highest value.** The SCAD's second-button y = **41.47** looks
   like a coordinate flip: `55.5 − 14.03 = 41.47`. The datasheet places the two
   buttons **side-by-side** at `(3.26, 14.03)` and `(13.72, 14.03)`. Measure both
   button centers — this decides whether the button sleeves are in the right wall.
2. **Fan / boost offset (#7–8).** `fan=[29,44]` and `boost=[52,34]` are used as
   *absolute* enclosure coordinates, while every board feature adds `O`. If they
   were meant PCB-relative, they are **2.9 mm off** toward the corner. Measure the
   ESP32 module's true position and confirm the fan sits over it.
3. **Speaker / BAT classification (from `03f8ada`).** That commit assumes the
   speaker is rear-facing and BAT is inward-facing, so neither needs an edge
   cutout. That assumption is itself **unverified** — confirm against the board
   photos (or the pending verification prompt) that those connectors really don't
   need openings in the shell.

## How to measure hole centers

A caliper measures edges, not centers. For the mounting pitch:

1. Measure the Ø of one hole (expect ~3.2 mm).
2. Measure the **edge-to-edge** distance between two holes along X, then along Y.
3. Center pitch = edge-to-edge + one hole Ø. (e.g. pitch = edge-to-edge + 3.2)

## Recording

Write the numbers below, then reconcile them against `cyd-case.scad` (I can do
that reconciliation and tell you exactly which lines change).

| # | Measured (mm) | Notes |
|---|---|---|
| 1 | | |
| 2 | | pitch X: / Y: |
| 3 | | BOOT: / RESET: |
| 4 | | |
| 5 | | |
| 6 | | |
| 7 | | module center: |
| 8 | | |
| 9 | | |
| 10 | | |

## Measured 2026-10-12 (owner calipers, CYD in hand)

"PCB back" = the connector/ESP32 face, opposite the LCD. Positions are from the
board corner, rear view, USB on the left.

| Item | Measured (mm) | SCAD |
|---|---|---|
| PCB outline | 101.5 × 55.4 | `pcb=[101.5,55.5]` (0.1 mm difference, absorbed by `gap`) |
| PCB thickness | 1.57 | seat gap 1.7 (bezel spacer top z=7.4 to column end z=9.1) |
| Glass front to PCB back | 5.65 (touch panel permanently fitted) | glass front lands at z=3.45, 0.45 behind the bezel face |
| RESET | 2 from USB end, 9.15 from SPI/I2C edge | `buttons` |
| BOOT | 2 from USB end, 9.15 from UART/SD edge | `buttons` |
| Button height above PCB back | 4.15 | plunger `button()` lengthened 18.5 → 20 (print-tested) |
| SPI housing | 59 – 69.25 (centre 64.125, 10.25 wide) | `upper_ports` |
| I2C housing | 79 – 89.25 (centre 84.125, 10.25 wide) | `upper_ports` |
| UART housing | 34.9 – 45.15 (centre 40.025, 10.25 wide) | `lower_ports` |
| microSD slot | 50.75 – 65.65 (centre 58.2, 14.9 wide), 2.5 in from edge | `lower_ports` |
| JST housing height | 3.75 above PCB back | opening z 7.75–14.25 covers PCB back (z=9.1) to housing top (12.85) |
| microSD card installed | flush with the slot, 2 above PCB back | within the opening |
| USB-C | top 3.06, bottom flush with PCB back | cutout z 7.5–14.5 |
| Tallest back component | 6.5 above PCB back (z=15.6) | 3 mm under the fan frame, 1 mm under the booster pocket |

Still unmeasured: GPIO39/35 connector (still datasheet position 29.44).
