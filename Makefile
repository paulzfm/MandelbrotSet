all: seq static

seq: seq_mandelbrot.c
	gcc seq_mandelbrot.c -o seq -lX11

static: static_mandelbrot.c
	gcc static_mandelbrot.c -o static -lX11 -lpthread

clean:
	rm -rf seq static
