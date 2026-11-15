# Design review — November 14, 2026

The saved main board, power board and revision C BM83 programmer have compatible electrical roles. Circuit and layout comparison supports proceeding toward a first prototype, with the charger output bypass recommendation resolved before freezing the design. No assembled-board measurements or verified module programming are claimed.

## Comparison with existing designs

| Reference | Comparison with the saved project |
| --- | --- |
| [Microchip BM83 EVB and UART update procedure](https://www.microchip.com/content/dam/mchp/documents/WSG/ProductDocuments/UserGuides/BM83-Bluetooth-Audio-Development-Board-User-Guide-DS50002902B.pdf) | The main board has UART TX/RX, MFB wake, P0_0 indication and open-drain reset control. The external fixture supplies ADAP_IN and accesses UART, TEST and reset for a loose module. |
| [TI TPA6130A2 evaluation board](https://www.ti.com/lit/ug/slou185a/slou185a.pdf) | Same amplifier family, I2C control and AC-coupled input architecture. TI uses 2.2 uF input capacitors; this board currently uses 0.47 uF. |
| [SparkFun LiPo Charger Plus schematic](https://cdn.sparkfun.com/assets/f/8/b/5/6/SparkFun_LiPo_Charger_Plus.pdf) | Same MCP73833 family and independent 5.1 kohm USB-C CC pulldowns. SparkFun uses 10 uF input/output capacitors; the saved power board has 1 uF local bypass capacitors. |

These comparisons check circuit choices and layout principles. They do not transfer a reference board's measured performance to this design.

## Board interfaces and power

Power J6 connects to main J5 by pin number: **1 VBAT, 2 GND, 3 +3V0**. Verify the assembled cable maps 1-to-1; connector rotation on the PCB does not define cable polarity. VBAT supplies BM83 BAT_IN and both amplifier supply pins. The TPS7A2030 supplies the STM32 and I2C pullups, not the entire audio load. The battery divider is 47k/47k with a 100 nF ADC filter.

Selected battery: **SparkFun PRT-13851, 3.7 V nominal, 400 mAh**. [SparkFun specifies](https://www.sparkfun.com/lithium-ion-battery-400mah.html) built-in voltage/current protection, 80 mA normal charge current and 400 mA maximum charge current. The saved 12.7k PROG resistor gives about **78.7 mA nominal**, using the MCP73833 formula. Ideal capacity/current time is 5.08 h; actual full charge takes longer due to preconditioning, taper and losses. Current and termination must be measured. Low-current accuracy should be checked against the datasheet test conditions, rather than assumed from the formula alone.

The selected [TDK B57861S0103F040](https://product.tdk.com/en/search/sensor/ntc/ntc_element/info?part_no=B57861S0103F040) is 10k at 25 C, nominal beta 3988 K. Direct THERM-to-ground connection gives an approximate **7–41 C** window using nominal 24k/5k thresholds and a constant-beta approximation; actual limits depend on the resistance curve and tolerances. This is narrower than the documented 0–45 C target. The sensor must contact the cell. No resistor network for a precisely defined 0–45 C window is installed.

Playback while charging is excluded by the project requirement. There is no charger-status/USB-detect signal to the STM32 and no automatic hardware interlock. The user turns the headphones off; firmware must implement a sufficiently low-current off state. LDO enable is tied to VBAT, so off does not physically disconnect the MCU rail. The 3.3 V planned battery shutdown threshold leaves nominal margin above the BM83's 3.2 V minimum, but cable/trace drop and transient sag need testing.

## Pending charger recommendation

**Power-board C1 is currently 1 uF.** [MCP73833 DS20002005C section 6.1.1.3](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP73833-4-Stand-Alone-Linear-Li-Ion-Li-Polymer-Charge-Management-Controller-DS20002005.pdf) recommends at least 4.7 uF at VBAT-to-VSS for constant-voltage stability. Select a suitable 4.7–10 uF replacement with effective capacitance checked at battery voltage. Keep it near the charger with a short return path. Distant main-board capacitance across a cable does not replace this local bypass. Increasing input C2 from 1 uF is also worth considering against the reference design and USB input requirements.

**These are recommendations, not installed changes.** Native CAD and schematic-derived BOMs intentionally retain the saved 1 uF values.

## Audio quality and layout

BM83 AOHPL/AOHPR drive the amplifier's negative inputs through C5/C7; positive inputs are AC-grounded through C6/C8. This agrees with TI's single-ended-input guidance. C5–C8 are each **0.47 uF**. With the datasheet's 37k minimum differential input resistance, their effective series capacitance is 0.235 uF, giving **18.3 Hz cutoff and approximately -2.64 dB at 20 Hz**. Input resistance varies with gain; these are calculated conditions, not measured response.

Optional quality improvement: select **2.2 uF for all four inputs**, matching TI's EVM value. At the same nominal resistance the cutoff becomes about 3.9 Hz. Check part tolerance and effective capacitance. The saved CAD still uses 0.47 uF.

The four-layer main PCB has broad internal ground planes, approximately 9 mm BM83-to-amplifier analog routes, short amplifier input routes, charge-pump connections of a few millimetres, and multiple amplifier exposed-pad ground vias. The charger is physically separated onto its own PCB. These support low-noise operation but do not establish noise or distortion figures.

The BM83 antenna projects beyond the main-board edge. Some bottom-layer control traces remain beneath the module body with internal ground planes between them and the module. Microchip recommends avoiding routing directly under the module; this remains a layout departure. Keep the antenna clear of the battery, wiring, the power PCB and enclosure metal. Range must be tested in the assembled enclosure. See [BM83 datasheet section 7.2](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf).

The selected **Ole Wolff OWR-4009T-38E** drivers are documented in the project purchasing BOM as nominal 38 ohm, 10 mW rated, 20 mW maximum. At nominal resistance, 10 mW corresponds to 0.616 V RMS. The amplifier can exceed the intended driver level; firmware must cap gain, start muted/disabled, and mute before shutdown. Final limits require dummy-load measurements and the driver specifications. Acoustic quality also depends on the driver, shell, seal and damping.

## Programmer revision C

One 90 x 50 mm PCB connects ten INGUN GKS-080-0001 probes directly through plated terminal holes. No second guide PCB or probe wire harness is used. USB-UART J1 is 5 V power with **3.3 V logic**; JP2 selects TEST, SW1 resets. ADAP_IN receives 5 V, BAT_IN receives its capacitor only. C1/C2 are on the underside near the probes. The fixture is used before the module is soldered onto the main board; installed-module reconfiguration is not provided by this loose-module fixture.

The reviewed pad map is P1→22, P2→23, P3→29, P4→30, P5→31, P6→43, P7→16, P8→50, P9→56, P10→57. Positions, nets and 0.45 mm terminal holes were checked against the module geometry. The probe footprint is custom, not an INGUN-supplied land pattern.

The installed `Fixture.scad` supports probe collars but its locating frame ends at the module underside; it does not positively constrain the seated module laterally. The project owner intends to arrange clamping separately. Physical retention, alignment, contact resistance and a complete verified write remain acceptance tests. Clamp friction alone has not been demonstrated sufficient. CAD checks do not establish repeatable fixture operation.

## Check results and remaining evidence

All three schematics: zero ERC findings. All three boards: zero unconnected items. Main: four DRC warnings, zero parity issues. Power: zero layout findings and three LED-value parity warnings. Programmer: zero layout/parity findings. Enabled severities and ignored categories are recorded in [the reports](checks/).

The main BM83 library warning was investigated: pad positions, sizes and shapes match the installed standard library footprint; graphical item counts differ. Other main warnings concern a 0.2525 mm dangling BT_WAKE stub and silkscreen clipping. These are distinct from a missing required net connection.

Before performance claims: measure charging stability/termination and cell temperature response; confirm SWD/UART/I2C operation; test gain, DC offset, clipping, startup/shutdown and noise into ~38 ohm dummy loads; then evaluate drivers, enclosure range and runtime. No buildable STM32 application or final BM83 image set is included yet. Firmware may be developed during PCB fabrication.
