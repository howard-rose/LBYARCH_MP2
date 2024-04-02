# LBYARCH_MP2

This x86-to-C interface programming project compares the performance of executing a specified function using a C kernel vs using an x86-64 kernel called from C. The given function is DAXPY (A*X + Y).

To run the program, simply build and run the solution in Visual Studio. Given a default vector size _n_ and randomized inputs _A_, _X_, and _Y_, the program will run the *daxpy_c* and *daxpy_asm* functions and show the results and execution times of each function. To change the value of _n_ across different runs, the first line of *int main()* in *main.c* may be set to another macro or a user defined value. 

The number of times the functions were run is defined by the *RUNS* macro, which is set to 30. The average execution times of the C kernel and the x86-64 kernel across 30 runs for each defined vector size n = {2<sup>20</sup>, 2<sup>24</sup>, 2<sup>28</sup>} are shown in the table below. 

| n | C ave. runtime (s) | x86-64 ave. runtime (s) |  % difference |
| --- | --- | --- | --- |
| 2<sup>20</sup> | 0.002107 | 0.002546 | |
| 2<sup>24</sup> | | | |
| 2<sup>28</sup> | | | |

Below is the program output of a sample run, including the correctness check for whether the outputs of the C kernel and the x86-64 kernel are equal. To speed up the execution time of the program, only the first 10 values of each result were compared. However, this may be modified to compare all values by changing the value of *10* in line 68 of *main.c* to *n*.

![image](https://github.com/howard-rose/LBYARCH_MP2/assets/25128643/6c5e58dd-6f49-4e3b-9a37-4862f62f8610)
