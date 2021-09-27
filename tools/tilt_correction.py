#!/usr/bin/env python3

# copyright (c) Toru Niina 2021
# distributed under the MIT license.

# README
#
# usage: python3 <filename.asd> <frame> [channel=1 or 2, 1 by default]
#
# left-click dragging : select rectangle to use as a background region
# right click         : remove rectangle that is mistakenly chosen
# Enter key           : run fitting. The result will be written "filename_corrected.dat" as space-separated file
#                       "filename_background.dat" will have pixels that are used to fit plane
#
# On ubuntu, you may need to run `sudo apt install python3-tk`.

import libasd
import tkinter
import numpy as np
import matplotlib
import matplotlib.cm
import sys

class TiltCorrection(tkinter.Frame):
    def __init__(self, img, x, y, fname, lname, master=None):
        super().__init__(master)

        self.afmimg = img
        self.afmx   = x
        self.afmy   = y
        self.filename = fname
        self.logname  = lname

        self.pixels_used = []
        self.rectangles  = []

        self.tmprect = None # to show temporary regions selected
        self.colormap = matplotlib.cm.get_cmap("afmhot") # to colorize heights

        # GUI stuff
        self.master = master # upper level widget (in this case, root window)
        self.master.title('afm tilt correction')
        self.pack() # I don't know but this is needed
        self.launch()

        return

    def launch(self):
        print("start launching GUI")

        refsize = 320
        self.magnification = refsize // min(self.afmx, self.afmy)
        mag = self.magnification # alias. "magnification" is too long to type

        self.width  = self.afmx * mag
        self.height = self.afmy * mag

        if mag != 1:
            print("image is too small to manipulate via GUI. magnifying x{}".format(mag))

        # colormap
        self.min_height = np.amin(self.afmimg)
        self.max_height = np.amax(self.afmimg)
        self.height_coef = 1.0 / (self.max_height - self.min_height)
        print("max height in the image = ", self.max_height, ", min height = ", self.min_height)

        # make image
        print("putting image on GUI...", end=" ", flush=True)
        self.canvas = tkinter.Canvas(self, bg='black', width=self.width, height=self.height, highlightthickness=0)
        self.canvas.pack()
        self.draw_image()
        print("done.")

        print("adding widgets to GUI...", end=" ", flush=True)
        self.canvas.bind('<ButtonPress-1>',    self.start_pickup)   # left-click pressed
        self.canvas.bind('<B1-Motion>',        self.during_pickup)  # left-click + drag
        self.canvas.bind('<ButtonRelease-1>',  self.stop_pickup)    # left-click released
        self.canvas.bind('<Button-3>',         self.undo_pickup)    # right click
        self.winfo_toplevel().bind('<Return>', self.run_correction) # keyboard enter key
        print("done.")

        return

    def draw_image(self):
        mag = self.magnification

        # show image
        for j in range(0, self.afmy):
            # change (0, 0) at the bottom-left (tk sets (0, 0) at top-left)
            y1 = self.height - (j+1)*mag
            y2 = self.height -  j   *mag
            for i in range(0, self.afmx):
                pixel_color=self.colorize(self.afmimg[j][i])
                x1 = i     * mag
                x2 = (i+1) * mag
                self.canvas.create_rectangle(x1, y1, x2, y2, fill=pixel_color, outline="")

        # show rectangle regions that are already selected
        for rect in self.rectangles:
            xstart, xend, ystart, yend = rect

            x1 = xstart * mag
            x2 = xend   * mag
            y1 = self.height - ystart * mag
            y2 = self.height - yend   * mag

            self.canvas.create_rectangle(x1, y1, x2, y2, width=3, outline="white")
        return

    def colorize(self, height):
        value = (height - self.min_height) * self.height_coef # adjust in [0, 1]
        return matplotlib.colors.rgb2hex(self.colormap(value)) # return in "ffffff" or something like that

    def undo_pickup(self, event):
        xstart, xend, ystart, yend = self.rectangles.pop(-1)
        print("removing rect({}, {}, {}, {})".format(xstart, xend, ystart, yend))
        self.draw_image()
        return

    def start_pickup(self, event):
        self.is_dragging = True
        self.tmp_start_x = event.x
        self.tmp_start_y = event.y

    def during_pickup(self, event):
        if not self.is_dragging:
            return

        mag = self.magnification

        # remove current tmprect if exists (it is selected in the last frame, so
        # it could be different than the rect currently selected)
        if not self.tmprect is None:
            self.canvas.delete(self.tmprect)

        x1, x2 = (self.tmp_start_x, event.x) if self.tmp_start_x < event.x else (event.x, self.tmp_start_x)
        y1, y2 = (self.tmp_start_y, event.y) if self.tmp_start_y < event.y else (event.y, self.tmp_start_y)

        self.tmprect = self.canvas.create_rectangle(x1, y1, x2, y2, width=3, fill="", outline="white", dash=(4,8))

        return

    def stop_pickup(self, event):
        self.is_dragging = False

        # If there is tmp rectangle, remove it
        if not self.tmprect is None:
            self.canvas.delete(self.tmprect)
            self.tmprect = None

        start_x = min(self.tmp_start_x, event.x)
        stop_x  = max(self.tmp_start_x, event.x)

        # in tkinter, y axis is flipped (ystart uses max, while xstart uses min)
        start_y = max(self.tmp_start_y, event.y)
        stop_y  = min(self.tmp_start_y, event.y)

        xstart = start_x                 // self.magnification
        xend   = stop_x                  // self.magnification + 1
        ystart = (self.height - start_y) // self.magnification
        yend   = (self.height - stop_y ) // self.magnification + 1

        print("appending rect({}, {}, {}, {})".format(xstart, xend, ystart, yend))

        self.rectangles.append((xstart, xend, ystart, yend))
        self.draw_image()
        return

    def fit_plane(self, heights):
        mean_x  = 0
        mean_y  = 0
        mean_z  = 0
        mean_xx = 0
        mean_xy = 0
        mean_xz = 0
        mean_yy = 0
        mean_yz = 0
        for (x, y, z) in heights:
            mean_x  += x
            mean_y  += y
            mean_z  += z
            mean_xx += x * x
            mean_xy += x * y
            mean_xz += x * z
            mean_yy += y * y
            mean_yz += y * z
        mean_x  /= len(heights)
        mean_y  /= len(heights)
        mean_z  /= len(heights)
        mean_xx /= len(heights)
        mean_xy /= len(heights)
        mean_xz /= len(heights)
        mean_yy /= len(heights)
        mean_yz /= len(heights)

        v_xx = mean_xx - mean_x * mean_x
        v_xy = mean_xy - mean_x * mean_y
        v_xz = mean_xz - mean_x * mean_z
        v_yy = mean_yy - mean_y * mean_y
        v_yz = mean_yz - mean_y * mean_z

        a = (v_xz / v_xx - (v_xy * v_yz) / (v_xx * v_yy)) / (1.0 - (v_xy * v_xy) / (v_xx * v_yy))
        b = (v_yz - a * v_xy) / v_yy
        c = mean_z - b * mean_y - a * mean_x

        return (a, b, c)

    def run_correction(self, event):
        print("start running correction")

        print("collecting bg pixels ... ", end="", flush =True)
        collected_xy = []
        bgheights = []
        for rect in self.rectangles:
            xstart, xend, ystart, yend = rect

            # to avoid out-of-range
            x1 = max(0, xstart)
            y1 = max(0, ystart)
            x2 = min(self.afmx, xend)
            y2 = min(self.afmy, yend)

            for j in range(y1, y2):
                for i in range(x1, x2):
                    if (i, j) in collected_xy: # to avoid double-count
                        continue
                    collected_xy.append((i, j))
                    z = self.afmimg[j][i]
                    bgheights.append((i, j, z))
        print("done.")

        print("writing bg pixel xyz to {} ... ".format(self.logname), end="", flush =True)
        with open(self.logname, "w") as f:
            f.write("# x[pixel], y[pixel], z[nm]")
            for bg in bgheights:
                x, y, z = bg
                f.write("{:10.6f} {:10.6f} {:10.6f}\n".format(x, y, z))
        print("done.")

        print("fitting background plane ... ", end = "", flush=True)
        a, b, c = self.fit_plane(bgheights)
        print("done.")

        print("fitted plane: z = {:9.4f}x + {:9.4f}y + {:9.4f}, x and y are in the unit of [pixels]".format(a, b, c))

        # ---------------------------------------------------------------------
        # dump corrected image

        min_corrected =  float("inf")
        max_corrected = -float("inf")
        print("writing corrected image into {} ... ".format(self.filename), end = "", flush=True)
        with open(self.filename, "w") as f:
            for y in range(0, self.afmy):
                line = ""
                for x in range(0, self.afmx):
                    stage = a * x + b * y + c
                    height = self.afmimg[y][x] - stage
                    line += "{:10.5f} ".format(height)
                    if min_corrected > height:
                        min_corrected = height
                    if max_corrected < height:
                        max_corrected = height
                line += "\n"
                f.write(line)
        print("done.")
        height_range_corrected = max_corrected - min_corrected

        # ---------------------------------------------------------------------
        # show corrected window in another window

        mag = self.magnification
        self.tmp_window = tkinter.Toplevel(self.master)
        self.tmp_window.title("corrected image")
        self.tmp_canvas = tkinter.Canvas(self.tmp_window, bg='black', width=self.width, height=self.height, highlightthickness=0)
        self.tmp_canvas.pack()
        for j in range(0, self.afmy):
            # change (0, 0) at the bottom-left (tk sets (0, 0) at top-left)
            y1 = self.height - (j+1)*mag
            y2 = self.height -  j   *mag
            for i in range(0, self.afmx):
                stage  = a * x + b * y + c
                height = self.afmimg[j][i] - stage
                value  = (height - min_corrected) / height_range_corrected
                pixel_color = matplotlib.colors.rgb2hex(self.colormap(value))
                x1 =  i    * mag
                x2 = (i+1) * mag
                self.tmp_canvas.create_rectangle(x1, y1, x2, y2, fill=pixel_color, outline="")

        return

