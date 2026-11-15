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

## Hardware support and implementation status

The saved PCB provides SWDIO, SWCLK, NRST, ground and 3.0 V reference at J4. PA9/PA10 connect to BM83 RX/TX; PA4 controls reset (open drain), PA5 MFB wake and PA6 wake indication. PA3 controls amplifier enable, PB6/PB7 I2C, PA0–PA2 buttons and PA7 battery ADC.

Only CubeMX configuration is included. Review and generate peripheral initialization before implementing the application; the pin assignment alone is not a complete UART setup. Implement BM83 startup/ACK/retry timing, amp mute/gain sequencing, button timing and battery shutdown. Configure boot option bytes as needed through SWD. The separate BM83 configuration must select compatible Host mode and single-ended stereo audio. Final images and successful hardware execution remain pending.
