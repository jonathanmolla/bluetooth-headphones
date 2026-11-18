# STM32 headphone controller firmware

This STM32CubeIDE project targets the STM32U031C8T6. It includes the application, peripheral setup, startup code, linker script, and [STM32CubeU0 HAL and CMSIS dependencies](Drivers/Package_license.md). [Firmware.ioc](Firmware.ioc) records the STM32CubeMX configuration.

## Build in STM32CubeIDE

1. Clone or download the repository.
2. Open **File → Import → General → Existing Projects into Workspace** and select the `firmware/` directory.
3. Select **Firmware** and use **Project → Build Project**. The Debug build produces `Debug/Firmware.elf`.

Open [Firmware.ioc](Firmware.ioc) in STM32CubeMX to inspect the configuration. The project uses STM32Cube FW_U0 V1.3.0.

## Controller interfaces

| Function | STM32 connection |
| --- | --- |
| SWD | PA13 SWDIO, PA14 SWCLK, NRST. J4 includes ground and a 3.0 V reference. |
| BM83 host UART | PA9 TX to module RX, PA10 RX from module TX. USART1 runs at 115200 baud. |
| BM83 reset and wake | PA4 open-drain reset, PA5 MFB wake. PA6 is the indication input. |
| Amplifier | PA3 enable, PB6/PB7 I2C1. |
| Buttons | PA0 volume up, PA1 volume down, PA2 play/pause. |
| Battery sense | PA7, ADC1 channel 14. |

[main.c](Core/Src/main.c) checks battery voltage before enabling audio. The [button and amplifier controller](BUTTON_CONTROLS.md) debounces buttons and sets volume over I2C. [BM83_UART.c](Core/Src/BM83_UART.c) handles module startup, pairing, UART events and acknowledgements, and play/pause in Host mode. [Battery monitoring](BATTERY_ADC.md) uses the internal ADC reference and disables audio at a measured 3.30 V or below.

[Editable MCU and button schematic](../hardware/main/MCU_Buttons.kicad_sch) · [System architecture](../docs/design.md)

## Host tests

From the `firmware/` directory, run these commands with native GCC. The tests mock the HAL calls used by the button, amplifier, and battery modules.

```sh
mkdir -p build
gcc -std=c11 -Wall -Wextra -Werror -D__MAIN_H -include tests/main.h -ICore/Inc tests/test_controls.c Core/Src/Headphones_control.c -o build/test_controls
gcc -std=c11 -Wall -Wextra -Werror -D__MAIN_H -include tests/main.h -ICore/Inc tests/test_battery.c Core/Src/battery_monitor.c -o build/test_battery
./build/test_controls
./build/test_battery
```
