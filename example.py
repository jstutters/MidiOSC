#!/usr/bin/env python

# this example requires the pyliblo library from http://das.nasophon.de/pyliblo
import liblo, sys

# set the target address where MidiOSC will be listening (assumes the default port is used)
try:
    midiosc = liblo.Address("localhost", 8000)
except liblo.AddressError, err:
    print str(err)
    sys.exit()
    
# change the device name to something appropriate for your system
device_name = "midi_device"
osc_address = "/midi/" + device_name + "/0"

# send a note on with note number 60 and velocity 100
liblo.send(midiosc, osc_address, "note_on", 60, 100)

# send a note off with note number 60 and velocity 0
liblo.send(midiosc, osc_address, "note_off", 60, 0)

# now to receive some messages
try:
    server = liblo.Server(7001)
except liblo.ServerError, err:
    print str(err)
    sys.exit()

def callback(path, args, types, src):
    print "Got $s" % (path)
    for a, t in zip(args, types):
        print "Argument type: %s, value: %s" % (t, a)

server.add_method(None, None, callback)

print "Kill the process with ctrl-c when you get bored"
while True:
    server.recv(100)
