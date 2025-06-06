# Introduction to Scientific Computing

Theoretical and practical study of the main numerical methods used in scientific computing, with a focus on efficient implementation on serial architectures. The course also addresses code optimization techniques and resilience to numerical errors.

---

## EP01: Solving Nonlinear Equations

### Objective
Implement a program to solve nonlinear equations using the Bisection and Newton-Raphson methods. The Newton-Raphson method should be implemented with both fast and slow approaches for derivative computation.

### Example Program Execution

**Input:**

4
</br>
2 4 3 -10 -15
</br>
0 3

Represents the equation: `2x⁴ + 4x³ + 3x² − 10x − 15 = 0`, with the root search interval ξ ∈ [0, 3].

**Output:**
 ```c
FAST
bisection <root> <tolerance-01> <iterations> <time>
bisection <root><tolerance-02> <iterations> <time>
bisection <root> <tolerance-03> <iterations> <time>
newton <root> <tolerance-01> <iterations> <time>
newton <root> <tolerance-02> <iterations> <time>
newton <root> <tolerance-03> <iterations> <time>

SLOW
bisection <root> <tolerance-01> <iterations> <time>
bisection <root><tolerance-02> <iterations> <time>
bisection <root> <tolerance-03> <iterations> <time>
newton <root> <tolerance-01> <iterations> <time>
newton <root> <tolerance-02> <iterations> <time>
newton <root> <tolerance-03> <iterations> <time>
```


---

## EP02: Solving Tri-diagonal Linear Systems

### Objective
Implement a program to solve a family of Ordinary Differential Equations (ODEs) using the LU Factorization Method.

### Example Program Execution

**Input:**

5
</br>
0 3.14159265
</br>
2 -2
</br>
0 1
</br>
0 0 0 0
</br>
0 0 1 0
</br>

**Output:**

 ```c
5
 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00 -2.000000000000000e+00
  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00
  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00
  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00 -0.000000000000000e+00
  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  2.000000000000000e+00

  1.744564700038279e+00  1.010847083350622e+00  3.147679729430032e-14 -1.010847083350569e+00 -1.744564700038249e+00
  3.02900187e-03

5
 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00 -1.762574218887082e+00
  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.370778388748534e-01
  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  4.920811303936179e-10
  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00 -1.370778380225440e-01
  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.762574219379162e+00

  1.468180054997135e+00  7.712759939050777e-01  7.928499164372629e-08 -7.712758565794439e-01 -1.468179975712145e+00
  6.71999529e-04
```


### Performance Measurement with LIKWID

Create a script that, after executing the `resolveEDO` program, uses the LIKWID tool to display **only** the following information:

- **Arithmetic Operations:** Use the `FLOPS_DP` group from LIKWID. Report the counter `FP_ARITH_INST_RETIRED_SCALAR_DOUBLE` and the `DP` metric (in MFLOP/s).
- These values must be obtained individually for the computation of **each** ODE solution.

---


## EP03: Curve Fitting Optimization

### Objective
Improve and evaluate the performance of the `ajustePol` curve fitting program. The program calculates an N-degree polynomial that fits a curve described by K points.

### Example Program Execution

**Input (stdin):**

N
</br>
K
</br>
x1 y1
</br>
x2 y2
</br>
…
</br>
xK yK
</br>

**Output (stdout):**
```c
a0 a1 a2 … aN
r0 r1 r2 … rK
K tSL tEG
```
The program should be invoked as follows: `./ajustePol < pontos.in > resultado.out`

### Performance Improvement and Analysis

**Objective:** Optimize the original `ajustePol` program (v1) to create a more performant version (v2) without altering its numerical results. Explain all code changes in the `LEIAME` file.

**Performance Analysis:** Compare v1 and v2 performance for:
(A) System of Linear Equations (SL) generation using the Least Squares Method.
(B) SL solution using the Gaussian Elimination Method.

**Conditions:** 
- Compile both versions with GCC -O3 -mavx -march=native on the same machine.
- Use LIKWID for instrumentation on core 3 (-C 3).
- Provide processor architecture details (`LIKWID-topology -g -c`).
- Utilize `long long int` for relevant variables in both versions to handle large K values.

**Tests:** 
- Generate test data using `./gera_entrada <Kpoints> <PolynomialDegree> | ./ajustePol (or with LIKWID)`.
- N values: N1=10, N2=1000.
- K values: 64, 128, 200, 256, 512, 600, 800, 1024, 2000, 3000, 4096, 6000, 7000, 10000, 50000, 10⁵. For N1, also include: 10⁶, 10⁷, 10⁸.
- Graphs: Two sets of line graphs (one for A, one for B). Each graph should have four distinct lines (N1+v1, N1+v2, N2+v1, N2+v2). X-axis (K) and Y-axis (time for time test) must be logarithmic. Explain each graph.

**Metrics to Monitor (using LIKWID):**
- Time: Execution time (`timestamp()`).
- L3 Cache Miss: "cache miss RATIO" from `L3CACHE` group.
- Energy: "Energy [J]" from `ENERGY` group.
- Arithmetic Operations: "FLOPS DP" and "FLOPS AVX DP" in MFLOP/s from `FLOPS_DP` group. Explain AVX DP results in the `LEIAME`.
