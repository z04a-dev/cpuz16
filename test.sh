#!/usr/bin/env bash
# simple testing program for cpuz16
# by z04a

make -B

if [[ $? != 0 ]]; then
	echo "Failed to build"
else
	files=$(find tests/ -type f -iname "*.asm")
	files_count=$(echo $files | wc -w)

	if [[ $files_count == 0 ]] ; then
		echo "Please put .asm tests in tests/ folder"
	else 
		for file in $files; do
			if [[ $file == "tests/deepjump.asm" ]] ; then
				echo "Skipping deepjump.asm, currently not supported."
				continue
			fi
			echo "--------------------"
			echo "EXECUTING " $file
			./build/z16 $file 
		done
	fi
fi


