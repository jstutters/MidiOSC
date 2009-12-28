#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include "RtMidi.h"
#include "midiinput.h"

#include "lo/lo.h"

using namespace std;
			
void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") {
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
	    // Found a token, add it to the vector.
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    // Skip delimiters.  Note the "not_of"
	    lastPos = str.find_first_not_of(delimiters, pos);
	    // Find next "non-delimiter"
	    pos = str.find_first_of(delimiters, lastPos);
	}
}

vector<int> selectPorts() {
	int port;
	vector<string>::iterator it;
	string userInput;
	vector<string>userInputTokenized;
	vector<int> portList;
	
	cout << "Select ports: ";
	getline(cin, userInput);
	tokenize(userInput, userInputTokenized);
	
	for (it = userInputTokenized.begin(); it < userInputTokenized.end(); it++) {
		stringstream(*it) >> port;
		portList.push_back(port);
	}
	return portList;
}

void listInputPorts() {
	RtMidiIn* midiIn = new RtMidiIn();
    unsigned int nPorts = midiIn->getPortCount();
	cout << "Found " << nPorts << " MIDI inputs." << endl;
	string portName;
    if(nPorts == 0) {
        cout << "No ports available!\n";
    } else {
		for(unsigned int i = 0; i < nPorts; i++) {
			portName = midiIn->getPortName(i);
			cout << "   Input port #" << i+1 << ": " << portName << endl;
		}
	}
	delete midiIn;
}

void listOutputPorts() {
	RtMidiOut* midiOut = new RtMidiOut();
    unsigned int nPorts = midiOut->getPortCount();
	cout << "Found " << nPorts << " MIDI outputs." << endl;
	string portName;
    if(nPorts == 0) {
        cout << "No ports available!\n";
    } else {
		for(unsigned int i = 0; i < nPorts; i++) {
			portName = midiOut->getPortName(i);
			cout << "   Output port #" << i+1 << ": " << portName << endl;
		}
	}
	delete midiOut;
}

void error(int num, const char *msg, const char *path)
{
	cout << "liblo server error " << num << " in path " << path << ": " << msg << endl;
}

map<string, int> populateMap() {
	map<string, int> stringToStatus;
	stringToStatus["note_off"] = 0x80;
	stringToStatus["note_on"] = 0x90;
	stringToStatus["key_pressure"] = 0xa0;
	stringToStatus["controller_change"] = 0xb0;
	stringToStatus["program_change"] = 0xc0;
	stringToStatus["channel_pressure"] = 0xd0;
	stringToStatus["pitch_bend"] = 0xe0;
	stringToStatus["system_exclusive"] = 0xf0;
	stringToStatus["song_position"] = 0xf2;
	stringToStatus["song_select"] = 0xf3;
	stringToStatus["tune_request"] = 0xf6;
	stringToStatus["end_of_sysex"] = 0xf7;
	stringToStatus["timing_tick"] = 0xf8;
	stringToStatus["start_song"] = 0xfa;
	stringToStatus["continue_song"] = 0xfb;
	stringToStatus["stop_song"] = 0xfc;
	return stringToStatus;
}

int generic_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	vector<unsigned char> msg;
	map<string, RtMidiOut*>::iterator it;
	map<string, int> stringToStatus = populateMap();
	
	string pathString = path;
	vector<string>pathTokenized;
	tokenize(pathString, pathTokenized, "/");
	if(pathTokenized[0].compare("midi") != 0) {
		return 1;
	}
	
	int channel = 0;
	if(pathTokenized.size() == 3) {
		string token = pathTokenized[2];
		stringstream(token) >> channel;
	}
	string statusString = &argv[0]->s;
	int status = stringToStatus[&argv[0]->s] + channel;
	msg.push_back((unsigned char)status);
	for(int i = 1; i < argc; i++) {
		cout << argv[i]->i << endl;
		msg.push_back((unsigned char)argv[i]->i);
	}
		
	map<string, RtMidiOut*> *m = (map<string, RtMidiOut*>*)user_data;
	it = m->find(pathTokenized[1]);
	RtMidiOut* midi = it->second;
	midi->sendMessage(&msg);
	return 0;
}

int main(int argc, char* argv[]) {
	vector<MidiInput*> inputs;
	vector<int> portList;
	vector<int>::iterator portIterator;
	vector<MidiInput*>::iterator inputIterator;
	
	listInputPorts();
	portList = selectPorts();

	for (portIterator = portList.begin(); portIterator < portList.end(); portIterator++) {
		*portIterator -= 1;
		MidiInput* m  = new MidiInput(*portIterator);
		inputs.push_back(m);
	}
	
	listOutputPorts();
	portList = selectPorts();

	map<string, RtMidiOut*> portMap;
	RtMidiOut* midiOut = new RtMidiOut();
	RtMidiOut* p;
	for (portIterator = portList.begin(); portIterator < portList.end(); portIterator++) {
		*portIterator -= 1;
		p = new RtMidiOut();
		p->openPort(*portIterator);
		portMap[midiOut->getPortName(*portIterator)] = p;
	}
	
	lo_server_thread st = lo_server_thread_new("8000", error);
	lo_server_thread_add_method(st, NULL, NULL, generic_handler, &portMap);
	lo_server_thread_start(st);
	
    cout << "\nReading MIDI input ... press <enter> to quit.\n";
    char input;
    cin.get(input);

	for (inputIterator = inputs.begin(); inputIterator < inputs.end(); inputIterator++) {
		delete *inputIterator;
	}
}
