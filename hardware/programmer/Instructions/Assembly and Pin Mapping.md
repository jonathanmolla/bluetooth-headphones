# BM83 single-PCB programming fixture — revision C

Revision C, 14 November 2026. One routed 90 x 50 mm two-layer PCB with an antenna notch. This replaces the two-board revision A. There are no wires between probes and PCB: each probe's lower terminal is soldered into a plated hole connected to copper routing. An insulating printed/machined locator supports the probe barrels; the BM83 is removable and unsoldered.

## What goes where

J1 on the left is a Samtec TSW-104-07-G-S four-pin 2.54 mm header. It accepts the Adafruit 954 cable's individual sockets: pin1 RED +5V power, pin2 BLACK GND, pin3 GREEN adapter TX -> module RX, pin4 WHITE adapter RX <- module TX. Cable logic is 3.3V; USB power is separately 5V. This is a custom numbered connection, not a fixed four-position plug.

JP1 is removed. J1 pin1 connects directly to ADAP_IN on the ADAP_5V net. JP2 is the only TSW-102-07-G-S header with one SNT-100-BK-G shunt and grounds TEST. SW1 is the B3F-1000 reset pushbutton. C1 and C2 are GRM219R71E105KA88D, 1uF/25V X7R 0805; C1 bypasses ADAP_IN, C2 connects only BAT_IN to ground. Both capacitors are on the underside beside power probes P1/P2, clear of the top-side locator. Their power-pad connections are approximately 1.8mm long; their ground pads share a short local connection returning to ground probe P7 (BM83 pad16). Never feed BAT_IN from USB5V. P1-P10 are INGUN GKS-080-0001 probes. Four corner holes support the PCB; four inner holes attach the locator/clamp. A toggle-clamp bracket is not part of these files.

## Exact physical-pad and programming map

| Probe | BM83 physical pad | Electrical net | Module X mm | Module Y mm |
| --- | --- | --- | --- | --- |
| P1 | 22 | ADAP_5V | 4.00 | 31.50 |
| P2 | 23 | BAT_CAP_ONLY | 5.00 | 31.50 |
| P3 | 29 | UART_RX | 11.00 | 31.50 |
| P4 | 30 | UART_TX | 12.00 | 31.50 |
| P5 | 31 | TEST | 14.50 | 30.10 |
| P6 | 43 | RESET_N | 14.50 | 18.10 |
| P7 | 16 | GND | 0.50 | 26.10 |
| P8 | 50 | GND | 14.50 | 11.10 |
| P9 | 56 | GND | 12.27 | 21.53 |
| P10 | 57 | GND | 4.21 | 11.42 |

X/Y are top view with antenna upward, measured from module top-left. Module origin on PCB = (107.50,50.00) mm. Probe pin1 is a terminal on the fixture: its reference P1 etc. is NOT the BM83 pad number. The table and schematic Value fields state which actual module pad it touches. Do not mirror the layout. Ground-pad contact points use the regions in Fig8-2; perimeter points follow the actual module pads in Fig8-1, not the extended solder lands.

## Why these are the programming pins

