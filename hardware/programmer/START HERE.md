# BM83 programmer — revision C, ONE PCB

Open **[BM83_Programmer.kicad_pro](BM83_Programmer.kicad_pro)**. It contains the schematic and the only PCB in this revision.

The ten probes are electrically soldered directly to that PCB. Copper tracks connect them to the USB-UART header, TEST jumper and reset button. **There is no second PCB, no J2 and no wire harness.**

The BM83 rests on the probes and is removable. Its pads are never soldered to this programmer.

A printed/machined insulating locator and two screw-down bars hold the probe bodies and module in position. These are mechanical parts, not additional circuit boards. Their design is [Instructions/Fixture.scad](Instructions/Fixture.scad).

See [Instructions/Assembly and Pin Mapping.md](Instructions/Assembly%20and%20Pin%20Mapping.md) for parts placement, assembly, exact pad mapping, software operation and manufacturer page references. Engineering reports are in [docs/checks](../../docs/checks/). Other root files are required KiCad component libraries.

USB cable: **Adafruit 954**, 3.3 V UART logic and separate 5 V power. The Google document's BOM is the purchasing list. CAD checks pass; the first built fixture still needs fit, continuity and successful-programming tests.


Read the [current review](../../docs/design-review-2026-11-14.md#programmer-revision-c) for physical alignment and lateral-retention requirements. The provided locator needs a completed clamping arrangement.
