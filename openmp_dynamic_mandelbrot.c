#include <X11/Xlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

/* window size */
#define width 400
#define height 400
#define N 20

/* complex */
typedef struct complexType
{
	double real, imag;
} Compl;

/* pixel buffer */
int pixels[width][height];

int main(void)
{
	Display *display;
	Window window;      // initialization for a window
	int screen;         // which screen

	/* open connection with the server */
	display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "cannot open display\n");
		return 0;
	}

	screen = DefaultScreen(display);

	/* set window position */
	int x = 0;
	int y = 0;

	/* border width in pixels */
	int border_width = 0;

	/* create window */
	window = XCreateSimpleWindow(display, RootWindow(display, screen), x, y, width, height, border_width,
					BlackPixel(display, screen), WhitePixel(display, screen));

	/* create graph */
	GC gc;
	XGCValues values;
	long valuemask = 0;

	gc = XCreateGC(display, window, valuemask, &values);
	XSetForeground (display, gc, BlackPixel (display, screen));
	XSetBackground(display, gc, 0X0000FF00);
	XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

	/* map(show) the window */
	XMapWindow(display, window);
	XSync(display, 0);

	struct timespec start, finish; // time
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	/* draw points */
	int i, j, k;

	omp_set_num_threads(N);
	#pragma omp parallel for schedule(dynamic)
	for (k = 0; k < width * height; k++) {
			Compl z, c;
			int repeats;
			double temp, lengthsq;
			i = k / height;
			j = k % height;

			z.real = 0.0;
			z.imag = 0.0;
			c.real = -2.0 + (double)i * (4.0 / (double)width);
			c.imag = -2.0 + (double)j * (4.0 / (double)height);
			repeats = 0;
			lengthsq = 0.0;

			while (repeats < 100000 && lengthsq < 4.0) {
				/* Theorem : If c belongs to M, then |Zn| <= 2. So Zn^2 <= 4 */
				temp = z.real * z.real - z.imag * z.imag + c.real;
				z.imag = 2 * z.real * z.imag + c.imag;
				z.real = temp;
				lengthsq = z.real * z.real + z.imag * z.imag;
				repeats++;
			}

			pixels[i][j] = 1024 * 1024 * (repeats % 256);
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = finish.tv_sec - start.tv_sec;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Total time: %lfs\n", elapsed);
	printf("Drawing...\n");

	/* drawing */
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			XSetForeground(display, gc, pixels[i][j]);
			XDrawPoint(display, window, gc, i, j);
		}
	}
	XFlush(display);
	sleep(2);

	return 0;
}
