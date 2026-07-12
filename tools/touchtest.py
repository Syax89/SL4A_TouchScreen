#!/usr/bin/env python3
"""Visual touch test — draws touch points on a fullscreen canvas.
Green dot = touch position (0..32767 mapped to screen pixels).
Screen edges shown as red border. Press ESC or close window to quit."""
import struct, sys, os
import tkinter as tk

EVDEV = "/dev/input/event16"
SCREEN_MAX = 32767.0
# Input event struct: timeval(8) + type(2) + code(2) + value(4) = 16 bytes
# Actually it's: tv_sec(8) + tv_usec(8) + type(2) + code(2) + value(4) = 24 on x64
FMT = '@llHHi'  # 24 bytes
EV_KEY, EV_ABS, EV_SYN = 1, 3, 0
BTN_TOUCH, ABS_X, ABS_Y = 330, 0, 1
ABS_MT_POSITION_X, ABS_MT_POSITION_Y = 53, 54

class TouchTest:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Touch Test")
        self.root.attributes('-fullscreen', True)
        self.root.configure(bg='black')

        self.w = self.root.winfo_screenwidth()
        self.h = self.root.winfo_screenheight()
        print(f"Screen: {self.w}x{self.h}")
        print("Touch anywhere. Close window or press ESC to quit.")

        self.canvas = tk.Canvas(self.root, width=self.w, height=self.h,
                                bg='black', highlightthickness=0)
        self.canvas.pack()

        # Draw corner markers
        r = 20
        for cx, cy in [(r,r), (self.w-r,r), (self.w-r,self.h-r), (r,self.h-r)]:
            self.canvas.create_oval(cx-r, cy-r, cx+r, cy+r,
                                     outline='red', width=2)

        # Crosshair at center
        self.canvas.create_line(self.w//2 - 50, self.h//2, self.w//2 + 50, self.h//2, fill='#333')
        self.canvas.create_line(self.w//2, self.h//2 - 50, self.w//2, self.h//2 + 50, fill='#333')

        self.touch_dot = self.canvas.create_oval(0, 0, 1, 1, fill='green', outline='')
        self.coord_text = self.canvas.create_text(10, 10, anchor='nw',
                          fill='white', font=('monospace', 14), text='No touch')

        self.touching = False
        self.x = self.y = 0
        self.root.bind('<Escape>', lambda e: self.root.destroy())
        self.root.after(16, self.poll_evdev)

    def poll_evdev(self):
        try:
            data = os.read(self.fd, 1024)
            for i in range(0, len(data), 24):
                chunk = data[i:i+24]
                if len(chunk) < 24:
                    break
                tv_sec, tv_usec, etype, ecode, evalue = struct.unpack(FMT, chunk)
                if etype == EV_KEY and ecode == BTN_TOUCH:
                    self.touching = (evalue == 1)
                if etype == EV_ABS:
                    if ecode == ABS_MT_POSITION_X:
                        self.x = evalue
                    elif ecode == ABS_MT_POSITION_Y:
                        self.y = evalue
                if etype == EV_SYN and ecode == 0 and self.touching:
                    self.draw()
        except BlockingIOError:
            pass
        self.root.after(16, self.poll_evdev)

    def draw(self):
        px = int(self.x / SCREEN_MAX * self.w)
        py = int(self.y / SCREEN_MAX * self.h)
        r = 15
        self.canvas.coords(self.touch_dot, px-r, py-r, px+r, py+r)
        xpct = self.x / SCREEN_MAX * 100
        ypct = self.y / SCREEN_MAX * 100
        self.canvas.itemconfig(self.coord_text,
            text=f"Raw:({self.x},{self.y}) Pix:({px},{py}) Pct:({xpct:.1f}%,{ypct:.1f}%)")

    def run(self):
        self.fd = os.open(EVDEV, os.O_RDONLY | os.O_NONBLOCK)
        self.root.mainloop()
        os.close(self.fd)

if __name__ == '__main__':
    if len(sys.argv) > 1:
        EVDEV = sys.argv[1]
    TouchTest().run()
