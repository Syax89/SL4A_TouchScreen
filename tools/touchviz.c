/*
 * touchviz.c — Visual touch position display using SDL2
 *
 * Fullscreen window. Green circle = touch position.
 * Red corners = screen edges. Press ESC to quit.
 *
 * Build: cc -Wall -o touchviz touchviz.c $(pkg-config --cflags --libs sdl2)
 * Usage: sudo ./touchviz [/dev/input/event16]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <SDL2/SDL.h>

#define EVDEV "/dev/input/event16"
#define SCREEN_MAX 32767.0

static int evdev = -1;
static int touching;
static int raw_x, raw_y;

static void draw_crosshair(SDL_Renderer *r, int x, int y, int size, Uint8 c)
{
	SDL_SetRenderDrawColor(r, c, c, c, 255);
	SDL_RenderDrawLine(r, x - size, y, x + size, y);
	SDL_RenderDrawLine(r, x, y - size, x, y + size);
}

int main(int argc, char **argv)
{
	const char *path = EVDEV;
	int w, h, done = 0;
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Event e;

	if (argc > 1) path = argv[1];

	evdev = open(path, O_RDONLY | O_NONBLOCK);
	if (evdev < 0) { perror(path); return 1; }

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	win = SDL_CreateWindow("Touch Viz",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!win) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		return 1;
	}

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
	SDL_GetWindowSize(win, &w, &h);
	printf("Screen: %dx%d\n", w, h);

	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

	while (!done) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN &&
			    e.key.keysym.sym == SDLK_ESCAPE))
				done = 1;
		}

		/* Read evdev events */
		struct input_event ev;
		while (read(evdev, &ev, sizeof(ev)) == sizeof(ev)) {
			if (ev.type == EV_KEY && ev.code == BTN_TOUCH)
				touching = ev.value;
			if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_X)
				raw_x = ev.value;
			if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_Y)
				raw_y = ev.value;
		}

		/* Draw */
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		/* Corner markers */
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		int r = 15;
		SDL_Rect cr = {0, 0, r*2, r*2};
		cr.x = 0; cr.y = 0; SDL_RenderDrawRect(rend, &cr);
		cr.x = w - r*2; SDL_RenderDrawRect(rend, &cr);
		cr.y = h - r*2; SDL_RenderDrawRect(rend, &cr);
		cr.x = 0; SDL_RenderDrawRect(rend, &cr);

		/* Center crosshair */
		draw_crosshair(rend, w/2, h/2, 40, 60);

		/* Touch point */
		if (touching && raw_x > 0 && raw_y > 0) {
			int px = (int)(raw_x / SCREEN_MAX * w);
			int py = (int)(raw_y / SCREEN_MAX * h);

			/* Filled green circle */
			SDL_SetRenderDrawColor(rend, 0, 255, 0, 200);
			for (int dy = -20; dy <= 20; dy++)
				for (int dx = -20; dx <= 20; dx++)
					if (dx*dx + dy*dy <= 400)
						SDL_RenderDrawPoint(rend, px+dx, py+dy);

			/* Crosshair at touch point */
			draw_crosshair(rend, px, py, 25, 255);

			/* Coordinate text in title bar */
			char title[128];
			snprintf(title, sizeof(title),
				"Touch: raw=(%d,%d) px=(%d,%d) pct=(%.1f%%,%.1f%%)",
				raw_x, raw_y, px, py,
				raw_x/SCREEN_MAX*100, raw_y/SCREEN_MAX*100);
			SDL_SetWindowTitle(win, title);
		}

		SDL_RenderPresent(rend);
		SDL_Delay(16);
	}

	close(evdev);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
