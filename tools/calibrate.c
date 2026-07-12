/*
 * calibrate.c — Touchscreen grid→screen calibration
 *
 * Sets calib_scale_*=1000 so screen coords == grid coords.
 * Captures 2 seconds of touches per corner, averages them.
 *
 * Usage: sudo ./calibrate [device]
 * Touch each corner: top-left, top-right, bottom-right, bottom-left.
 * Hold ~1 sec each. Press Enter after each corner.
 * Build: cc -Wall -o calibrate calibrate.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <linux/input.h>

#define SCREEN_MAX 32767

static const char *corner_names[] = {
	"top-left", "top-right", "bottom-right", "bottom-left"
};
static const int corner_ideal[4][2] = {
	{0, 0}, {SCREEN_MAX, 0}, {SCREEN_MAX, SCREEN_MAX}, {0, SCREEN_MAX}
};

static void set_param(const char *param, int val)
{
	char cmd[128];
	snprintf(cmd, sizeof(cmd), "echo %d > /sys/module/spi_hid/parameters/%s", val, param);
	system(cmd);
}

int main(int argc, char **argv)
{
	const char *devpath = "/dev/input/event16";
	int grid_x[4] = {0}, grid_y[4] = {0};
	int fd, i;

	if (argc > 1) devpath = argv[1];

	set_param("calib_scale_x", 1000);
	set_param("calib_scale_y", 1000);
	set_param("calib_offset_x", 0);
	set_param("calib_offset_y", 0);

	fd = open(devpath, O_RDONLY);
	if (fd < 0) { perror(devpath); return 1; }

	printf("=== Touchscreen Grid → Screen Calibration ===\n");
	printf("(scale=identity: evdev values are grid coords)\n");
	printf("Touch and HOLD each corner for best results.\n\n");

	/* Drain any buffered events */
	{
		struct input_event ev;
		for (int n = 0; n < 32; n++)
			read(fd, &ev, sizeof(ev));
	}

	for (i = 0; i < 4; i++) {
		struct input_event ev;
		struct timeval start, now;
		int touching = 0, x = 0, y = 0;
		int frame = 0, samples = 0;
		long sum_x = 0, sum_y = 0;

		printf("Touch and HOLD %s corner...", corner_names[i]);
		fflush(stdout);
		gettimeofday(&start, NULL);

		/* Sample for up to 3 seconds after first touch */
		while (1) {
			if (read(fd, &ev, sizeof(ev)) != sizeof(ev))
				continue;

			gettimeofday(&now, NULL);
			double elapsed = (now.tv_sec - start.tv_sec) +
					 (now.tv_usec - start.tv_usec) / 1000000.0;

			if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
				if (ev.value == 1 && !touching) {
					touching = 1;
					frame = 0;
				} else if (ev.value == 0) {
					touching = 0;
				}
			}
			if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_X)
				x = ev.value;
			if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_Y)
				y = ev.value;

			/* Take one sample per SYN_REPORT while touching,
			 * reject grid coords of zero (no valid blob). */
			if (ev.type == EV_SYN && ev.code == SYN_REPORT) {
				frame++;
				if (touching && x > 0 && y > 0 &&
				    samples < 30 && frame > 3) {
					sum_x += x; sum_y += y;
					samples++;
				}
				/* Advance after 2 sec of data or 3 sec total */
				if ((samples >= 5 && elapsed > 1.5) ||
				    elapsed > 3.0)
					break;
			}
		}

		if (samples == 0) {
			printf(" NO TOUCH DETECTED\n");
			close(fd);
			return 1;
		}
		grid_x[i] = (int)(sum_x / samples);
		grid_y[i] = (int)(sum_y / samples);
		printf(" grid=(%d,%d) [%d samples]\n", grid_x[i], grid_y[i], samples);
	}

	close(fd);

	/* Restore DLL calibration */
	set_param("calib_scale_x", 0);
	set_param("calib_scale_y", 2730780);
	set_param("calib_offset_x", 0);
	set_param("calib_offset_y", 0);

	printf("\n=== Calibration Results ===\n");
	printf("%-12s %8s %8s %8s %8s\n", "Corner", "IdealX", "GridX", "IdealY", "GridY");
	for (i = 0; i < 4; i++)
		printf("%-12s %8d %8d %8d %8d\n",
		       corner_names[i], corner_ideal[i][0], grid_x[i],
		       corner_ideal[i][1], grid_y[i]);

	/* Least-squares linear regression */
	{
		double sgx = 0, ssx = 0, sgx2 = 0, sgxsx = 0;
		double sgy = 0, ssy = 0, sgy2 = 0, sgysy = 0;
		double scale_x, scale_y, off_x, off_y;

		for (i = 0; i < 4; i++) {
			sgx += grid_x[i];  ssx += corner_ideal[i][0];
			sgy += grid_y[i];  ssy += corner_ideal[i][1];
			sgx2 += (double)grid_x[i] * grid_x[i];
			sgy2 += (double)grid_y[i] * grid_y[i];
			sgxsx += (double)grid_x[i] * corner_ideal[i][0];
			sgysy += (double)grid_y[i] * corner_ideal[i][1];
		}

		scale_x = (4*sgxsx - sgx*ssx) / (4*sgx2 - sgx*sgx);
		scale_y = (4*sgysy - sgy*ssy) / (4*sgy2 - sgy*sgy);
		off_x = (ssx - scale_x*sgx) / 4;
		off_y = (ssy - scale_y*sgy) / 4;

		printf("\nFit:  screen = grid * scale + offset\n");
		printf("  X: %.6f * grid_x + %.1f\n", scale_x, off_x);
		printf("  Y: %.6f * grid_y + %.1f\n", scale_y, off_y);

		/* Residual error */
		printf("\nResiduals:\n");
		for (i = 0; i < 4; i++) {
			double px = scale_x * grid_x[i] + off_x;
			double py = scale_y * grid_y[i] + off_y;
			printf("  %-12s: err=(%+.1f, %+.1f)\n",
			       corner_names[i],
			       px - corner_ideal[i][0],
			       py - corner_ideal[i][1]);
		}

		printf("\nModule params (apply via insmod/sysfs):\n");
		printf("  calib_scale_x=%lld\n", (long long)(scale_x * 1000 + 0.5));
		printf("  calib_scale_y=%lld\n", (long long)(scale_y * 1000 + 0.5));
		printf("  calib_offset_x=%lld\n", (long long)(off_x + 0.5));
		printf("  calib_offset_y=%lld\n", (long long)(off_y + 0.5));
	}

	return 0;
}
