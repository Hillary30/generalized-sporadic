#!/bin/bash 
#SBATCH -J fourteenth_job_ds 
#SBATCH -o fourteenth_job_ds.o%j 
#SBATCH -e fourteenth_job_ds.o%j.err
#SBATCH --mail-user=hillarynguyenco@gmail.com
#SBATCH --mail-type=FAIL,END,BEGIN
#SBATCH --ntasks-per-node=28 -N 1
#SBATCH -t 24:00:00 
#SBATCH --mem-per-cpu=1GB
COUNT=1000
UTILIZATION=900

mpiicpx -o main main.cpp

mpirun ./main $COUNT $UTILIZATION