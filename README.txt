
=======================================================================
Flash midi player
=======================================================================
The goal of this project is to build a flash midi player allowing to 
play General Midi files inside a Flash player.

Note: This project will not allow you to control a midi keyboard, or to 
send events to a midi device. It runs alone in the flash player and can't 
communicate with any local ressource of your computer

Technology:
-----------
There were a lot of attemps to build a Flash midi player. The issue was, 
until flash 10, to be able to produce the sound, either by sending it to 
the local midi device (soundcard) via midi events or by generating them 
directly into the flash player by embedding a synthetizer. Since Flash 10 
allows sending sound samples to the soundcard directly, the second solution 
is used in this project.

It integrates code from 3 others projects:
- midas3: a midi file parser written in actionscript 
  http://code.google.com/p/midas3/
- Fluisynth: a C Soundfont 2 compliant synthetizer
  http://www.fluidsynth.org/
- Adobe Alchemy: a project from Adobe Labs allowing to compile C code 
  into Flash bytecode (no Actionscript there) 
  http://labs.adobe.com/technologies/alchemy/

Licence:
--------
Since this project rely both on Fluidsynth (LGPL) and midas3 (GPLv3), the 
license is GPLv3.
Quickly said, that mean you will have to share the code of any project derived 
from this one.

=======================================================================
Building
=======================================================================

Building Fluidsynth:
--------------------
First, you have to get Alchemy there http://labs.adobe.com/technologies/alchemy/
Follow carefully the instruction to get it running.