[Microchip DS70005402H, Table2-2, printed pp13-15](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf#page=13) identifies pad22 ADAP_IN, pad23 BAT_IN, pad29 P8_6/UART_RXD, pad30 P8_5/UART_TXD, pad31 P3_4/SYS_CFG, pad43 RST_N and ground pads16/50. [Fig8-1/8-2, pp56-57](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf#page=56) identify package geometry and additional grounds56/57.

[Sec6.3.1/Table6-1, p44](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf#page=44) specifies 5V at ADAP_IN and P3_4 LOW during startup/reset to enter Test mode. [Sec6.4 notes2-3, p46](https://ww1.microchip.com/downloads/aemDocuments/documents/WSG/ProductDocuments/DataSheets/BM83-Bluetooth-Stereo-Audio-Module-Data-Sheet-DS70005402.pdf#page=46) specifically assigns P3_4 to mode selection and recommends reserving P8_5/P8_6 for Flash download in Test mode. Those are the programming functions used here. Normal-application GPIO assignments are separate; no application-button GPIO mapping is assumed.

Sec6.2 p43 describes internally pulled-up active-low reset. Table9-2/9-3 p58 describes power/logic limits. SYS_PWR, VDD_IO, USB D+/D-, analog audio and production-test pads51-55/58-59 are not contacted. This fixture is for firmware/configuration loading, not complete audio/RF qualification.

## Direct PCB probe mounting — important physical detail

[INGUN's GKS-080-0001 drawing, p1](https://ingun.com/media/01/86/fe/1752238559/INGUN_GKS-080_301_035_A_0805_GKS-080-0001_EN_Datasheet.pdf?ts=1774801120) is available at the manufacturer link. The barrel is0.70mm, collar0.80mm, lower electrical terminal0.30mm nominal and3.00mm long; overall free length22.30mm. The drawing labels the lower terminal **spring loaded** as well as the top plunger. Do not treat it as a structural rigid mounting post.

The custom PCB footprint uses a **0.45mm finished plated hole and0.80mm circular copper pad** for that0.30mm terminal. This gives0.175mm nominal annular ring and0.20mm copper gap between adjacent1mm-pitch probes. It is our designed footprint based on the drawing, not an INGUN-supplied land pattern. A0.70mm body hole is deliberately NOT used as a plated electrical mounting hole.

`Fixture.scad` is a nonconductive locator with0.70mm barrel bores and collar support at Z10.20mm above the PCB top. Finish bores/cavity to actual parts with adequate machining/print accuracy; ordinary FDM dimensional accuracy is not assumed. All collars must seat consistently. The lower barrels end at Z1.20; terminal tips are nominally Z-1.80, protruding0.20mm through a1.60mm PCB. Free top tips are Z20.50; module underside supports are Z17.50, giving3.00mm nominal working travel. Manufacturer maximum travel is3.80mm. The shield top/bar stops are nominally Z20.00 for a2.50mm-thick module. Actual probe/module tolerances require height measurement and shimming; these are not guaranteed fit dimensions.

The locator carries the axial clamping load through the collars, so the thin solder terminals do not carry that load. Insert probes into the locator, align all lower terminals into PCB holes without forcing them, secure the locator and then solder the designated lower terminals from the underside with controlled heat. Avoid solder wicking into barrels; never solder the upper moving tips or the BM83. Check continuity after soldering and while cycling the fixture. Do not omit the locator and expect solder joints alone to align/support the probes.

The locator uses PCB origin-local (100,50) mm. Clamp holes locally (2.5,12.5), (27.5,12.5), (2.5,33.5), (27.5,33.5). Use four M3x30 screws and washers through the bars/locator/PCB into four20mm female standoffs below; verify actual thread engagement. Four M3x6 screws, washers and four10mm standoffs support the outer PCB corners. Export `part="locator"`, `"upper_bar"`, `"lower_bar"` separately in OpenSCAD; `"assembly"` illustrates their relation. Antenna extends over the board notch, clear of clamp pressure.

## Use

Disconnect USB and any bench supply before changing module placement or JP2. For bench power, set 5.0V, connect positive to J1 pin1 and negative to J1 pin2; disconnect/insulate the adapter red power wire and retain adapter ground/TX/RX. Never connect both power sources together. Fit and clamp the loose BM83; close JP2; connect the cable to the PC; press/release SW1. In Microchip isUpdate select the correct COM port,115200 baud and image count; connect, browse the compatible firmware/DSP/configuration image set and update. Require successful completion/verification. Disconnect in the tool and unplug USB before removing the module. To boot application mode, open JP2 while unpowered and power/reset again. The configuration uses Host MCU mode with single-ended stereo output. STM32 firmware is programmed separately.

[DS50002902B Sec5.1/Table5-1, printed pp20-23](https://www.microchip.com/content/dam/mchp/documents/WSG/ProductDocuments/UserGuides/BM83-Bluetooth-Audio-Development-Board-User-Guide-DS50002902B.pdf#page=20) documents the UART update sequence. The electronic interface is adapted from Microchip documents; the one-PCB fixture and printed locator are custom designs.

## Electrical and dimensional checks

Schematic ERC and PCB DRC, connectivity and schematic parity reports pass. The validation script also checks every probe net, all ten target pad numbers/coordinates,1mm pitch,0.45mm plated terminal holes, and the absence of J2/a separate guide PCB. The assembly procedure checks alignment, contact height/force, continuity and successful programming verification.
