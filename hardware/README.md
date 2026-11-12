# Hardware design files

The headphone electronics are organized into two KiCad board projects.

| Board | Purpose | Open the design | View the layout |
| --- | --- | --- | --- |
| **Main audio PCB** | BM83 Bluetooth audio, TPA6130A2 amplification, STM32 controller and buttons | [Main audio board files](main/) | [Main audio PCB image](../docs/images/main-board.svg) |
| **Charging and power PCB** | USB power input, MCP73833 battery charging and 3.0 V regulation | [Charging and power board files](power/) | [Power PCB image](../docs/images/power-board.svg) |

Each board folder contains its editable `.kicad_pcb` layout, `.kicad_sch` schematic, `.kicad_pro` project, component list and project-local libraries. Open the `.kicad_pro` file to browse the complete board project in KiCad.

For browser-readable circuit diagrams, see [schematic previews](../docs/schematics/). For recorded design checks, see [engineering check results](../docs/validation.md).
