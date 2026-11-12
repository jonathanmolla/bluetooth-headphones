# Opening the project

## View without installing tools

Read the main README, open the SVG schematic exports in `docs/schematics`, and inspect the layout previews in `docs/images`. Download an SVG if your browser does not display it inline.

## KiCad

The portfolio exports were prepared with KiCad 10.0.3. Install KiCad 10 with its standard symbol and footprint libraries.

1. Download the repository or clone it from GitHub.
2. Open `hardware/main/Bluetooth Headphones Project.kicad_pro`.
3. Open the root schematic, then its audio and MCU child sheets.
4. Open the PCB editor to inspect placement, routing and stack-up.
5. Separately open `hardware/power/Charging and Power.kicad_pro` for charging and regulation.

Keep the library folders and `fp-lib-table` / `sym-lib-table` beside their projects. Some optional custom 3D models are not included; missing model warnings do not indicate a completed 3D assembly.

## STM32CubeMX

Open `firmware/Firmware.ioc`. Inspect pin assignments and peripherals. There is no buildable application or tested firmware binary in this snapshot; generating code is a future implementation step.

## LTspice

Follow `simulation/README.md`. Keep the local symbols and model library beside the schematic. Analysis results depend on the selected directive and model assumptions.

## Reviewing or changing the hardware

Run ERC and DRC in KiCad and inspect every remaining finding. Check interfaces between the two boards. Complete component, battery and thermal verification before generating fabrication files. No fabrication package is included as an approved release.
