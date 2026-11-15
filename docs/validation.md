# Engineering checks and measurement procedure

Design checks: **November 14, 2026**, KiCad 10. The published copies were checked after synchronization with the saved design. DRC refilled zones in memory without saving changes to the boards.

| Check | Main | Power | Programmer Rev C |
| --- | --- | --- | --- |
| Schematic ERC findings | 0 | 0 | 0 |
| PCB layout findings | 4 warnings | 0 | 0 |
| Unconnected items | 0 | 0 | 0 |
| Schematic parity | 0 | 3 warnings | 0 |

Main warnings: dangling BT_WAKE stub, silkscreen edge/mask clipping and BM83 library mismatch. Pad geometry of BM83 matches the installed standard footprint; graphic count differs. Power parity warnings are D1/D2/D3 value labels. Reports record ignored checks and configured severities; zero findings alone does not establish circuit performance.

- [Main ERC](checks/main-erc.json) and [DRC](checks/main-drc.json)
- [Power ERC](checks/power-erc.json) and [DRC](checks/power-drc.json)
- [Programmer ERC](checks/programmer-erc.json) and [DRC](checks/programmer-drc.json)

## Circuit and layout assessment

Read [the full design review](design-review-2026-11-14.md) for reference-design comparisons, cross-board connections, capacitor comparisons, calculated bass response, selected pack/thermistor and programmer mechanical requirements. 

To reproduce checks from the repository root, substitute each project path:

```sh
kicad-cli sch erc --format json -o main-erc.json "hardware/main/Bluetooth Headphones Project.kicad_sch"
kicad-cli pcb drc --schematic-parity --refill-zones --format json -o main-drc.json "hardware/main/Bluetooth Headphones Project.kicad_pcb"
```

Other projects: `hardware/power/Charging and Power` and `hardware/programmer/BM83_Programmer`, with the corresponding extensions. Omit `--save-board` to leave CAD unchanged.

## Measurement method

The LTspice models describe the analog coupling network and ideal voltage gain. The measurement procedure covers the electrical and assembled-system characteristics below.

| Measurement | Evidence to record |
| --- | --- |
| Power and charging | Rail voltages/current, charge current/termination, bypass stability, temperature and NTC cutoff |
| Programmer | Alignment, retention, continuity while clamped and verified complete module write |
| Controller | SWD, buttons, UART and amplifier I2C |
| Audio into ~38 ohm dummy loads | Gain, DC offset, clipping, noise, startup/reset/shutdown transients |
| Assembled headset | Driver balance, enclosure range, reconnection, playback/standby current and runtime |

Record instruments, loads, supply voltage, firmware revision and conditions. 
