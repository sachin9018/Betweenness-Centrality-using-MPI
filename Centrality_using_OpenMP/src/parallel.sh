#!/bin/bash
#$ -N parallel_brandes
#$ -q class8i
#$ -pe openmp 8

# Module load gcc compiler version 4.8.2
module load  gcc/4.9.2

# Runs a bunch of standard command-line
# utilities, just as an example:

echo "Script began:" `date`
echo "Node:" `hostname`
echo "Current directory: ${PWD}"

echo ""

./parallel_brandes data_set.txt myrun_file.txt BC_file.txt 40000

echo ""
echo "=== Done! ==="

# eof
