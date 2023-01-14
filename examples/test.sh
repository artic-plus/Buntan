ulimit -s unlimited
../build/src/client --repetition 3 --circuit ../examples/adder_4b.json 
echo Unode_test
../build/src/runtime_unode --repetition 3 --circuit ../examples/adder_4b.json 
../build/src/verify --repetition 3 --circuit ../examples/adder_4b.json 
echo MPI_test
STARPU_WORKERS_NOBIND=1 mpirun -n 4 --map-by :PE=2 ../build/src/runtime_mpi --repetition 3 --circuit ../examples/adder_4b.json   --disable-mpi --enable-mpi-master-slave
../build/src/verify --repetition 3 --circuit ../examples/adder_4b.json 