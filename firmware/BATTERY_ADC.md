# Battery ADC

PA7 uses ADC1 channel 14. The [MCU schematic](../hardware/main/MCU_Buttons.kicad_sch) sets the divider with R10 and R11 at 47 kΩ and C15 at 100 nF. The cell voltage is twice the divider voltage.

- [adc.c](Core/Src/adc.c) uses 12-bit conversions and a 160.5-cycle sampling time.
- [battery_monitor.c](Core/Src/battery_monitor.c) calibrates ADC1 once, averages eight VREFINT and eight PA7 conversions per tracked reading, and uses the factory VREFINT calibration to estimate the ADC reference voltage.
- [main.c](Core/Src/main.c) waits 20 ms for the divider to settle and checks the battery once per second. The amplifier stays disabled until the first valid reading above 3.30 V. A successful reading at or below 3.30 V disables the amplifier, requests BM83 power-off if startup was attempted, and latches the audio shutdown state until reset.
- The MCP73833 handles charging in hardware. The MCU continues running while its supply is available.

For bench verification, compare `battery.battery_mv` to the voltage measured at the battery terminals. The debugger also exposes `battery.raw`, `battery.pin_mv`, and `battery_status`.
