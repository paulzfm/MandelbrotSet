all: seq static dynamic

seq: seq_mandelbrot.c
	gcc seq_mandelbrot.c -o seq -lX11

static: static_mandelbrot.c
	gcc static_mandelbrot.c -o static -lX11 -lpthread

dynamic: dynamic_mandelbrot.c
	gcc dynamic_mandelbrot.c -o dynamic -lX11 -lpthread

mp: openmp_mandelbrot.c
	gcc openmp_mandelbrot.c -o mp -lX11 -fopenmp

clean:
	rm -rf seq static mp
