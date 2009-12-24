import os, sys

libs = ['lo']

if sys.platform == 'darwin':
    env = Environment(
        FRAMEWORKS = ['CoreMidi', 'CoreAudio', 'CoreFoundation'],
        CCFLAGS = '-D__MACOSX_CORE__'
    )
else:
    env = Environment(
        CCFLAGS = '-D__LINUX_ALSASEQ__'
    )
    libs.append(['asound', 'pthread'])

env.Program('midiosc', ['main.cpp', 'midiinput.cpp', 'RtMidi.cpp'], LIBS=libs)