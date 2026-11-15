# STM32 controller configuration

[Firmware.ioc](Firmware.ioc) captures the STM32CubeMX configuration for the **STM32U031C8T6** headphone controller.

## Explore the configuration

1. Open `Firmware.ioc` in STM32CubeMX.
2. Inspect the controller pin assignments and configured peripherals.
3. Cross-reference the [MCU and button schematic](../hardware/main/MCU_Buttons.kicad_sch) to follow the hardware connections.

## Related design files

- [Controller schematic preview](../docs/schematics/main/Bluetooth%20Headphones%20Project-MCU_and_Buttons.svg)
- [Main audio board project](../hardware/main/)
- [System architecture](../docs/design.md)

## Controller interface design

| Function | STM32 connection |
| --- | --- |
| SWD programming | PA13 SWDIO, PA14 SWCLK, NRST; J4 includes ground and 3.0 V reference |
| BM83 UART | PA9 TX to module RX; PA10 RX from module TX |
| BM83 reset | PA4, open-drain control |
| BM83 wake / indication | PA5 MFB; PA6 indication |
| Amplifier | PA3 enable; PB6/PB7 I2C |
| Buttons | PA0–PA2 |
| Battery sensing | PA7 ADC |

The control architecture covers BM83 startup and acknowledgement timing, reset/retry handling, amplifier mute/gain sequencing, button timing and battery shutdown. The BM83 interface uses Host mode and single-ended stereo audio. The STM32CubeMX source records the controller pin and peripheral configuration.
