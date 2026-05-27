# parallels

Three C++ labs exploring parallel computing: numerical integration via pthreads, OpenMP, and multithreaded file processing with `std::thread`.

## Tech Stack

C++17, pthreads, OpenMP, `std::thread`, `std::filesystem`

## Labs

### Lab 1 — Parallel integration with pthreads

Computes `∫₀¹ x² dx` using the midpoint rectangle method. The integration range `[a, b]` is split evenly across `N` threads, each integrating its own subinterval. A shared `pthread_mutex_t` protects the result accumulation.

**Notable:** thread count is entered at runtime; `ThreadData` struct bundles all per-thread state (bounds, step count, shared result pointer, mutex pointer) — clean data passing without globals.

```bash
cd first_lab
g++ --std=c++17 first_lab.cc -o first_lab -lpthread
./first_lab
```

---

### Lab 2 — Parallel integration with OpenMP

Same integral, rewritten with `#pragma omp parallel for` and `reduction(+:result)`. Each loop iteration computes a tiny sub-subinterval independently.

**Notable:** the outer loop iterates `n = 1,000,000` times (one iteration per rectangle), with `local_n = n / numThreads` sub-steps inside each — effectively a two-level decomposition that doesn't match typical OpenMP usage. The `reduction` clause eliminates any need for explicit locks.

```bash
cd second_lab
g++ --std=c++17 second_lab.cc -o second_lab -fopenmp
./second_lab
```

---

### Lab 3 — Multithreaded GCD and prime factorization from files

Two separate implementations:

**`third_lab.cc`** — reads pairs of integers from a file, spawns one `std::thread` per line to compute GCD (recursive Euclidean algorithm), writes each result to a separate output file `out_N.txt`. Uses `std::filesystem` to create the output directory if missing. Threads are batched in groups of `std::thread::hardware_concurrency()` — natural thread pool without a library.

**`third_lab_.cc`** — reads single integers, computes prime factorization (`O(√n)` trial division), outputs to per-thread files named with the thread ID in hex (`outABCDEF_1.txt`). Uses two persistent threads — one for I/O, one for computation — both sharing a single mutex-protected `ifstream`. Thread IDs in filenames make output traceable to the specific thread that produced it.

```bash
cd third_lab

# GCD variant
g++ --std=c++17 third_lab.cc -o third_lab
./third_lab input.txt output_dir

# Prime factorization variant
g++ --std=c++17 third_lab_.cc -o third_lab_
./third_lab_
```
