# IR remote control on an adruino via a rpi

An arduino is used to set the beacon mode/colour using an IR led with the NEC protocol at 38kHz.

The arduino's serial interface is used so that a raspberry pi can tell the arduino what it should do.

IR led (ky-005) is in pin 10 on the arduino

<video src="Video.mp4" controls title="Title"></video>

the IR codes that are transmitted were found by reading the beacon's original remote's transmission codes with the BeaconRemoteCodeReader arduino file with a ky-022