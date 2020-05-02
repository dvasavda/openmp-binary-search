# Parallel Binary Search with OpenMP
Binary Search implemented in C using OpenMP


How to run program:
```
cd openmp-binary-search

python3 integer_generator.py

clang -Xpreprocessor -fopenmp -lomp binarySearch_openmp.c -o binarySearch_openmp

./binarySearch_openmp
```

`Integer_generator.py` will generate 200,000,000 integers in ascending order into `input1.txt`

`binarySearch_openmp.c` will perform serial and OpenMP parallellized binary search
