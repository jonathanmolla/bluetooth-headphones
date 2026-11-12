# STM32 configuration

`Firmware.ioc` is the STM32CubeMX configuration for the STM32U031C8T6 controller. It captures pin and peripheral selections for the headphone control system.

## Current status

This folder contains the configuration only. Application source, a build system, a flashing procedure, and a working binary have not been supplied in the current project files. Opening the configuration does not demonstrate successful firmware execution.

## Planned implementation

- Generate and review the STM32 project.
- Implement button handling and the required BM83 control behavior.
- Implement and verify amplifier control over I2C.
- Define reset, startup, mute and power-state sequencing.
- Verify behavior on the assembled hardware and record the toolchain and test procedure.
