# Validation status

Snapshot reviewed on **November 11, 2026** using KiCad **10.0.3**. These checks describe the repository snapshot; they are not a fabrication approval or physical test result.

## Schematic and PCB checks

| Check | Main/audio board | Charging/power board |
| --- | --- | --- |
| Schematic ERC | 0 reported errors or warnings | 0 reported errors or warnings |
| PCB layout-rule findings | 4 warnings | 0 reported layout violations |
| PCB unconnected-item findings | 6 errors | 0 |
| PCB/schematic parity findings | 0 | 3 warnings |

Main-board warnings concern a dangling track, silkscreen edge clearance, a library footprint mismatch, and silkscreen over copper. The unconnected-item errors include ground-zone connectivity and must be reviewed before release. The power-board parity warnings concern the D1/D2/D3 footprint values differing from the schematic LED values.

Full reports: [main ERC](checks/main-erc.json), [main DRC](checks/main-drc.json), [power ERC](checks/power-erc.json), and [power DRC](checks/power-drc.json).

The reports use the projects' current severity configuration. Several rule categories are ignored in the project configuration; see `ignored_checks` in the DRC reports. Checks were run against the saved zone fills without refilling or modifying the board. Zero reported findings therefore means only that the enabled checks found none in that saved state.

Commands used, with paths relative to the repository root:

```sh
kicad-cli sch erc --format json -o docs/checks/main-erc.json "hardware/main/Bluetooth Headphones Project.kicad_sch"
kicad-cli pcb drc --schematic-parity --format json -o docs/checks/main-drc.json "hardware/main/Bluetooth Headphones Project.kicad_pcb"
kicad-cli sch erc --format json -o docs/checks/power-erc.json "hardware/power/Charging and Power.kicad_sch"
kicad-cli pcb drc --schematic-parity --format json -o docs/checks/power-drc.json "hardware/power/Charging and Power.kicad_pcb"
```

## Simulation evidence

LTspice source and simplified models are included. A completed run is evidenced by the original project log and waveform files. Earlier project notes also report AC and transient work. Quantitative results and selected plots have not been reverified for this portfolio snapshot, so no measured or simulated performance figure is presented as a validated result.

See [simulation instructions and limits](../simulation/README.md).

## Firmware evidence

The CubeMX `.ioc` file is present. No application source, successful build, flashing record or hardware execution result is included.

## Physical test plan

Testing awaits the first assembled project PCBs. The following measurements remain planned:

| Test | Evidence to record |
| --- | --- |
| Power bring-up | Supply current, rail voltages, polarity, shorts and temperature |
| Controller bring-up | Programming connection, clock/reset behavior and button inputs |
| Control interfaces | BM83 communication and amplifier I2C transactions |
| Audio gain | Input/output voltage and gain at stated frequencies and settings |
| DC offset | Output DC voltage with approximately 38 ohm dummy loads |
| Clipping | Output waveform versus input level and battery voltage |
| Startup/shutdown | Output transients and mute sequencing |
| Stability | Oscilloscope traces under the intended load conditions |
| Battery/runtime | Verified cell specifications, charge behavior and operating current |

Record instrument settings, load, supply voltage, firmware revision and test conditions alongside every result. Update this page when measurements exist.
