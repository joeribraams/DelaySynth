# DelaySynth
DelaySynth is a synthesizer based off of a bucket brigade delay pedal. Noisy, unpredictable and not very good sounding, but it does have an interesting character I have yet to find anywhere else. It's quite interactive in the way it plays, larger intervals seem to be better intonated than small ones, the timbre changes depending on whether you play an ascending or descending line, and where the pitch when the system is "idle" is relative to the played pitch.

I copied the schematic for a Boss DM-2 for the analog part, with an arduino nano taking care of the digital side of things, such as midi input. The level and feedback are controlled with a digital potentiometer, and the time is controlled with a LED/LDR couple, as digipots don't support the large range of the original pot. Both the PWM powering the LED and the digipot have 8 bit resolution, which is a bit of a limiting factor for the intonation.

# Demo:
https://www.youtube.com/watch?v=6eWO1zAZjJ4

# Schematic:
![Schematic](/Schematic.png)
