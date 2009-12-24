#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "RtMidi.h"
#include "midiinput.h"

using namespace std;
			
void tokenize(const string* str, vector<string>* tokens) {
	stringstream ss(*str);
	string buf;
	while(ss >> buf) {
		tokens->push_back(buf);
	}
}

vector<int> selectPorts() {
	int port;
	vector<string>::iterator it;
	string userInput;
	vector<string>userInputTokenized;
	vector<int> portList;
	
	cout << "Select input port: ";
	getline(cin, userInput);
	tokenize(&userInput, &userInputTokenized);
	
	for (it = userInputTokenized.begin(); it < userInputTokenized.end(); it++) {
		stringstream(*it) >> port;
		portList.push_back(port);
	}
	return portList;
}

void listPorts() {
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

void parseInputs(int argc, char* argv[]) {
	char listArg[] = "-l";
	for(int i = 1; i < argc; i++) {
		if(strcmp(listArg, argv[i]) == 0) {
			listPorts();
			exit(EXIT_SUCCESS);
		}
	}
	cout << endl;
}

int main(int argc, char* argv[]) {
	vector<MidiInput*> inputs;
	vector<int> portList;
	vector<int>::iterator portIterator;
	vector<MidiInput*>::iterator inputIterator;

	parseInputs(argc, argv);
	portList = selectPorts();

	for (portIterator = portList.begin(); portIterator < portList.end(); portIterator++) {
		*portIterator -= 1;
		MidiInput* m  = new MidiInput(*portIterator);
		inputs.push_back(m);
	}
	
    cout << "\nReading MIDI input ... press <enter> to quit.\n";
    char input;
    cin.get(input);

	for (inputIterator = inputs.begin(); inputIterator < inputs.end(); inputIterator++) {
		delete *inputIterator;
	}
}
