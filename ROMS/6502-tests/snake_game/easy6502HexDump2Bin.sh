#!/bin/bash

cat $1 | cut -d ':' -f 2 | ./hex2bin - > $2




