#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

static const char *corner_names[] = {"top-left", "top-right", "bottom-right", "bottom-left"};

struct point { int x, y; };

int main(int argc, char **argv)
{
	const char *devpath = "/dev/input/event16";
	struct point samples[4][10];
	int nsamples[4] = {0,0,0,0};
	int fd, i;

	if (argc > 1) devpath = argv[1];

	fd = open(devpath, O_RDONLY);
	if (fd < 0) { perror(devpath); return 1; }

	for (i = 0; i < 4; i++) {
		struct input_event ev;
		int touching = 0, x = 0, y = 0;
		int frame = 0;

		printf("\nTouch and HOLD the %s corner...\n", corner_names[i]);
		printf("(keep holding for stable samples, then release)\n");
		fflush(stdout);

		while (1) {
			if (read(fd, &ev, sizeof(ev)) != sizeof(ev))
				continue;
			if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
				if (ev.value == 1 && !touching) {
					touching = 1;
					frame = 0;
				} else if (ev.value == 0 && touching) {
					touching = 0;
					if (nsamples[i] > 0)
						goto next_corner;
				}
			}
			if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_X) x = ev.value;
			if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_Y) y = ev.value;
			if (ev.type == EV_SYN && ev.code == SYN_REPORT && touching) {
				frame++;
				/* Skip first 5 frames (debounce/EMA settling) */
				if (frame > 5 && nsamples[i] < 10 && x > 0 && y > 0) {
					samples[i][nsamples[i]].x = x;
					samples[i][nsamples[i]].y = y;
					nsamples[i]++;
				}
			}
		}
next_corner: ;
	}

	close(fd);

	printf("\n=== Collected samples ===\n");
	for (i = 0; i < 4; i++) {
		int j, sumx = 0, sumy = 0;
		printf("  %-12s: ", corner_names[i]);
		for (j = 0; j < nsamples[i]; j++) {
			printf("(%d,%d) ", samples[i][j].x, samples[i][j].y);
			sumx += samples[i][j].x;
			sumy += samples[i][j].y;
		}
		if (nsamples[i] > 0)
			printf("-> avg(%d,%d)", sumx/nsamples[i], sumy/nsamples[i]);
		printf("\n");
	}

	return 0;
}
