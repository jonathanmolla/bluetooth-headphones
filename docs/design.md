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
