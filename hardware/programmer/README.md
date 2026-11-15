# BM83 programmer — revision C

One PCB with ten direct spring contacts for configuring a loose BM83 module before main-board assembly.

- [KiCad project](BM83_Programmer.kicad_pro), [schematic](BM83_Programmer.kicad_sch), [PCB](BM83_Programmer.kicad_pcb)
- [Assembly, pin mapping and manufacturer references](Instructions/Assembly%20and%20Pin%20Mapping.md)
- [Locator and clamp CAD](Instructions/Fixture.scad)
- [Schematic component list](bom.csv)
- [Layout preview](../../docs/images/programmer-board.svg)
- [Schematic preview](../../docs/schematics/programmer/BM83_Programmer.svg)
- [Review and physical acceptance requirements](../../docs/design-review-2026-11-14.md#programmer-revision-c)

J1: 1 +5V, 2 GND, 3 adapter TX to BM83 RX, 4 adapter RX from BM83 TX. UART logic must be 3.3 V. JP2 grounds TEST; SW1 resets. BAT_IN has a capacitor only and must not receive USB 5 V.

The fixture is electrically checked but has not performed a verified hardware write. Lateral retention and precision alignment require a completed clamping arrangement. The supplied locator does not positively retain the seated module laterally. See the review before manufacturing the mechanical parts.

The CSV is schematic-derived and excludes mechanical parts and external equipment. The existing Google document remains the purchasing BOM. Manufacturer documents are linked in the instructions rather than redistributed here.
