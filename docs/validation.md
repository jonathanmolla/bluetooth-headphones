# Engineering checks and measurement procedure

The repository records KiCad **10.0.3** electrical-rule and PCB design-rule checks performed on **November 11, 2026**.

## Recorded design checks

| Check | Main audio board | Charging and power board |
| --- | --- | --- |
| Schematic ERC findings | 0 | 0 |
| PCB layout-rule findings | 4 warnings | 0 |
| PCB connectivity findings | 6 errors | 0 |
| PCB/schematic parity findings | 0 | 3 warnings |

The reports capture the project check configuration, severity levels, affected items and coordinates. PCB checks use the saved copper-zone fills. The `ignored_checks` field records rule categories excluded by the project configuration.

### Full reports

- [Main audio board: schematic ERC](checks/main-erc.json)
- [Main audio board: PCB DRC](checks/main-drc.json)
- [Charging and power board: schematic ERC](checks/power-erc.json)
- [Charging and power board: PCB DRC](checks/power-drc.json)

### Reproduce the checks

Run these commands from the repository root:

```sh
kicad-cli sch erc --format json -o docs/checks/main-erc.json "hardware/main/Bluetooth Headphones Project.kicad_sch"
kicad-cli pcb drc --schematic-parity --format json -o docs/checks/main-drc.json "hardware/main/Bluetooth Headphones Project.kicad_pcb"
kicad-cli sch erc --format json -o docs/checks/power-erc.json "hardware/power/Charging and Power.kicad_sch"
kicad-cli pcb drc --schematic-parity --format json -o docs/checks/power-drc.json "hardware/power/Charging and Power.kicad_pcb"
```

## Simulation workflow

The LTspice circuit provides parameterized transient and AC analysis of stereo audio coupling and voltage gain. The [simulation guide](../simulation/README.md) defines the source signals, input resistance, gain, loads and analysis directives.

## Bench measurement procedure

This procedure defines the measurements and evidence to collect for assembled-board evaluation.

| Measurement | Record |
| --- | --- |
| Power rails | Supply current, rail voltages, polarity and temperature |
| Controller operation | Programming connection, clock/reset behavior and button inputs |
| Control interfaces | BM83 communication and amplifier I2C transactions |
| Audio gain | Input/output voltage, frequency and control setting |
| DC offset | Output DC voltage with approximately 38 ohm dummy loads |
| Clipping | Output waveform versus input level and supply voltage |
| Startup/shutdown | Output transients and mute sequencing |
| Stability | Oscilloscope traces under specified load conditions |
| Battery operation | Cell specifications, charge behavior and operating current |

Record instrument settings, load, supply voltage and firmware revision alongside each measurement.
