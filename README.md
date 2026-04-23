# MPI con Docker

## 1. Crear contenedor
```bash
mkdir mpim
docker run -itd -v /home/jabar/mpi:/mpim ubuntu bash
docker exec -it <container_id> bash
```

## 2. Instalar MPI en Ubuntu
```bash
apt update
apt install -y openmpi-bin openmpi-common libopenmpi-dev gcc
```

## 3. Crear usuario sin privilegios
```bash
useradd -m -s /bin/bash mpiuser
su - mpiuser
```

## 4. Compilar programas
```bash
cd /mpim
mpicc -o pi_mpi pi_mpi.c
mpicc -o mpi-mm mpi-mm.c
```

## 5. Ejecutar con diferentes procesos

### PI_MPI
```bash
mpirun --allow-run-as-root -np 1 ./pi_mpi
mpirun --allow-run-as-root -np 2 ./pi_mpi
mpirun --allow-run-as-root -np 3 ./pi_mpi
mpirun --allow-run-as-root -np 4 ./pi_mpi
```

### MPI-MM
```bash
mpirun --allow-run-as-root -np 1 ./mpi-mm 100
mpirun --allow-run-as-root -np 2 ./mpi-mm 100
mpirun --allow-run-as-root -np 3 ./mpi-mm 100
mpirun --allow-run-as-root -np 4 ./mpi-mm 100
```
