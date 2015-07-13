all: seq static dynamic mp_static mp_dynamic

seq: seq_mandelbrot.c
	gcc seq_mandelbrot.c -o seq -lX11 -lrt

static: static_mandelbrot.c
	gcc static_mandelbrot.c -o static -lX11 -lpthread -lrt

dynamic: dynamic_mandelbrot.c
	gcc dynamic_mandelbrot.c -o dynamic -lX11 -lpthread -lrt

mp_static: openmp_mandelbrot.c
	gcc openmp_mandelbrot.c -o mp_static -lX11 -fopenmp -lrt

mp_dynamic: openmp_dynamic_mandelbrot.c
	gcc openmp_dynamic_mandelbrot.c -o mp_dynamic -lX11 -fopenmp -lrt

clean:
	rm -rf seq static dynamic mp_static mp_dynamic
