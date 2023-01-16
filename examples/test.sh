if test $# -gt 0; then
    circuit_path=$1
else
    circuit_path="../examples/adder_4b.json"
fi

echo $circuit_path

ulimit -s unlimited
../build/src/client --repetition 3 --circuit $circuit_path
echo Unode_test
../build/src/runtime_unode --repetition 3 --circuit $circuit_path
../build/src/verify --repetition 3 --circuit $circuit_path
echo MPI_test
STARPU_WORKERS_NOBIND=1 mpirun -n 4 --map-by :PE=2 ../build/src/runtime_mpi --repetition 3 --circuit $circuit_path
../build/src/verify --repetition 3 --circuit $circuit_path