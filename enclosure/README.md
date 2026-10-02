# CYD case draft 0.3 — physical and electrical validation pending

Revision 0.3 (2026-09-30): keyed axial pogo dock carries battery BAT+/GND
through the recessed rail. Exterior battery socket, pod grommet and jumper removed.
PowerBoost remains inside the main shell; the battery-only pod body remains 14 mm.
Complete printed assembly with pod: approximately 110 × 72.7 × 47.75 mm.
In inches: 4.33 × 2.86 × 1.88. Depth accounting: 31 mm main body + 0.35 mm
pod gap + 14 mm pod body + 2.4 mm lid = 47.75 mm. Pod-only bounding depth is
18.15 mm because the local contact guard projects inward 4.15 mm; 3.80 mm of that
projection overlaps the case. The dovetail shoe itself is 3.65 mm deep, with
3.30 mm inside the case. The pod-only depth is not additive. Full width 75.5 mm
includes its 9.5 mm guarded docking/lock tab; its storage body is 66 mm wide.
Without a battery pod, the full assembled envelope is 108 × 72.7 × 37 mm
(4.25 × 2.86 × 1.46 in), including stylus holder and cap tether lug.
The recessed track is 92.3 mm long and its 90.3 mm cover reaches the side entry.
Connector openings are space reservations, not completed connector mounts.
The internal PowerBoost is a nominal envelope: actual component heights,
retention, antenna clearance and harness need physical verification.

Parametric source: `cyd-case.scad`. Units: millimetres. Draft STL exports and
actual CAD previews are rebuilt by `export.py`. The earlier concept pictures
were appearance studies; these previews come from the fabrication geometry.

## Selected hardware and reference basis

- Owner board: 3.5-inch ESP32-32E N4 resistive CYD, exact manufacturer SKU
  unconfirmed. Owner photo and connector map are in Coding_Practice/references.
- Closely matching LCDWiki E32R35T mechanical drawing, revision 1.0 (2024-08-14):
  <https://www.lcdwiki.com/res/E32R35T/E32R35T_Size.pdf>.
  PCB 101.50 × 55.50 mm; mounting centers 94.50 × 47.90 mm, holes 3.20 mm.
  Rotated rear view: USB left; upper edge GPIO39/35, speaker, SPI, I2C;
  lower edge BAT, UART, microSD. RESET and BOOT face rearward.
  Owner inspection and photos (2026-09-30) correct the connector orientations:
  BAT faces inward; speaker faces rearward. Both remain enclosed with no access
  cutouts. UART, microSD, GPIO39/35, SPI, I2C and USB retain edge openings.
- Stylus, owner tape measurements (2026-09-30): length 87.31 mm, widest handle
  about 7.94 mm, shaft about 4.76 mm. Approximate, not caliper measurements.
  Tube bore 8.8 mm with a quarter-turn bayonet cap and a bought flexible tether.
  Tip has free space rather than carrying the retention load. No spring ejector.
- Battery: protected Adafruit 328, 3.7 V 2500 mAh, 60 × 50 × 7.3 mm:
  <https://www.adafruit.com/product/328>.
- Booster/charger: Adafruit PowerBoost 1000C, product 2465:
  <https://www.adafruit.com/product/2465>. Envelope 45 × 23 × 10 mm including
  optional USB-A connector; exact charging-port location still needs verification.
- Fan candidate: Sunon MF25100V2-1000U-A99, 25 × 25 × 10 mm, 5 V,
  rated 65 mA. Three mounting holes are provisional pending the delivered fan.
  <https://www.digikey.com/en/products/detail/sunon-fans/MF25100V2-1000U-A99/7805270>.
- Rail contacts: 2 × Mill-Max **0947-0-15-20-77-14-11-0** solder-cup pogo pins,
  5.6 A derated, paired with 2 × Harwin **S70-125161545R** gold pads, 6 A.
  Pad carriers and actual pin retention remain provisional. See `WIRING.md`
  for source links, dimensions, sequencing and the unvalidated 2 A load assumption.
