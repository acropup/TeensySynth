# Teensy Synth

This is an ongoing project of retrofitting a cheap 44-note electronic keyboard with a [Teensy 3.2](https://www.pjrc.com/teensy/teensy31.html) and [Teensy Audio Adaptor](https://www.pjrc.com/store/teensy3_audio.html). See [KeyboardSensor.h](./KeyboardSensor.h) for code and documentation regarding the Teensy keyboard sensing interface.

Goals:
* Configurable switch between standard ([Equal Temperament](https://en.wikipedia.org/wiki/Equal_temperament)) tuning and [Just Intonation](https://en.wikipedia.org/wiki/Just_intonation).
* MIDI input/output.
* Glissando/portamento between keys.
* Electrical connection points such that one can connect an oscilloscope to the right and left channel and visualize the audio.
* Variety of waveforms and samples to choose from.
* Remove all circuit limitations regarding key ghosting. The original keyboard was not very good at this, and you couldn't press two keys simultaneously if they were a multiple of 8 keys apart from each other. Will require some hardware changes (adding diodes next to every key).