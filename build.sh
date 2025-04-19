#!/bin/bash

config="$1"
appName="Sandbox"

if [[ $config = "run" ]]; then
	mode="$2"
	system="$3"

	if [[ $mode = "debug" ]]; then
		cd "bin/Debug-$system/$appName"
		LD_LIBRARY_PATH="." ./$appName
	elif [[ $mode = "release" ]]; then
		cd "bin/Release-$system/$appName"
		LD_LIBRARY_PATH="." ./$appName
	elif [[ $mode = "dist" ]]; then
		cd "bin/Dist-$system/$appName"
		LD_LIBRARY_PATH="." ./$appName
	fi
elif [[ $config = "assets" ]]; then
	mode="$2"
	system="$3"
	if [[ $mode = "debug" ]]; then
		cp -r "Sandbox/assets/" "bin/Debug-$system/$appName/"
	elif [[ $mode = "release" ]]; then
		cp -r "Sandbox/assets/" "bin/Release-$system/$appName/"
	elif [[ $mode = "dist" ]]; then
		cp -r "Sandbox/assets/" "bin/Dist-$system/$appName/"
	fi
elif [[ $config = "ecc" ]]; then
	system="$2"
	"./vendor/premake/$system/premake5" export-compile-commands
	cp "compile_commands/debug.json" "compile_commands.json"
elif [[ $config = "clean" ]]; then
	rm -rf bin build
	rm Makefile
	rm Sandbox/Makefile
	rm Luma/Makefile
else
	system="$2"
	"./vendor/premake/$system/premake5" gmake
	make all config=$config -j7
fi