- Pod lock: **ISO 4762 M3 × 16 screw + M3 heat-set insert**, nominal Ø4.6 ×
  4.2 mm, installed from shell interior in the reinforced boss. One Ø4.1 × 4.2 mm
  seat replaces the loose nut and overlapping rail seat. A Ø5.8 × 3.2 mm head
  counterbore recesses the head 0.2 mm; nominal insert engagement is 4.2 mm.
  Screw-tip clearance (tip z=15.50) and insert retention require a test print.

## Construction

Front bezel and rear shell capture the board between spacers and rear columns.
Four screws go through the bezel and board's mounting holes into M3 inserts in
the rear columns. Confirm screw length and insert diameter against chosen
hardware; column seats default to 4.1 mm diameter × 4.2 mm deep for nominal 4.6 mm OD
inserts. Tune `insert_od`, `insert_seat_d` and `insert_seat_depth` to bought hardware. Do not tighten against
the touch film. Shell height is 31 mm to allow fan and board clearance.

The female dovetail track is recessed, with a blind bottom, a left stop and a
right entry. Its removable cover or the battery pod uses a matching shoe.
The cover retains its M3 insert seat. The pod uses an M3 screw and heat-set insert in
a reinforced boss: the locking tab is outside the pouch footprint. The rail now
carries battery power through separate guarded contacts as well as alignment.
An asymmetric shoe rib blocks reversed insertion. At the seating stop the lock
hole aligns; tighten the screw before use and remove it before sliding the pod.
Contacts compress along the slide axis, not against the dovetail sidewalls.
It currently supports one cover or one module at a time; stacking additional
modules has not been designed or load-tested.

The stylus tube lies behind the lower connector plane. The bayonet cap includes
a tether hole; use a purchased silicone tether or cord, not a rigid printed
strap. Confirm latch clearance using a test print. Keep the pointed end away
from the closed tube end. The rear RESET/BOOT holes accept two printed plungers,
whose lengths must be adjusted to the actual switch height.

The fan draws air through right-side intake slots and exhausts through its rear
grille. The battery pod leaves the fan and rear buttons clear. There is no
waterproofing claim. Validate airflow around actual PCB components and screen
backing; the draft has no experimentally verified temperature result.

The pod now holds only the pouch. Leave room for foam padding, leads and battery
expansion; do not clamp the pouch. Lid screw pilots stay above the cell envelope.
The PowerBoost cradle is inside the main case, opposite the fan in the upper
region. Secure it on an insulating pad after checking actual component and
antenna clearance. The old divider export is a legacy part, unused in revision
0.2 and later. Switch, contact retention and circuit hardware remain provisional.
Pod leads run in separate internal tunnels, around the lock screw, then up the
left pod wall to emerge above the pouch. No lead passes through an exterior hole.
The flush rail cover has an underside relief around the dock, with a minimum
0.8 mm skin locally. The original three-body fit coupon remains a rail/bore test;
it does not validate the new key or contacts. Test those on an unpowered pod/shell.

## Proposed power arrangement — not a validated wiring instruction

Battery -> recessed shoe pads -> shell pogo pins -> internal PowerBoost BAT/GND.
PowerBoost regulated output -> isolated CYD 5 V feed and 5 V fan.
The built-in PowerBoost charge input is Micro-USB, not USB-C. A separate USB-C
sink socket with CC termination can feed its USB/GND charge-input pads. Never
connect this input to its boosted output. PowerBoost load sharing supports
charging while running, but the manufacturer requires a connected LiPo.
Do not expect this charger to operate normally with the removable cell absent.
References: Adafruit 2465 product page and its pinouts guide:
https://learn.adafruit.com/adafruit-powerboost-1000c-load-share-usb-charge-boost/pinouts
The CYD BAT socket stays unused; do not attach the battery to two chargers.
An internal switch can control PowerBoost EN rather than carrying battery current.
Only short insulated internal leads remain, with mechanical strain relief.
Ground makes first and breaks last; BAT+ makes last and breaks first, with 1 mm
nominal sequencing travel. Two insulating lanes and guarded contact recesses
must prevent bridging or reversed mating. Disable the booster and unplug charge
power before swapping the pod. Future accessories must explicitly match this
battery-voltage interface; it is not a generic 5 V accessory connector.

