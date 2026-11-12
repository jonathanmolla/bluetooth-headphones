# Architecture and design decisions

## Goal

Develop a custom Bluetooth headphone electronics platform and document the process from component integration through eventual assembled-board testing.

## Audio path

The BM83 module provides left and right analog audio signals. In the current audio schematic, those signals pass through 0.47 uF coupling capacitors into the TPA6130A2 amplifier. The reference inputs are AC-grounded through matching capacitors. The simulation uses 38 ohm resistive loads as an approximation of the headphone drivers; those resistors do not model acoustic behavior or frequency-dependent driver impedance.

The coupling network is a useful place to examine low-frequency response. Its behavior depends on capacitance and input resistance; the real amplifier input resistance varies with volume setting. A single ideal gain and resistance value therefore cannot validate every register configuration.

## Controller

The project uses an STM32U031C8T6 in an LQFP48 package. The MCU/button schematic and CubeMX configuration capture the planned control connections. Application code and end-to-end control behavior have not been included in this snapshot.

## Power partition

Charging and regulation are maintained in their own KiCad project. The schematic uses an MCP73833 charger and a fixed 3.0 V TPS7A2030PDBVR regulator. Keeping the two projects separate makes the power circuitry and inter-board connections easier to inspect.

Battery selection, allowable charging current, thermal behavior, protection, connector polarity, and final power budget must be verified before assembled-board operation. The present files are a design snapshot, not evidence that those checks have passed.

## Local libraries and portability

Custom symbols and footprints are stored with the relevant KiCad project and referenced through `${KIPRJMOD}`. Standard KiCad symbols, footprints, and 3D models still require the normal KiCad library installation. Some optional custom 3D models are absent from the source snapshot; this does not make those models part of the deliverable.

## Evidence and scope

Native schematics, board layouts, the CubeMX file, and LTspice source are the primary artifacts. Generated previews are navigation aids. Design checks and simulation must be interpreted within their stated limits; neither demonstrates a functioning assembled product.
