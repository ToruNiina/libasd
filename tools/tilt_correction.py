#!/usr/bin/env python3

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
import tkinter.ttk
import numpy as np
import matplotlib
import matplotlib.cm
import matplotlib.pyplot as plt
import sys

class TiltCorrection(tkinter.Frame):
    def __init__(self, img, x, y, fname, lname, master=None):
        super().__init__(master)

        self.afmimg = img
        self.afmx   = x
        self.afmy   = y
        self.filename = fname
        self.logname = lname

        self.pixels_used = []
        self.rectangles = []

        self.master = master
        self.master.title('afm tilt correction')
        self.pack()
        self.launch(img, x, y)

        return

    def launch(self, img, x, y):
        refsize = 320
        self.magnification = refsize // min(x, y)
        mag = self.magnification # alias

        if mag != 1:
            print("image is too small to manipulate via GUI. magnifying x", mag)

        self.width  = x * mag
        self.height = y * mag

        # colormap
        self.min_height = np.amin(img)
        self.max_height = np.amax(img)
        self.height_coef = 1.0 / (self.max_height - self.min_height)
        print("max height in the image = ", self.max_height, ", min height = ", self.min_height)

        # make image
        print("colorizing and magnifying image ...", end=" ", flush=True)
        self.image = tkinter.PhotoImage(width=self.width, height=self.height)
        self.draw_image()
        print("done.")

        print("putting image on GUI...", end=" ", flush=True)
        self.canvas = tkinter.Canvas(self, bg='black', width=self.width, height=self.height, highlightthickness=0)
        self.canvas.pack()
        self.canvas.create_image(self.width, self.height, image=self.image, anchor="se")
        print("done.")

        print("adding widgets to GUI...", end=" ", flush=True)
        self.canvas.bind('<ButtonPress-1>',    self.start_pickup)
        self.canvas.bind('<B1-Motion>',        self.during_pickup)
        self.canvas.bind('<ButtonRelease-1>',  self.stop_pickup)
        self.canvas.bind('<Button-3>',         self.undo_pickup)
        self.winfo_toplevel().bind('<Return>', self.run_correction)
        print("done.")

        return

    # change (0, 0) at the bottom-left
    def convert_coord(self, x, y):
        return (x, self.height - y)

    def draw_image(self):
        mag = self.magnification
        # show image
        for j in range(0, self.afmy):
            for i in range(0, self.afmx):
                pixel_color=self.colorize(self.afmimg[j][i])
                for h in range(j * mag, (j+1) * mag):
                    for w in range(i * mag, (i+1) * mag):
                        self.image.put(pixel_color, to=self.convert_coord(w, h))

        # show shade to the selected regions
        for rect in self.rectangles:
            xstart, xend, ystart, yend = rect
            for h in range(ystart * mag, yend * mag):
                for w in range(xstart * mag, xend * mag):
                    w_, h_ = self.convert_coord(w, h)
                    r, g, b = self.image.get(w_, h_)
                    r = (r + 255) // 2
                    g = (g + 255) // 2
                    b = (b + 255) // 2
                    self.image.put("#" + format(r, "02x") + format(g, "02x") + format(b, "02x"), to=(w_, h_))
        return

    def colorize(self, height):
        value = (height - self.min_height) * self.height_coef # [0, 1]
        afmhot = matplotlib.cm.get_cmap('afmhot')
        return matplotlib.colors.rgb2hex(afmhot(value))

    def undo_pickup(self, event):
        xstart, xend, ystart, yend = self.rectangles.pop(-1)
        print("removing rect(", xstart, ", ", xend, ", ", ystart, ", ", yend, ")")
        self.draw_image()
        return

    def start_pickup(self, event):
        self.is_dragging = True
        self.tmp_start_x = event.x
        self.tmp_start_y = event.y

    def during_pickup(self, event):
        if not self.is_dragging:
            return

        tmp_stop_x = event.x
        tmp_stop_y = event.y
        self.draw_image()

        # show tmp rect
        mag = self.magnification

        start_x = min(self.tmp_start_x, event.x)
        stop_x  = max(self.tmp_start_x, event.x)
        start_y = max(self.tmp_start_y, event.y)
        stop_y  = min(self.tmp_start_y, event.y)
        xstart = start_x                 // self.magnification
        xend   = stop_x                  // self.magnification
        ystart = (self.height - start_y) // self.magnification
        yend   = (self.height - stop_y ) // self.magnification

        for h in range(ystart * mag, yend * mag):
            for w in range(xstart * mag, xend * mag):
                w_, h_ = self.convert_coord(w, h)
                r, g, b = self.image.get(w_, h_)
                r = (r + 255) // 2
                g = (g + 255) // 2
                b = (b + 255) // 2
                self.image.put("#" + format(r, "02x") + format(g, "02x") + format(b, "02x"), to=(w_, h_))
        return

    def stop_pickup(self, event):
        self.is_dragging = False

        start_x = min(self.tmp_start_x, event.x)
        stop_x  = max(self.tmp_start_x, event.x)

        # in tkinter, y axis is flipped
        start_y = max(self.tmp_start_y, event.y)
        stop_y  = min(self.tmp_start_y, event.y)

        xstart = start_x                 // self.magnification
        xend   = stop_x                  // self.magnification + 1
        ystart = (self.height - start_y) // self.magnification
        yend   = (self.height - stop_y ) // self.magnification + 1

        print("appending rect(", xstart, ", ", xend, ", ", ystart, ", ", yend, ")")

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
        print("running correction ... ")

        print("collecting bg pixels ... ")
        bgheights = []
        for rect in self.rectangles:
            xstart, xend, ystart, yend = rect
            for j in range(ystart, yend):
                for i in range(xstart, xend):
                    z = self.afmimg[j][i]
                    bgheights.append((i, j, z))

        with open(self.logname, "w") as f:
            f.write("# x[pixel], y[pixel], z[nm]")
            for bg in bgheights:
                x, y, z = bg
                f.write("{:10.6f} {:10.6f} {:10.6f}\n".format(x, y, z))

        print("fitting background plane ... ")

        a, b, c = self.fit_plane(bgheights)

        print("fitted plane: z = {:9.4f}x + {:9.4f}y + {:9.4f}, x and y are in the unit of [pixels]".format(a, b, c))

        # ---------------------------------------------------------------------
        # dump corrected image

        with open(self.filename, "w") as f:
            for y in range(0, self.afmy):
                line = ""
                for x in range(0, self.afmx):
                    stage = a * x + b * y + c
                    height = self.afmimg[y][x] - stage
                    line += "{:10.5f} ".format(height)
                line += "\n"
                f.write(line)
        print("done.")

        return

# ------------------------------------------------------------------------------

if len(sys.argv) != 3 and len(sys.argv) != 4:
    print("usage: python3 <filename.asd> <frame> [channel=1 or 2, 1 by default]")
    sys.exit(1)

data = libasd.read_asd(sys.argv[1])

x_pixels = data.header.x_pixel
y_pixels = data.header.y_pixel

print("image size = ", x_pixels, "x", y_pixels)

if len(sys.argv) == 4:
    channel = int(sys.argv[3])
    frame   = int(sys.argv[2])
    afmimg = data.channels[channel][frame].image()
    print("reading image at channel ", channel, ", frame ", frame)
else:
    frame   = int(sys.argv[2])
    afmimg = data.frames[frame].image()
    print("reading image at frame ", frame)

filename = sys.argv[1][0:-4] + "_corrected.dat" # for output
logname  = sys.argv[1][0:-4] + "_background.dat" # for output

root = tkinter.Tk()
app = TiltCorrection(afmimg, x_pixels, y_pixels, filename, logname, master=root)
app.mainloop()
