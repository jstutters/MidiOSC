#pragma once
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "RtMidi.h"
#include "lo/lo.h"

#include "midimessages.h"

class MidiInput {
	private:
		int portNumber;
		RtMidiIn* midiIn;
		void stringReplace(std::string*, char);
		
	public:
		MidiInput(int);
		~MidiInput();
		static void onMidi(double, std::vector<unsigned char>*, void*);
		std::string portName;
};
