# Buntan
Buntan is a generic engine for evaluating logical circuits over TFHE, a fully homomorphic encryption.
Buntan currently supports [TFHEpp](https://github.com/virtualsecureplatform/TFHEpp).

Buntan Supports parallel evaluation due to MPI support.
Buntan utilize [StarPU](https://starpu.gitlabpages.inria.fr/) for scheduling tasks and MPI support.

## Build
Buntan requires MPIC++, MPIExec, and StarPU installed.
Since Main-Support utility, which requires MPI Master Slave Support of StarPU will be implemented, StarPU 1.3.10 is recommended.

```
$ mkdir build && cd build
$ cmake ..
$ make
```

## Options
 - plain_mode     -evaluate in plain operand
 - dump_mode      -show debug info
 - use_simple_FF  -ignore clock and reset input of FF
 - perf_measure   -measure and show duration of procesing
 - use_lvl0       -use TLWElvl0 as a format of coded message
