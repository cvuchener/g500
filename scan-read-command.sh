#!/bin/bash

hidraw=$1

for i in {0..255}; do
	for type in short long; do
		data=$(src/logitech-raw-command "$hidraw" $i read $type 2> /dev/null)
		error=$?
		case $error in
		0)
			printf "Command 0x%02x: $type\n" $i
			echo $data
			;;

		2)
			;;

		10)
			printf "Command 0x%02x: $type - unavailable\n" $i
			;;

		11)
			printf "Command 0x%02x: $type - invalid params\n" $i
			;;
		
		*)
			printf "Command 0x%02x: $type - error: %d\n" $error
			exit -1
		esac
	done
done