# ------------------------------------------------------------------------------
# main

if len(sys.argv) != 3 and len(sys.argv) != 4:
    print("usage: python3 <filename.asd> <frame> [channel=1 or 2, 1 by default]")
    sys.exit(1)

print("""
TiltCorrection
  left-click dragging : select rectangle to use as a background region
  right click         : remove rectangle that is mistakenly chosen
  Enter key           : run fitting.
                        The resulting image will be written "filename_corrected.dat" as a space-separated file.
                        "filename_background.dat" will have pixels that are used to fit plane (mainly for debugging and reproducibility).
""")

data = libasd.read_asd(sys.argv[1])

x_pixels = data.header.x_pixel
y_pixels = data.header.y_pixel

print("image size = {}x{}".format(x_pixels, y_pixels))

if len(sys.argv) == 4:
    channel = int(sys.argv[3]) - 1 # 1 | 2 -> 0 | 1
    frame   = int(sys.argv[2])
    afmimg = data.channels[channel][frame].image()
    print("reading image at channel {}, {}-th frame".format(channel, frame))
else:
    frame   = int(sys.argv[2])
    afmimg = data.frames[frame].image()
    print("reading image at frame", frame)

filename = sys.argv[1][0:-4] + "_corrected.dat" # for output
logname  = sys.argv[1][0:-4] + "_background.dat" # for output

# run GUI

root = tkinter.Tk()
app = TiltCorrection(afmimg, x_pixels, y_pixels, filename, logname, master=root)
app.mainloop()
