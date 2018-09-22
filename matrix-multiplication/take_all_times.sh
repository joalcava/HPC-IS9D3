#!/bin/sh

echo 'Compiling serial matrix multiplication...'
g++ -std=c++11 serial_mm.cpp -o serial_mm.out
rm serial_mm.txt
echo 'running serial mm...'
echo 'running 10x10 mm'
./serial_mm.out 10 >> serial_mm.txt
echo 'done.'
echo 'running 100x100 mm'
./serial_mm.out 100 >> serial_mm.txt
echo 'done.'
echo 'running 250x250 mm'
./serial_mm.out 250 >> serial_mm.txt
echo 'done.'
echo 'running 500x500 mm'
./serial_mm.out 500 >> serial_mm.txt
echo 'done.'
echo 'running 750x750 mm'
./serial_mm.out 750 >> serial_mm.txt
echo 'done.'
echo 'running 1000x1000 mm'
./serial_mm.out 1000 >> serial_mm.txt
echo 'done.\n\n'

echo 'Compiling omp paraller matrix multiplication...'
g++ -std=c++11 -fopenmp omp_mm.cpp -o omp_mm.out
rm omp_mm.txt
echo 'running opm parallel mm...'
echo 'running 10x10 mm'
./omp_mm.out 10 >> omp_mm.txt
echo 'done.'
echo 'running 100x100 mm'
./omp_mm.out 100 >> omp_mm.txt
echo 'done.'
echo 'running 250x250 mm'
./omp_mm.out 250 >> omp_mm.txt
echo 'done.'
echo 'running 500x500 mm'
./omp_mm.out 500 >> omp_mm.txt
echo 'done.'
echo 'running 750x750 mm'
./omp_mm.out 750 >> omp_mm.txt
echo 'done.'
echo 'running 1000x1000 mm'
./omp_mm.out 1000 >> omp_mm.txt
echo 'done.\n\n'


echo 'Compiling thread parallel multiplication...'
g++ -std=c++11 -pthread thread_mm.cpp -o thread_mm.out
rm thread_mm.txt
echo 'running thread mm...'
echo 'running 10x10 mm'
./thread_mm.out 10 10 >> thread_mm.txt
echo 'done.'
echo 'running 100x100 mm'
./thread_mm.out 100 10 >> thread_mm.txt
echo 'done.'
echo 'running 250x250 mm'
./thread_mm.out 250 10 >> thread_mm.txt
echo 'done.'
echo 'running 500x500 mm'
./thread_mm.out 500 10 >> thread_mm.txt
echo 'done.'
echo 'running 750x750 mm'
./thread_mm.out 750 10 >> thread_mm.txt
echo 'done.'
echo 'running 1000x1000 mm'
./thread_mm.out 1000 10 >> thread_mm.txt
echo 'done.\n\n'

echo 'Compiling process parallel multiplication...'
g++ -std=c++11 process_mm.cpp -o process_mm.out
rm process_mm.txt
echo 'running process mm...'
echo 'running 10x10 mm'
./process_mm.out 10 10 >> process_mm.txt
echo 'done.'
echo 'running 100x100 mm'
./process_mm.out 100 10 >> process_mm.txt
echo 'done.'
echo 'running 250x250 mm'
./process_mm.out 250 10 >> process_mm.txt
echo 'done.'
echo 'running 500x500 mm'
./process_mm.out 500 10>> process_mm.txt
echo 'done.'
echo 'running 750x750 mm'
./process_mm.out 750 10 >> process_mm.txt
echo 'done.'
echo 'running 1000x1000 mm'
./process_mm.out 1000 10 >> process_mm.txt
echo 'done.\n\n'
