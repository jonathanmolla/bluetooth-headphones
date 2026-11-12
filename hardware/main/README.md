# Main audio board

This board brings together BM83 Bluetooth audio, the TPA6130A2 headphone amplifier, and the STM32 controller and button circuitry.

## Main audio PCB

**Editable board layout:** [Bluetooth Headphones Project.kicad_pcb](Bluetooth%20Headphones%20Project.kicad_pcb)

**Open the complete KiCad project:** [Bluetooth Headphones Project.kicad_pro](Bluetooth%20Headphones%20Project.kicad_pro)

**View the layout in your browser:** [Main audio PCB preview](../../docs/images/main-board.svg)

## Schematics and component list

| File | Contents |
| --- | --- |
| [Root schematic](Bluetooth%20Headphones%20Project.kicad_sch) | Project hierarchy linking the audio and controller sheets |
| [Audio schematic](Bluetooth_Audio.kicad_sch) | BM83 module, AC coupling, TPA6130A2 amplifier and headphone outputs |
| [Controller schematic](MCU_Buttons.kicad_sch) | STM32 controller and button connections |
| [Component list](bom.csv) | Schematic references, values, footprints and datasheet links |

[View all schematic previews](../../docs/schematics/) · [Open the separate charging and power board](../power/)

## Library files

`Project_Audio.pretty/` and `STM32.pretty/` contain project-local footprints. The `.kicad_sym` files contain project-local symbols. Keep these folders and both library tables beside the project when downloading it.
