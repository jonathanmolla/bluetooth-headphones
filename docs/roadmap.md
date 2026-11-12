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
