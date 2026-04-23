#!/bin/bash
cd /mpim

echo "=== PI_MPI ==="
mpirun --allow-run-as-root -np 1 ./pi_mpi
mpirun --allow-run-as-root -np 2 ./pi_mpi
mpirun --allow-run-as-root -np 3 ./pi_mpi
mpirun --allow-run-as-root -np 4 ./pi_mpi

echo ""
echo "=== MPI-MM ==="
mpirun --allow-run-as-root -np 1 ./mpi-mm 100
mpirun --allow-run-as-root -np 2 ./mpi-mm 100
mpirun --allow-run-as-root -np 3 ./mpi-mm 100
mpirun --allow-run-as-root -np 4 ./mpi-mm 100
