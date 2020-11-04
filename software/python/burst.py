# Capture a burst of images from epc901 breakout board
#
# Copyright 2020 by Adrian Studer, Frank Milburn
#
# Licensed under the terms of MIT License, all text here must be included in any redistribution.
# See the LICENSE-SW file for details.

from epc901camera import Camera
import sys
import argparse

# read command line arguments
# we expect camera.py -p <serial port> -e <exposure time in ms>
parser = argparse.ArgumentParser(description="EPC901 Burst Capture 1.0")
parser.add_argument("-p", help="serial port of camera, for example /dev/serial0 or COM3", dest="port", required=True)
parser.add_argument("-e", help="exposure time in microseconds", dest="exposure", type=int, default=1000)
parser.add_argument("-f", help="number of frames in burst", dest="frames", type=int, default=10)
parser.add_argument("-i", help="interval in milliseconds between frames (0 = best effort)", dest="interval", type=int, default=0)
parser.add_argument("-fast", help="enable fast burst mode", action="store_true", default=False)
parser.add_argument("-t", help="enable trigger and select source", dest="trig_source", choices=["external", "level", "zone"])
parser.add_argument("-tdir", help="direction for external and level trigger", dest="trig_direction", choices=["falling", "rising"], default="falling")
parser.add_argument("-tl", help="brightness for level trigger (0-4095)", dest="trig_level", type=int)
parser.add_argument("-tz", help="rectangle for zone trigger", dest="trig_zone", metavar="x1,y1,x2,y2")
parser.add_argument("-td", help="delay in milliseconds after trigger", dest="trig_delay", type=int, default=0)
parser.add_argument("-a", help="automatically scale based on image content", dest="auto_scale", action="store_true", default=False)
parser.add_argument("-g", help="graph type: 2=animated 2d graph, 3=3d surface plot", dest="graph_type", type=int)
parser.add_argument("-gc", help="alternative color map for 3d graph. currently only supports 'spectrum'", dest="color")
parser.add_argument("-gq", help="quiet mode, don't show window with graph", dest="graph_quiet", action="store_true", default=False)
parser.add_argument("-gf", help="file to save graph of data (2d:GIF, 3d:PNG)", dest="graph_file")
parser.add_argument("-png", help="file to save image (PNG)", dest="png_file")
parser.add_argument("-csv", help="file to save data (CSV)", dest="csv_file")
parser.add_argument("-sx", help="scale x axis given wavelengths w1, w2 at pixels px1 and px2", dest="scale_x", metavar="px1,w1,px2,w2")
args = parser.parse_args()

pixels = []
timestamps = []
camera = Camera()
camera.open(args.port)
camera.setExposure(args.exposure)

if args.trig_source is not None:
    camera.setTriggerSource(args.trig_source)
if args.trig_level is not None:
    camera.setTriggerLevel(args.trig_level)
if args.trig_zone is not None:
    x1,y1,x2,y2 = args.trig_zone.split(",")
    camera.setTriggerZone(x1, y1, x2, y2)
if args.trig_direction is not None:
    camera.setTriggerDirection(args.trig_direction)
if args.trig_delay is not None:
    camera.setTriggerDelay(args.trig_delay)
if args.scale_x is not None:
    px1, w1, px2, w2 = (args.scale_x.split(","))
    px1 = float(px1)
    px2 = float(px2)
    w1 = float(w1)
    w2 = float(w2)
    slope = (w2 - w1)/(px2 - px1)
    intercept = w1 - px1 * slope
    xscale = []
    for i in range(1024):
        xscale = xscale + [slope * float(i) + intercept]

camera.setBurst(args.frames, args.interval)
print("Recording images..")
camera.captureBurst(args.fast)

print("Transfering image data", end="")
p = camera.getPixels()
while p:
    print(".", end="", flush=True)
    pixels.append(p)
    timestamps.append(camera.frame_timestamp)
    p = camera.getPixels()
camera.close()
print()

if args.png_file:
    from PIL import Image
    from itertools import chain
    png = Image.new("L", (len(pixels[0]), args.frames))
    flat = list(chain.from_iterable(pixels))
    if args.auto_scale == True:
        max_val = max(flat)
        min_val = min(flat)
        scale = 256/(max_val - min_val)
        png.putdata(flat, scale=scale, offset=-min_val*scale)
    else:
        png.putdata(flat, scale=256/3000)
    png.save(args.png_file)

if args.csv_file:
    try:
        with open(args.csv_file, 'w') as csv:
            csv.write("frame,time ms")
            for p in range(len(pixels[0])):
                csv.write(",{}".format(p))
            csv.write("\n")
            for f in range(args.frames):
                csv.write("{},{}".format(f,timestamps[f]))
                for p in range(len(pixels[0])):
                    csv.write(",{}".format(pixels[f][p]))
                csv.write("\n")
    except:
        print("Failed opening CSV file for writing data.")

def graph_loop(frame):
    yar = pixels[frame % args.frames]
    ax1.clear()
    if args.auto_scale == False:
        ax1.set_ylim((0, 3000), auto=False)
    if args.scale_x is not None:
        ax1.plot(xscale, yar)
    else:
        ax1.plot(yar)

if args.graph_type == 2:
    # animated 2D graph
    if args.graph_quiet == False or args.graph_file:
        import matplotlib.pyplot as plt
        import matplotlib.animation as animation

        fig = plt.figure()    
        ax1 = fig.add_subplot(1,1,1)
        ani = animation.FuncAnimation(fig, graph_loop, interval=200, save_count=args.frames)
        if args.graph_file:
            print("Saving animation to GIF")
            ani.save(args.graph_file, writer="pillow", fps=5)
        if args.graph_quiet == False:
            plt.show()

if args.graph_type == 3:
    # 3d surface plot
    if args.graph_quiet == False or args.graph_file:
        
        import matplotlib.pyplot as plt
        from matplotlib import cm
        from mpl_toolkits.mplot3d import Axes3D
        import numpy as np

        if args.graph_quiet == True:
            matplotlib.use("Agg")

        data = np.array(pixels)
        width = data.shape[1]
        x, y = np.meshgrid(np.array(timestamps), np.arange(width))

        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.set_xlabel("pixel")
        ax.set_ylabel("time ms")
        ax.set_zlabel("brightness")
        if args.auto_scale == False:
            ax.set_zlim((0, 3000), auto=False)

        if args.color == "spectrum":
            colors = plt.cm.nipy_spectral( (y-y.min())/float((y-y.min()).max()) )
            ax.plot_surface(y, x, np.transpose(data),
                facecolors=colors, shade=False, linewidth=0, antialiased=True)
        else:
            ax.plot_surface(y, x, np.transpose(data),
                cmap=cm.coolwarm, linewidth=0, antialiased=True)

        if args.graph_file:
            plt.savefig(args.graph_file, format="png")

        if args.graph_quiet == False:
            plt.show()
