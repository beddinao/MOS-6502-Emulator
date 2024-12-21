#!/usr/bin/env python

# Short script for changing test output to format of output before some big changes were made.  Delete me in a few commits.


import sys

f = open(sys.argv[1], 'r')
o = open(sys.argv[2], 'w')

alltext = f.read()
lines = alltext.split('\n')
lineNum = 0
for singleLine in lines:
    print("{} {}".format(lineNum, singleLine))

    posSp = singleLine.find("SP=")
    if (posSp == -1):
        print singleLine
        print "No SP found"
        o.write(singleLine + "\n")
        continue

    spValStr = singleLine[posSp + 3: posSp + 5]
    spVal = int(spValStr, 16)
    spNewVal = spVal + 2
    spNewValStr = "000" + hex(spNewVal)[2:]
    spNewValStr = spNewValStr[-2:]

    singleLine = singleLine[:posSp+3] + spNewValStr + singleLine[posSp + 5:]

    posSr = singleLine.find("SR=")

    if posSr == -1:
        print singleLine
        o.write(singleLine + "\n")
        continue

    srRegIn = singleLine[posSr+3:posSr + 5]
    print("SR = {}".format(srRegIn))

    hexVal = int(srRegIn, 16)

    hexValOut = hexVal ^ 0x24

    hexOutStr = ("0000" + hex(hexValOut)[2:])[-2:]

    print("SR = {} -> {}".format(hex(hexVal), hexOutStr))

    hexFixed = singleLine[:posSr+3] + hexOutStr + singleLine[posSr+5:]


    final = hexFixed.replace("FLG_INTD | ", "")
    final = final.replace("FLG_INTD", "")

    print final
    o.write(final + "\n")

f.close()
o.close()


