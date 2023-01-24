# Buntan
Buntan is a generic engine for evaluating logical circuits over TFHE, a fully homomorphic encryption.
Buntan currently supports [TFHEpp](https://github.com/virtualsecureplatform/TFHEpp).

Buntan Supports parallel evaluation due to MPI support.
Buntan utilize [StarPU](https://starpu.gitlabpages.inria.fr/) for scheduling tasks and MPI support.

## Build
Buntan requires MPIC++, MPIExec, and StarPU installed.
Since Main-Support utility requires MPI Master Slave Support of StarPU, StarPU 1.3.10 is recommended.
In order to utilize MPI utility(runtime_mpi), option "--enable-mpi" is required when building StarPU(On by default)
In order to utilize Main-Support utility(runtime_ms), option "--enable-mpi-master-slave" is required when building StarPU

```
$ mkdir build && cd build
$ cmake ..
$ make
```

## About Executables
 - client         -generate inputs of circuit and save in ./cloud.data
 - client_manual  -generate inputs manually
 - runtime_unode  -evaluate circuit without using mpi and save in ./result.data
 - runtime_mpi    -evaluate circuit on StarPU MPI Support
 - runtime_ms     -evaluate circuit on StarPU MPI Master Slave Support
 - verify         -show result by using secret key

## Compile Options
 - plain_mode     -evaluate in plain operand
 - dump_mode      -show debug info
 - use_simple_FF  -ignore clock and reset input of FF
 - perf_measure   -measure and show duration of procesing
 - use_lvl0       -use TLWElvl0 as a format of coded message

## Execution Options
 - circuit        -give path of json file which describe the circuit to evaluate(./circuit.json by default)
 - repetition     -give number of evaluation(once by default)