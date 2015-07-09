#include <X11/Xlib.h>
#include <pthread.h>
#include <stdio.h>

/* X Window */
Display *display;
Window window;
GC gc;

/* window size */
const int width = 400;
const int height = 400;

/* complex */
typedef struct complexType
{
	double real, imag;
} Compl;

/* mutex */
pthread_mutex_t mutex;

/* draw pixel at (i, j) */
void DrawPixel(int i, int j)
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

	pthread_mutex_lock(&mutex);
	XSetForeground(display, gc, 1024 * 1024 * (repeats % 256));
	XDrawPoint(display, window, gc, i, j);
	pthread_mutex_unlock(&mutex);
}

/* draw pixels of a few column */
typedef struct argsType
{
	int start;
	int end;
} Args;

void* DrawColumns(void* args)
{
	Args* a = (Args*)args;
	int i, j;

	printf("Computing columns [%d, %d]\n", a->start, a->end);

	for (i = a->start; i < a->end; i++) {
		for (j = 0; j < height; j++) {
			DrawPixel(i, j);
		}
	}

	pthread_mutex_lock(&mutex);
	XFlush(display);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main(void)
{
	/* open connection with the server */
	display = XOpenDisplay(NULL);
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
	window = XCreateSimpleWindow(display, RootWindow(display, screen), x, y, width, height, border_width,
					BlackPixel(display, screen), WhitePixel(display, screen));

	/* create graph */
	XGCValues values;
	long valuemask = 0;

	gc = XCreateGC(display, window, valuemask, &values);
	//XSetBackground (display, gc, WhitePixel (display, screen));
	XSetForeground(display, gc, BlackPixel(display, screen));
	XSetBackground(display, gc, 0X0000FF00);
	XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);

	/* map(show) the window */
	XMapWindow(display, window);
	XSync(display, 0);

	/* initialize mutex */
	pthread_mutex_init(&mutex, NULL);

	/* create threads to draw pixels */
	pthread_t threads[width / 20];
	Args a[width / 20];
	int i;
	for (i = 0; i < width / 20; i++) {
		a[i].start = i * 20;
		a[i].end = (i + 1) * 20;
		pthread_create(&threads[i], NULL, DrawColumns, (void*)(&a[i]));

		printf("Create thread #%d\n", i);
	}

	/* join all threads */
	for (i = 0; i < width / 20; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
