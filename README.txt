/* Flash Midi Player - A midi player for the Adobe Flash Player
 *
 * Copyright (C) 2009 Yoann Huiban, yoann.huiban@gmail.com.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

=======================================================================
Flash midi player
=======================================================================
The goal of this project is to build a flash midi player allowing to 
play General Midi files inside a Flash player.

Note: This project will not allow you to control a midi keyboard, or to 
send events to a midi device. It runs alone in the flash player and can not 
communicate with any local ressource of your computer

Technology:
-----------
There were a lot of attempts to build a Flash midi player. The issue was, 
until flash 10, to be able to produce the sound, either by sending it to 
the local midi device (soundcard) via midi events or by generating the samples 
directly into the flash player with an embedded synthetizer. Since Flash player 10 
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

Then, here are the commands I type in cygwin (perhaps in the future I will 
write an better script):

# ------------
# Alchemy init
# ------------
# change the following vars according to your configuration
export FLASH_MIDI_PLAYER_DIR=/cygdrive/c/dev/projects/googlecode/flash-midi-player
export ALCHEMY_HOME=/cygdrive/c/dev/alchemy-cygwin-v0.5a
export FLEX_SDK_DIR=/cygdrive/c/dev/flex_sdk_3.2.0.3958

export PATH="${FLEX_SDK_DIR}/bin:/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin:/cygdrive/c/WINDOWS/system32:/cygdrive/c/WINDOWS:/cygdrive/c/WINDOWS/System32/Wbem"
export ADL=${FLEX_SDK_DIR}/bin/adl.exe
source ${ALCHEMY_HOME}/alchemy-setup
alc-on


# ----------------
# Fluidsynth build
# ----------------
export FLUIDSYNTH_DIR=${FLASH_MIDI_PLAYER_DIR}/fluidsynth/fluidsynth-1.0.8
cd ${FLUIDSYNTH_DIR}
# this will configure fluidsynth, and install it in $ALCHEMY_HOME/fluidsynth
./configure --prefix=$ALCHEMY_HOME/fluidsynth
make clean
make
# note: fluidsynth.exe comilation fails, but we do not need it...

# --------------------
# Fluidsynth swc build
# --------------------
cd ${FLASH_MIDI_PLAYER_DIR}/fluidsynth_swc
gcc fluidsynth_swc.c -I ${FLUIDSYNTH_DIR}/include -c -o fluidsynth_swc.o
rm -f fluidsynth_swc.swc
gcc fluidsynth_swc.o ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_settings.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_hash.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_synth.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_sys.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_cmd.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_adriver.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_list.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_mdriver.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_conv.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_defsfont.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_chan.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_voice.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_rev.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_chorus.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_dsp_float.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_mod.o \
                     ${FLUIDSYNTH_DIR}/src/libfluidsynth_la-fluid_gen.o \
                     -lpthread -lm -swc -o fluidsynth_swc.swc
cp fluidsynth_swc.swc ${FLASH_MIDI_PLAYER_DIR}/fluidsynthtest/lib

Testing Fluidsynth:
--------------------
- Import the fluidsynthtest project into Flex Builder
- Run the fluidsynthtest project: you chould hear a note playing...



