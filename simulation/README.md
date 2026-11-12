# LTspice stereo audio simulation

[Headphones_Audio.asc](Headphones_Audio.asc) models the analog audio chain from the BM83 output sources through the AC-coupling network to a differential amplifier and stereo resistive loads.

## Circuit and model files

| File | Purpose |
| --- | --- |
| `Headphones_Audio.asc` | Stereo audio simulation schematic |
| `audio_models.lib` | Ideal source and differential voltage-gain models |
| `BM83_AUDIO_APPROX.asy` | Analog source block symbol |
| `TPA6130A2_AUDIO_APPROX.asy` | Amplifier model symbol |

Keep all four files in the same folder when opening the schematic in LTspice.

## Model parameters

| Parameter | Value | Role |
| --- | --- | --- |
| `VBAT` | 3.7 V | Supply reference shown on the schematic |
| `VBIAS` | 1.35 V | Ideal source DC bias |
| `VSIG` | 50 mV peak | Transient source amplitude |
| `FTEST` | 1 kHz | Transient source frequency |
| `AVOL` | 1 | Ideal amplifier voltage gain |
| `RINPUT` | 37 kohm | Differential input resistance |
| Coupling capacitors | 0.47 uF | Signal and reference-input AC coupling |
| Output loads | 38 ohm per channel | Resistive driver representation |

These parameters define an idealized analog-network model. The model focuses on coupling-network response and voltage gain; source bias and driver resistance are explicit analysis assumptions. Amplifier input resistance and gain can be varied independently for comparison.

## Transient analysis

The supplied schematic activates `.tran 5`. Run it and probe the amplifier input and output signals. To inspect a short interval with a controlled maximum timestep, use `.tran 0 20m 0 1u`.

## AC analysis

Disable the transient directive and activate `.ac dec 100 10 100k`. The two sources use an AC amplitude of 1, allowing normalized voltage-gain inspection across the sweep.

Record the selected parameters, plotted nodes and axis ranges with exported results. See the [engineering check documentation](../docs/validation.md) for the associated design-review workflow.
