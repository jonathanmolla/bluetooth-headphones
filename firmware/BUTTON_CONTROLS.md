# Buttons and amplifier

[Headphones_control.c](Core/Src/Headphones_control.c) reads active-low buttons on PA0, PA1, and PA2. Each input must stay in its new state for 30 ms. Volume changes by one step per press, and PA2 raises a play/pause event for [the BM83 UART handler](Core/Src/BM83_UART.c).

[main.c](Core/Src/main.c) starts the amplifier after a successful battery check and polls the controls while audio is running. PA3 enables the TPA6130A2. I2C1 on PB6/PB7 sets stereo operation, gain, and mute at the amplifier's 7-bit address `0x60`.

Gain codes run from 0 through 63. Code 0 is the lowest gain, and mute uses dedicated register bits. Volume writes preserve the mute bits. An I2C error drives PA3 low to disable the amplifier.

[TPA6130A2 datasheet](https://www.ti.com/lit/ds/symlink/tpa6130a2.pdf) · [MCU and button schematic](../hardware/main/MCU_Buttons.kicad_sch)
