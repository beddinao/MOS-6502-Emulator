#!/usr/bin/env python

from dbgClient import *
import sys
import subprocess
import time
import re

def printUsage(progName):
    print("This utility will trace the execution of a binary in the 6502 emulator by printing all registters")
    print("after each instruction is executed")
    print("")
    print("{} binaryFile loadAddressHex [-t=numStepsTraceDec]").format(progName)
    print("{} --config   config.json    [-t=numStepsTraceDec]").format(progName)
    print("")
    print("You must provide a binary and address to load the binary.  You can optionally specify the number of")
    print("steps you want to trace.")
    print("")

def main(args):
    if (len(args) < 3):
        printUsage(args[0])
        return

    if ( args[1] == "--config" ):
        configFile = args[2]
        spawnArgs = [ './emu6502', "-c", configFile ]
    else:
        binaryFile = args[1]
        loadAddress = int(args[2], 16)
        spawnArgs = [ './emu6502', "-f", binaryFile, "-b", hex(loadAddress), "-c", "../configs/emudev.json"]

    if ( "--profiling" in args ):
        print ("Running with profiling")
        # Want to add one of the following options
        # valgrind --tool=callgrind -v --dump-every-bb=10000
        # perf record -a -g -F 97
        profilingPrefix = "perf record -a -g -F 97"
        pArgs = profilingPrefix.split()

        for singlePArg in reversed(pArgs):
            spawnArgs.insert(0, singlePArg)

    print("Emulator cmd line={}".format(" ".join(spawnArgs)))
    
    numSteps = None
    if (len(args) >= 4):
        for argNum in range(3,len(args)):
            curArg = args[argNum]

            if curArg.startswith("-t="):
                # User specified number of trace steps
                numSteps = int(curArg[3:])
                print("Tracing {} steps".format(numSteps))

    if numSteps != None:
        spawnArgs.append("-n")
        spawnArgs.append(str(numSteps))

    print("Emulator cmd line={}".format(" ".join(spawnArgs)))

    print("Going to spawn the emulator process")
    
    emulatorStdOut = open("emulator_stdout.txt", "w+")
    emulatorStdErr = open("emulator_stderr.txt", "w+")

    # Start the emulator in an external process
    p = subprocess.Popen(spawnArgs, stdout=emulatorStdOut, stderr=emulatorStdErr)

    # Wait for the process to exit
    p.wait()

    emulatorStdOut.close()
    emulatorStdErr.close()

if __name__ == "__main__":
    main(sys.argv)
