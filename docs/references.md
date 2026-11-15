# Component documentation and references

| Component or tool | Documentation |
| --- | --- |
| BM83 Bluetooth audio module | [Microchip product page](https://www.microchip.com/en-us/product/BM83) |
| TPA6130A2 headphone amplifier | [TI datasheet](https://www.ti.com/lit/ds/symlink/tpa6130a2.pdf) |
| STM32U031 controller | [ST datasheet](https://www.st.com/resource/en/datasheet/stm32u031r6.pdf) |
| TPS7A20 regulator | [TI documentation](https://www.ti.com/lit/gpn/TPS7A20) |
| MCP73833 charger | [Microchip datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP73833-4-Stand-Alone-Linear-Li-Ion-Li-Polymer-Charge-Management-Controller-DS20002005.pdf) |
| KiCad | [Tool documentation](https://docs.kicad.org/) |

## CAD and simulation assets

Project-local symbols and footprints accompany the native design files. Third-party CAD assets and standard library material retain their respective authorship and terms. Manufacturer documentation is accessible through the links above.

The LTspice library contains educational ideal-source and differential-gain models. Its [parameter definitions and analysis instructions](../simulation/README.md) document the analog-network representation used in this project.

## Reference designs and selected external parts reviewed November 14

- [Microchip BM83 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf), pin map, Test mode, UART and antenna placement.
- [Microchip BM83 EVB guide](https://www.microchip.com/content/dam/mchp/documents/WSG/ProductDocuments/UserGuides/BM83-Bluetooth-Audio-Development-Board-User-Guide-DS50002902B.pdf), firmware update sequence.
- [TI TPA6130A2 EVM guide](https://www.ti.com/lit/ug/slou185a/slou185a.pdf), input capacitors and amplifier layout.
- [SparkFun LiPo Charger Plus schematic](https://cdn.sparkfun.com/assets/f/8/b/5/6/SparkFun_LiPo_Charger_Plus.pdf), USB-C and charger comparison.
- [SparkFun PRT-13851 battery](https://www.sparkfun.com/lithium-ion-battery-400mah.html), protection and charge rate.
- [TDK B57861S0103F040](https://product.tdk.com/en/search/sensor/ntc/ntc_element/info?part_no=B57861S0103F040), selected thermistor.
- [INGUN GKS-080-0001 drawing](https://ingun.com/media/01/86/fe/1752238559/INGUN_GKS-080_301_035_A_0805_GKS-080-0001_EN_Datasheet.pdf), probe dimensions. The PCB terminal footprint and fixture mechanics are custom project designs.
