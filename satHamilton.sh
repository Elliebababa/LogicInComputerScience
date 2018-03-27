#!/bin/sh
for var in `seq 1 12` 
do
minisat ./hamiltonOutput/sat$var.in ./hamiltonOutput/output/sat$var.out;
done

