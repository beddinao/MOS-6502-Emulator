#!/bin/bash

# My testing script to test emulator functionality and verify that I don't break it
# while implementing new features (or refactoring old ones)




if [ -d testOutput ];
then
	NUM_FILES=i$(ls -A testOutput | wc -l)
	if [[ "$NUM_FILES" -ne "0" ]];
	then
		echo "Removing files in testOutput directory"
		rm testOutput/*
	else
		echo "testOutput directory exists, and it is empty"
	fi

else
	echo "Creating testOutput directory"
	mkdir testOutput
fi

# First argument is the root name of the test rom
# Second argument is the trace options required for the test
execute_test_case () {

	TESTCASE=$1
	TESTARGS=$2

	echo "********** Starting ${TESTCASE} ******"
	
	echo "./traceUtility.py testbin/${TESTCASE}.rom.bin --config config.json ${TESTARGS}"	
	./traceUtility.py testbin/${TESTCASE}.rom.bin ${TESTARGS}

	mv emulator_stdout.txt emulator_stderr.txt testOutput
	mv trace_output.txt testOutput/${TESTCASE}-trace.txt

	# Verify the results
	cmp testOutput/${TESTCASE}-trace.txt expectedResults/${TESTCASE}-trace.txt

	if [ $? -eq 0 ];
	then
		echo "${TESTCASE} results verified successfully with previous results"
	else
		echo "${TESTCASE} results don't match previous results"
		#exit 1
	fi

}

execute_test_case test00-loadstore     "f000 -d=0000 -d=0200"
execute_test_case test01-andorxor      "f000 -d=0000"
execute_test_case test02-incdec        "f000 -d=0000 -d=0100"
execute_test_case test03-bitshifts     "f000 -d=0000 -d=0100"
execute_test_case test04-jumpsret      "f000 -d=0000"
execute_test_case test05-reginstrs     "f000 -d=0000"
execute_test_case test06-addsub        "f000 -d=0000 -d=0100"
execute_test_case test07-cmpbeqbne     "f000 -d=0000 -d=0100"
execute_test_case test08-cpxybit       "f000 -d=0000 -d=0100"
execute_test_case test09-otherbranches "f000 -d=0000 -d=0100"
execute_test_case test10-flaginstrs    "f000 -d=0000"
execute_test_case test11-stackinstrs   "f000 -d=0000"
execute_test_case test12-rti           "f000 -d=0000 -d=0100"
execute_test_case test13-specialflags  "f000 -d=0000 -d=0100"

echo "All tests completed successfully"


