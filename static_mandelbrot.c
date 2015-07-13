#include <X11/Xlib.h>
#include <pthread.h>
#include <stdio.h>

/* window size */
#define width 400
#define height 400
#define N 20 // num of threads

/* complex */
typedef struct complexType
{
	double real, imag;
} Compl;

/* pixel buffer */
int pixels[width][height];

/* draw pixel at (i, j) */
void SetPixel(int i, int j)
{
	Compl z, c;
	int repeats;
	double temp, lengthsq;

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

/* draw pixels of a few columns */
typedef struct argsType
{
	int start;
	int end;
} Args;

void* DrawColumns(void* args)
{
	Args* a = (Args*)args;
	int i, j;

	printf("Computing columns [%d, %d)...\n", a->start, a->end);

	for (i = a->start; i < a->end; i++) {
		for (j = 0; j < height; j++) {
			SetPixel(i, j);
		}
	}

	pthread_exit(NULL);
}

int main(void)
{
	/* open connection with the server */
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "Error: cannot open display.\n");
		return 0;
	}

	int screen = DefaultScreen(display);

	/* set window position */
	int x = 0;
	int y = 0;

	/* border width in pixels */
	int border_width = 0;

	/* create window */
	int window = XCreateSimpleWindow(display, RootWindow(display, screen), x, y, width, height, border_width,
					BlackPixel(display, screen), WhitePixel(display, screen));

	/* create graph */
	XGCValues values;
	long valuemask = 0;

	GC gc = XCreateGC(display, window, valuemask, &values);
	XSetForeground(display, gc, BlackPixel(display, screen));
	XSetBackground(display, gc, 0X0000FF00);
	XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);

	/* map(show) the window */
	XMapWindow(display, window);
	XSync(display, 0);

	/* create threads to draw pixels */
	pthread_t threads[N];
	Args a[N];
	int i, j;
	for (i = 0; i < N; i++) {
		a[i].start = i * (width / N);
		a[i].end = (i + 1) * (width / N);
		pthread_create(&threads[i], NULL, DrawColumns, (void*)(&a[i]));
	}

	/* join all threads */
	for (i = 0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("All done.\n");

	/* drawing */
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			XSetForeground(display, gc, pixels[i][j]);
			XDrawPoint(display, window, gc, i, j);
		}
	}
	XFlush(display);

	return 0;
}
