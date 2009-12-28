#include "midiinput.h"

using namespace std;

MidiInput::MidiInput(int p) {
	try {
	    midiIn = new RtMidiIn();
	} catch(RtError &error) {
		error.printMessage();
	}
	midiIn->openPort(p);
	portName = midiIn->getPortName(p);
	stringReplace(&portName, '_');
    midiIn->setCallback(onMidi, &portName);
	//respond to sysex and timing, ignore active sensing
    midiIn->ignoreTypes(false, false, true);
	cout << "Listening to port " << midiIn->getPortName(p) << endl;
}

MidiInput::~MidiInput() {
	delete this->midiIn;
}

void MidiInput::stringReplace(string* str, char rep) {
	replace_if(str->begin(), str->end(), bind2nd(equal_to<char>(), ' '), rep);
}

void MidiInput::onMidi(double deltatime, vector<unsigned char> *message, void *userData) {
	unsigned int nBytes = 0;
	unsigned char channel = 0, status = 0;
    string message_type;
	int j = 0, bytes = 0;
	
	try {
    	nBytes = message->size();
	} catch(RtError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}
	
	if((message->at(0) & 0xf0) != 0xf0) {
	    channel = message->at(0) & 0x0f;
	    status = message->at(0) & 0xf0;
	} else {
		channel = 0;
		status = message->at(0);
	}
	
	string *name = static_cast<string*>(userData);
	switch(status) {
		case NOTE_OFF:
		message_type = "note_off";
		bytes = 2;
		break;
		
		case NOTE_ON:
		message_type = "note_on";
		bytes = 2;
		break;
		
		case KEY_PRESSURE:
		message_type = "key_pressure";
		bytes = 2;
		break;
		
		case CONTROLLER_CHANGE:
		message_type = "controller_change";
		bytes = 2;
		break;
		
		case PROGRAM_CHANGE:
		message_type = "program_change";
		bytes = 2;
		break;
		
		case CHANNEL_PRESSURE:
		message_type = "channel_pressure";
		bytes = 2;
		break;
		
		case PITCH_BEND:
		message_type = "pitch_bend";
		bytes = 2;
		break;
		
		case SYSTEM_EXCLUSIVE:
		if(message->size() == 6) {
			unsigned int type = message->at(4);
			if(type == 1) {
				message_type = "mmc_stop";
			} else if(type == 2) {
				message_type = "mmc_play";
			} else if(type == 4) {
				message_type = "mmc_fast_forward";
			} else if(type == 5) {
				message_type = "mmc_rewind";
			} else if(type == 6) {
				message_type = "mmc_record";
			} else if(type == 9) {
				message_type = "mmc_pause";
			}
		}
		bytes = 0;
		break;
		
		case SONG_POSITION:
		message_type = "song_position";
		bytes = 2;
		break;
		
		case SONG_SELECT:
		message_type = "song_select";
		bytes = 2;
		break;
		
		case TUNE_REQUEST:
		message_type = "tune_request";
		bytes = 2;
		break;
		
		case TIMING_TICK:
		message_type = "timing_tick";
		bytes = 0;
		break;
		
		case START_SONG:
		message_type = "start_song";
		bytes = 0;
		break;
		
		case CONTINUE_SONG:
		message_type = "continue_song";
		bytes = 0;
		break;
		
		case STOP_SONG:
		message_type = "stop_song";
		bytes = 0;
		break;
		
		default:
		message_type = "";
		bytes = 0;
		break;
	}
	
	if(status == NOTE_ON && message->at(2) == 0) {
		message_type = "note_off";
	}

	lo_message m = lo_message_new();
	lo_message_add_string(m, message_type.c_str());
	
	for(j = 0; j < bytes; j++) {
		lo_message_add_int32(m, (int)message->at(j + 1));
	}

	lo_address t = lo_address_new("239.0.0.1", "7001");
	stringstream path;
	path << "/midi/" << *name;
	if(bytes > 0) {
		path << "/" << (int)channel;
	}
	string pathString;
	pathString = path.str();
	lo_send_message(t, pathString.c_str(), m);
	lo_address_free(t);
	lo_message_free(m);
}
