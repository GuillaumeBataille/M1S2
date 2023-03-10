#!/bin/sh
bindir=$(pwd)
cd /home/bambipc/Bureau/M1S2/Moteur/Base_Code/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/bambipc/Bureau/M1S2/Moteur/Base_Code/build/TP1 
	else
		"/home/bambipc/Bureau/M1S2/Moteur/Base_Code/build/TP1"  
	fi
else
	"/home/bambipc/Bureau/M1S2/Moteur/Base_Code/build/TP1"  
fi
