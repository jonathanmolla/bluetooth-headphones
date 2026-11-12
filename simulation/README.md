# LTspice audio-path approximation

Open `Headphones_Audio.asc` in LTspice with `audio_models.lib` and both local `.asy` symbols beside it.

## What is modeled

- Two ideal BM83 analog output sources.
- Four 0.47 uF coupling/reference capacitors.
- A simplified differential amplifier with adjustable input resistance and voltage gain.
- Two 38 ohm resistive loads.

Nominal illustrative parameters are VBAT = 3.7 V, source bias = 1.35 V, signal amplitude = 50 mV peak, signal frequency = 1 kHz, ideal gain = 1, and differential input resistance = 37 kohm. These are modeling assumptions, not measured hardware specifications.

## Reproduce an analysis

1. Open the schematic and inspect the active simulation directive. The supplied snapshot has `.tran 5` active.
2. For a transient run, use that directive or replace it with `.tran 0 20m 0 1u` to focus on a short interval. Run the analysis and probe the two amplifier outputs and input signals.
3. For an AC sweep, disable the transient directive and activate `.ac dec 100 10 100k`. Each source has AC amplitude 1 for normalized voltage-gain inspection.
4. Record the exact parameters, traces, axis ranges and findings when publishing results.

## Evidence status

The original project includes a dated LTspice log that confirms a simulation run completed, and generated waveform files exist. The source schematic and its old README still contain setup-only wording; those comments are stale. This repository replaces that README with instructions that reflect the active transient directive. Earlier project notes report AC and transient work, but no quantitative result or selected plot has been reverified for publication here. Large raw waveforms and machine-specific logs are omitted.

## Limits

These are educational approximations, not manufacturer device models. Bluetooth, decoding, current limiting, clipping, bandwidth, noise, distortion, charge-pump behavior, startup, mute, I2C, battery current and acoustic response are not modeled. Supply bypassing is drawn for context and does not validate supply integrity. The real amplifier input resistance varies with volume, so `AVOL` and `RINPUT` do not represent a calibrated register model.

See `docs/validation.md` for physical tests still required.
