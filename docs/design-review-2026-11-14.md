# Circuit design and reference comparisons

The headphone system combines a main audio/controller PCB, a separate charging and regulation PCB, and a revision C loose-module BM83 programming fixture.

## Reference-design comparisons

| Reference | Project implementation |
| --- | --- |
| [Microchip BM83 EVB](https://www.microchip.com/content/dam/mchp/documents/WSG/ProductDocuments/UserGuides/BM83-Bluetooth-Audio-Development-Board-User-Guide-DS50002902B.pdf) | UART TX/RX, MFB wake, P0_0 indication and open-drain reset connect the host STM32 to BM83. The separate fixture accesses ADAP_IN, UART, TEST and reset. |
| [TI TPA6130A2 EVM](https://www.ti.com/lit/ug/slou185a/slou185a.pdf) | Stereo AC-coupled analog inputs, I2C volume control, charge-pump supply and ground-referenced headphone outputs. |
| [SparkFun LiPo Charger Plus](https://cdn.sparkfun.com/assets/f/8/b/5/6/SparkFun_LiPo_Charger_Plus.pdf) | MCP73833 charger architecture and separate 5.1k USB-C CC pulldowns. |

## Board interfaces and power

Power J6 connects to main J5 by pin number: **1 VBAT, 2 GND, 3 +3V0**. VBAT supplies BM83 BAT_IN and the amplifier. TPS7A2030 supplies the STM32 and I2C pullups. The battery divider uses 47k/47k resistors and a 100 nF ADC filter.

The battery is a protected **SparkFun PRT-13851, 3.7 V nominal, 400 mAh** pack. [SparkFun specifies](https://www.sparkfun.com/lithium-ion-battery-400mah.html) an 80 mA normal charge rate and a 400 mA maximum. The 12.7k PROG resistor calculates to **78.7 mA nominal**. The capacity/current calculation gives 5.08 h, with the constant-voltage taper extending full-charge time.

The [TDK B57861S0103F040](https://product.tdk.com/en/search/sensor/ntc/ntc_element/info?part_no=B57861S0103F040) is a 10k thermistor with nominal beta 3988 K. Its direct THERM connection calculates to approximately **7–41 C** using nominal charger thresholds and a constant-beta model. Actual switching temperatures depend on the resistance curve and tolerances. The bead senses cell temperature through physical contact with the battery.

The operating arrangement uses a manual off action during charging. LDO enable is connected to VBAT, and the MCU controls the system's low-power state. The battery shutdown design uses 3.3 V against the BM83's 3.2 V minimum supply specification.

## Charger capacitance comparison

The power schematic specifies **1 uF for C1 and C2**. [Microchip DS20002005C section 6.1.1.3](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP73833-4-Stand-Alone-Linear-Li-Ion-Li-Polymer-Charge-Management-Controller-DS20002005.pdf) describes a 4.7 uF minimum recommended battery-output bypass, while SparkFun uses 10 uF input and output capacitors. Effective capacitance, voltage bias, cable inductance and local return-path length govern the comparison. The schematic-derived component lists record the project's exact values.

## Audio response and layout

BM83 AOHPL/AOHPR connect through C5/C7 to the amplifier's negative inputs. C6/C8 AC-ground the positive inputs, following TI's single-ended arrangement. All four capacitors are **0.47 uF**. With 37k differential input resistance, their 0.235 uF effective series capacitance gives a calculated **18.3 Hz cutoff and -2.64 dB response at 20 Hz**. Resistance varies with gain. TI's EVM uses 2.2 uF input capacitors; at the same resistance that value calculates to a 3.9 Hz cutoff.

The four-layer main PCB provides broad internal ground planes, approximately 9 mm BM83-to-amplifier analog routes, short input connections, charge-pump connections of a few millimetres and multiple amplifier ground vias. The separate PCB carries the charging circuitry.

The BM83 antenna projects beyond the main-board edge. Internal ground planes separate bottom-layer control routing from the module body. [Microchip section 7.2](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf) provides the placement reference for module grounding, antenna clearance and routing. Battery, wiring and enclosure placement determine the assembled RF environment.

The **Ole Wolff OWR-4009T-38E** drivers have nominal 38 ohm impedance, a documented 10 mW rated input and 20 mW maximum. At nominal impedance, 10 mW corresponds to **0.616 V RMS**. The control architecture uses amplifier gain limits and mute/enable sequencing. Driver response, enclosure seal and damping determine acoustic behavior.

## Programmer revision C

One 90 x 50 mm PCB connects ten INGUN GKS-080-0001 probes through plated terminal holes. J1 supplies 5 V power and **3.3 V UART logic**, JP2 selects TEST and SW1 resets. ADAP_IN receives 5 V; BAT_IN connects to its bypass capacitor. C1/C2 sit on the underside near the power probes.

The physical-pad mapping is **P1→22, P2→23, P3→29, P4→30, P5→31, P6→43, P7→16, P8→50, P9→56, P10→57**. The custom probe footprint uses a 0.45 mm plated terminal hole and 0.80 mm pad. The insulating locator supports probe collars and the screw-down bars apply clamping pressure. Its frame height equals the module underside height; lateral positioning is provided by the clamping arrangement.

The module is configured in the fixture and then soldered onto the main board. The [assembly instructions](../hardware/programmer/Instructions/Assembly%20and%20Pin%20Mapping.md) document power, pin mapping, module placement and the isUpdate procedure.

## Recorded electrical checks

All three schematics have zero ERC findings. All three boards have zero unconnected items. Main: four DRC warnings and zero parity findings. Power: zero layout findings and three LED-value parity warnings. Programmer: zero layout or parity findings. [Full reports](checks/) include configured severities and ignored categories.

The BM83 footprint's pad positions, sizes and shapes match the installed standard library footprint; graphical item counts differ. The other main warnings concern a 0.2525 mm BT_WAKE stub and silkscreen clipping. [Measurement methods](validation.md#measurement-method) cover charging, control interfaces, dummy-load audio, radio operation and runtime.
