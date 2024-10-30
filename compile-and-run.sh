#!/usr/bin/env bash

# This script compiles z16 and z16c
# Then compiles $1 (provide .asm)
# Then runs output.bin

if [ -z $1 ]; then
	echo "Provide .asm"
else
	make -B
	if [[ $? != 0 ]]; then
		echo "Failed to build"
	else
		build/z16c $1
		if [[ $? != 0 ]]; then
			echo "[Compile-and-run] Failed to compile, exiting..."
			exit -1
		fi
		build/z16 output.bin
	fi
fi

