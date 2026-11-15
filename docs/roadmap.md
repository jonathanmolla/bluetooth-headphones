# Project file guide

## Hardware design

| Deliverable | Location |
| --- | --- |
| Main audio PCB and KiCad project | [hardware/main](../hardware/main/) |
| Charging and power PCB and KiCad project | [hardware/power](../hardware/power/) |
| Audio and controller circuit diagrams | [Main schematic previews](schematics/main/) |
| Charging and regulation circuit diagram | [Power schematic preview](schematics/power/Charging%20and%20Power.svg) |
| Main board component list | [Main audio BOM](../hardware/main/bom.csv) |
| Power board component list | [Power BOM](../hardware/power/bom.csv) |

## Configuration and analysis

- [STM32 pin and peripheral configuration](../firmware/)
- [LTspice stereo audio simulation](../simulation/)
- [Recorded schematic and PCB checks](validation.md)
- [Bench measurement procedure](validation.md#bench-measurement-procedure)

## Design documentation

- [System architecture and design decisions](design.md)
- [Software and setup instructions](getting-started.md)
- [Board layout images](images/)
- [Component documentation and references](references.md)

## November 14 status and next steps

- Current main/power CAD and revision C programmer are included; zero unconnected items on all three boards.
- Main/power interface and selected battery charge rate reviewed.
- Resolve power C1 local bypass recommendation before freezing the design; optionally increase C5–C8 for flatter deep bass.
- Finalize module retention, fixture alignment and manufacturing outputs.
- Develop STM32 application and BM83 configuration during fabrication; hardware tests follow assembly.
- Complete the measured validation plan before making performance claims.
