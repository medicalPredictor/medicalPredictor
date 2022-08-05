#! /bin/bash
make
./a.out
gnuplot GnuScriptMeanFitnessOverTime
pstopdf CovidModelerLogs.eps