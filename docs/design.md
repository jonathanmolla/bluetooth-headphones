# Architecture and design decisions

## System design

The headphone electronics combine a BM83 Bluetooth audio module, TPA6130A2 stereo headphone amplifier, STM32U031 controller, and a separate charging and regulation board. Native KiCad schematics define the electrical connections, component values and board interfaces.

## Stereo audio path

The BM83 left and right analog outputs connect through 0.47 uF coupling capacitors to the TPA6130A2 amplifier. Matching capacitors AC-ground the reference inputs. This topology provides DC isolation while preserving the stereo analog signal path.

The LTspice circuit represents the audio chain using ideal source signals, differential input resistance, voltage gain and 38 ohm resistive loads. Parameterized capacitance, input resistance and gain support analysis of the coupling network's frequency response. The real amplifier input resistance varies with volume setting; the simulation exposes resistance and gain as independent parameters for controlled comparisons.

## STM32 controller configuration

The STM32U031C8T6 uses an LQFP48 package. The MCU/button schematic defines the controller connections, and the STM32CubeMX `.ioc` file captures pin assignments and peripheral configuration in an editable source format.

## Charging and regulation

The power board integrates an MCP73833 charging circuit and a fixed 3.0 V TPS7A2030PDBVR regulator. Separate KiCad projects organize the main audio electronics and the power circuitry around their distinct functions and board connections.

## Project-local CAD libraries

Custom symbols and footprints reside beside their KiCad projects and use `${KIPRJMOD}` references. This structure keeps the design's local CAD assets together when the repository is downloaded. Standard symbols, footprints and 3D models use the KiCad library installation.

## Engineering evidence

The repository includes hierarchical schematics, PCB layouts, schematic-derived component lists, controller configuration, parameterized audio simulation, browser-readable design exports and recorded electrical/design-rule checks.

## Current interface and component decisions

Power J6 → main J5 is 1 VBAT, 2 GND, 3 +3V0. The BM83 and amplifier use VBAT; the STM32 and I2C pullups use +3V0. The selected protected SparkFun PRT-13851 is 400 mAh, with nominal 79 mA charging set by 12.7k. Playback is excluded while charging and requires a manual off action; no automatic USB-detection interlock is installed.

The STM32 uses SWD; the BM83 uses the separate [revision C loose-module fixture](../hardware/programmer/). The BM83 configuration uses Host mode and single-ended stereo output.

[November 14 circuit/layout review](design-review-2026-11-14.md) documents comparison with Microchip, TI and SparkFun designs, charger bypass capacitance, audio coupling values, thermistor behavior and fixture geometry. The saved CAD retains 1 uF power C1 and 0.47 uF audio C5–C8.
