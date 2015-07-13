# Mandelbrot Set

## Install

Simply type

    $ make

and the four programs `seq`, `static`, `dynamic` and `mp` are generated.

Note that this project is ONLY available for Unix-like OS.

## Usage

- `./seq` Sequential version.
- `./static` Static muitl-thread version.
- `./dynamic` Dynamic muitl-thread version using task queue.
- `./mp_static` Openmp static version.
- `./mp_dynamic` Openmp dynamic version.

## Results

For more accuracy, we only measure the time of computation and save data into buffer. Finally, we flush them all together. Note that we create 20 threads for all muitl-thread versions.

Here is the result executing on our server:

Version    | Time (s)  |
---------- | --------- |
sequential | 13.920745 |
static     | 3.171405  |
dynamic    | 1.155419  |
openmp static | 3.192544 |
openmp dynamic | 0.917282 |
