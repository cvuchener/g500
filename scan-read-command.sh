#!/bin/bash

hidraw=$1

for i in {0..255}; do
	for type in short long; do
		data=$(src/logitech-raw-command "$hidraw" $i read $type 2> /dev/null)
		error=$?
		if [ "$error" -eq 2 ]; then
			continue
		fi
		printf "Command 0x%02x: $type" $i
		case $error in
		0)
			printf "\n"
			echo $data
			;;

		3)
			printf " - invalid value\n"
			;;

		10)
			printf " - unavailable\n"
			;;

		11)
			printf " - invalid params\n"
			;;
		
		*)
			printf " - error: %d\n" $error
			exit -1
		esac
	done
done
