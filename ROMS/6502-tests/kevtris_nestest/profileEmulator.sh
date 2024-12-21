#!/bin/bash

pidof emu6502
if [ $? -eq 0 ]; then
	echo "Emulator already running, please close before running this script"
	exit 1
fi

userName=$(whoami)
if [ ${userName} != "root" ]; then
	echo "Script needs to be run as root"
	exit 1
fi

echo "Going to start the emulator"
./traceUtility.py --config nestest.json -t=2000 &

# Give the traceUtility script a second to start the emulator
echo "Waiting a second for emulator to start execution (as part of traceUtility)"
sleep 1

# Now find the emulators pid
emulatorPid=$(pidof emu6502)

if [ $? -ne 0 ]; then
	echo "Unable to find the PID of the emualtor we just started, perhaps it crashed?"
	exit 2
fi

echo "Found emulator running as pid ${emulatorPid}"

perf record -p ${emulatorPid} -a -g -F 99

sudo chmod a+rw perf.data

echo "Emulator must have exitted!"

perf script > out.stacks

~/checkouts/third_party/FlameGraph/stackcollapse-perf.pl < out.stacks > fgme

~/checkouts/third_party/FlameGraph/flamegraph.pl < fgme > out.svg

chmod a+rw out.svg
chmod a+rw out.stacks

