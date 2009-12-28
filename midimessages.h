#pragma once
//status bytes
const unsigned char NOTE_OFF = 0x80;
const unsigned char NOTE_ON = 0x90;
const unsigned char KEY_PRESSURE = 0xA0;
const unsigned char CONTROLLER_CHANGE = 0xB0;
const unsigned char PROGRAM_CHANGE = 0xC0;
const unsigned char CHANNEL_PRESSURE = 0xD0;
const unsigned char PITCH_BEND = 0xE0;
const unsigned char SYSTEM_EXCLUSIVE = 0xF0;
const unsigned char SONG_POSITION = 0xF2;
const unsigned char SONG_SELECT = 0xF3;
const unsigned char TUNE_REQUEST = 0xF6;
const unsigned char END_OF_SYSEX = 0xF7;
const unsigned char TIMING_TICK = 0xF8;
const unsigned char START_SONG = 0xFA;
const unsigned char CONTINUE_SONG = 0xFB;
const unsigned char STOP_SONG = 0xFC;
const unsigned char ACTIVE_SENSING = 0xFE;
const unsigned char SYSTEM_RESET = 0xFF;
