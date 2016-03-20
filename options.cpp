// Copyright (C) 2010 Jonny Stutters
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "options.h"

using namespace std;

Options::Options() {
	this->opt = new AnyOption();
	
	this->opt->addUsage("Usage: ");
    this->opt->addUsage(" -h  --help         Prints this help");
    this->opt->addUsage(" -i  --input-port   Sets the port to listen for OSC messages on");
    this->opt->addUsage(" -o  --output-port  Sets the port to send OSC messages on");

    this->opt->setFlag("help", 'h');
    this->opt->setOption("input-port", 'i');
    this->opt->setOption("output-port", 'o');
}

Options::~Options() {
	delete this->opt;
}

void Options::processArguments(int argc, char* argv[]) {
	this->opt->processCommandArgs(argc, argv);
    if(this->opt->getFlag("help") || this->opt->getFlag('h')) {
        this->opt->printUsage();
    }
  
    if(this->opt->getValue("input-port") != NULL || this->opt->getValue("input-port") != NULL) {
        stringstream ss(stringstream::in | stringstream::out);
        ss << this->opt->getValue('i');
        ss >> this->inputPort;
    } else {
    	this->inputPort = 8000;
    }
  
    if(this->opt->getValue("output-port") != NULL || this->opt->getValue("output-port") != NULL) {
        stringstream ss(stringstream::in | stringstream::out);
        ss << this->opt->getValue('o');
        ss >> this->outputPort;
    } else {
        this->outputPort = 7001;
    }
}
