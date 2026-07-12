/*
 * touchtest.c — Visual touch position display
 *
 * Reads touch events and prints position as % of screen (0..100%).
 * Also maps to pixel coordinates (2256x1504 for Surface Laptop 4).
 *
 * Build: cc -Wall -o touchtest touchtest.c
 * Usage: sudo ./touchtest [/dev/input/event16]
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define SCREEN_MAX  32767.0
#define SCR_W       2256
#define SCR_H       1504

int main(int argc, char **argv)
{
	const char *devpath = "/dev/input/event16";
	int fd, touching = 0, x = 0, y = 0;

	if (argc > 1) devpath = argv[1];

	fd = open(devpath, O_RDONLY);
	if (fd < 0) { perror(devpath); return 1; }

	printf("Touch test — Surface Laptop 4 (%dx%d)\n", SCR_W, SCR_H);
	printf("%-6s %6s %6s %5s %5s %6s %6s\n",
	       "State", "RawX", "RawY", "X%", "Y%", "PixX", "PixY");
	printf("───────────────────────────────────────────────────\n");

	while (1) {
		struct input_event ev;
		if (read(fd, &ev, sizeof(ev)) != sizeof(ev))
			continue;

		if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
			touching = ev.value;
			if (!touching) {
				/* Show release position */
				double xp = 100.0 * x / SCREEN_MAX;
				double yp = 100.0 * y / SCREEN_MAX;
				int px = (int)(xp / 100.0 * SCR_W);
				int py = (int)(yp / 100.0 * SCR_H);
				printf("LIFT   %6d %6d %4.1f %4.1f %6d %6d\n",
				       x, y, xp, yp, px, py);
			}
		}
		if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_X)
			x = ev.value;
		if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_Y)
			y = ev.value;

		if (ev.type == EV_SYN && ev.code == SYN_REPORT && touching
		    && x > 0 && y > 0) {
			double xp = 100.0 * x / SCREEN_MAX;
			double yp = 100.0 * y / SCREEN_MAX;
			int px = (int)(xp / 100.0 * SCR_W);
			int py = (int)(yp / 100.0 * SCR_H);

			/* ASCII visual bar */
			char barx[52], bary[52];
			int bx = (int)(xp / 2.0);  /* 0..50 */
			int by = (int)(yp / 2.0);
			for (int i = 0; i <= 50; i++) {
				barx[i] = (i == bx) ? '█' : (i == 0 || i == 50) ? '│' : ' ';
				bary[i] = (i == by) ? '█' : (i == 0 || i == 50) ? '│' : ' ';
			}
			barx[51] = 0; bary[51] = 0;

			printf("TOUCH  %6d %6d %4.1f %4.1f %6d %6d | %s | %s\n",
			       x, y, xp, yp, px, py, barx, bary);
		}
	}

	close(fd);
	return 0;
}
