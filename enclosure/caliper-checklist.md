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
| 4 | Upper-edge connectors | `upper_ports` = 29.44 / 45.17 / 64.1 / 84.1 | read off E32R35T drawing | center + width of each (GPIO39/35, speaker, SPI, I2C) |
| 5 | Lower-edge connectors | `lower_ports` = 25.48 / 40.02 / 57.89 | read off E32R35T drawing | center + width of each (BAT, UART, microSD) |
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