Owner-reported CYD BAT polarity (2026-09-30): negative toward USB, positive
toward microSD. Inventory agrees: positive nearest UART. Verify both connector
polarity and battery cable polarity with a meter before connecting anything.
The proposed isolated 5 V feed does not require this CYD BAT connection.

An external powered USB connection for firmware programming must be handled
with a confirmed power-selection/isolation arrangement: do not directly parallel
the booster output with a computer's USB supply. Disconnect pod power for the
first programming setup. EN alone is not USB supply isolation. Preserving use
of the native CYD USB-C while hardwiring the booster may require intercepting
its VBUS path and adding a power mux or break-before-make source selector.
Exact CYD solder points and this circuit have not been validated. Final
harness, source switching and protection still
need selection and bench verification. No runtime or available-current claim.

## Review and verification gates

CAD renders and watertight meshes do not prove physical fit. Before a full print:

1. Confirm owner PCB outline, mounting pitch, screen position and component
   height against the family drawing. Port widths/heights are deliberately
   parameterized; current vertical centers/opening sizes are provisional.
2. Check charging-port and slide-switch placement with the selected hardware.
3. Print `fit_coupon.stl` for dovetail and stylus bore, then a short tube/cap
   section and board-edge samples before committing to the complete case.
4. Confirm three fan mounting holes and adequate grille clearance.
5. Check pouch clearance, internal wire routes, key/comb/carrier fit, button
   travel, insert retention and screw lengths. With no battery/charger, confirm
   contact polarity, non-bridging and engagement order through repeated insertion.
6. Bench-test power consumption, charging, USB source isolation and fan startup;
   then measure temperature under actual screen/Wi-Fi load.

Carter approved saving and pushing this CAD draft on 2026-09-30. Physical fit
and electrical validation remain pending. Suggested first
material is PETG; orientation/support strategy must be agreed with the print
provider. STL files retain CAD assembly coordinates; arrange each part on the
bed in the slicer. Do not print the combined assembly as a single part.

After exporting, run `python enclosure/pack_viewer.py` from the repository root
to refresh the explorer's actual CAD meshes. Regenerate its standalone wrapper
after editing the fragment. Dimension verification:
`python enclosure/audit_dimensions.py --openscad PATH_TO_OPENSCAD_COM` compares STL vertex data
against embedded viewer meshes (0.005 mm rounding tolerance), verifies viewer
placements against SCAD constants, and independently exports the two complete
CAD assemblies to compare their bounding dimensions. Results are recorded in
`dimension-audit.json`. This proves CAD agreement, not physical hardware fit.

Interactive viewer: open `viewer/index.html` for the styled standalone page.
Opening the editable `viewer/fragment.html` directly redirects to that page.
Alternatively, serve this enclosure directory with `python -m http.server
8779`, then open `http://127.0.0.1:8779/viewer/`. The standalone viewer contains
the exported meshes and loads Three.js from an approved CDN. `viewer/fragment.html`
is its editable inline source. The committed snapshot matches this revision;
after CAD changes, regenerate embedded meshes and the standalone wrapper before
running the dimension audit. Viewer preferences are presentation state only.

See `WIRING.md` for the proposed power topology. Source isolation and CYD solder
points are unresolved; that diagram is not a completed assembly instruction.

Rebuild with Python, numpy, trimesh, networkx and a local OpenSCAD installation:

```text
python export.py --openscad PATH_TO_OPENSCAD_COM
```

Revision 0.3 CAD check: all nine exports pass watertight/winding/body-count
checks. `dock_clearance_check` produces an empty shell/pod intersection at the
seated placement (`dock-clearance.json`). This checks printed solid clearance
only, not contact fit, sliding force, key strength or electrical behavior.
