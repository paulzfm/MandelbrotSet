# Mandelbrot Set

## Install

Simply type

    $ make

and the four programs `seq`, `static`, `dynamic` and `mp` are generated.

## Usage

- `seq` Sequential version.
- `static` Static muitl-thread version.
- `dynamic` Dynamic muitl-thread version using task queue.
- `mp` Openmp version.

## Results

For more accuracy, we only measure the time of computation and save data into buffer. Finally, we flush them all together. Note that we create 20 threads for static and dynamic version.

Here is the result executing on our server:

Version  | sequential | static | dynamic | openmp |
-------  | ---------- | ------ | ------- | ------ |
Time (s) | 13.920745  | 3.171405 | 1.155419 | 3.192544 |
