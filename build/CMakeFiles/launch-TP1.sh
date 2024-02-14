#!/bin/sh
bindir=$(pwd)
cd /home/pie/gitfolder/TP1_MoteurJeux/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/pie/gitfolder/TP1_MoteurJeux/build/TP1 
	else
		"/home/pie/gitfolder/TP1_MoteurJeux/build/TP1"  
	fi
else
	"/home/pie/gitfolder/TP1_MoteurJeux/build/TP1"  
fi
