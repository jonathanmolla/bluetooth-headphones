# Open and explore the design

## Find the board you want

- [Main audio PCB: layout, schematics and project files](../hardware/main/)
- [Charging and power PCB: layout, schematic and project files](../hardware/power/)
- [Board images](images/)
- [Circuit diagram previews](schematics/)

## Open in KiCad

The design exports use KiCad 10.0.3. Install KiCad 10 with its standard symbol and footprint libraries.

1. Download or clone the repository.
2. For the main audio board, open `hardware/main/Bluetooth Headphones Project.kicad_pro`.
3. Open the schematic editor to explore the root, audio and MCU sheets, or the PCB editor to inspect the board layout and copper layers.
4. For the power board, open `hardware/power/Charging and Power.kicad_pro`.
5. Keep the project-local library folders and `fp-lib-table` / `sym-lib-table` alongside each project.

## Open the STM32 configuration

Open [firmware/Firmware.ioc](../firmware/Firmware.ioc) in STM32CubeMX to inspect the STM32U031C8T6 pin assignments and peripherals.

## Run the audio simulation

Open [simulation/Headphones_Audio.asc](../simulation/Headphones_Audio.asc) in LTspice. Keep the local symbols and model library in the same folder. Follow the [simulation instructions](../simulation/README.md) to select transient or AC analysis and inspect the output traces.

## Inspect component lists and checks

Each board folder includes a `bom.csv` with schematic references, values, footprints and datasheet links. The [engineering check report](validation.md) links to the recorded ERC and DRC results and their reproduction commands.
