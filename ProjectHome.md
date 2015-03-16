A midi player for the Adobe Flash player, running in a web page.

The player integrates:
  * a midi file parser
  * support for the SoundFont 2 standard thanks to Fluidsynth

Note: the player can't connect with your local midi keyboard, nor send midi events. Flash technology does not allow this.

The project is ongoing, but the proof of concept is visible at http://www.playintune.net.
For now, you will find here the harder part: how to compile fluidsynth into a swc, and how to use this swc in AS3 code.