# Live view for epc901 breakout board
#
# Copyright 2020 by Adrian Studer, Frank Milburn
#
# Licensed under the terms of MIT License, all text here must be included in any redistribution.
# See the LICENSE-SW file for details.

from epc901camera import Camera
import sys
import argparse
import matplotlib.pyplot as plt
import matplotlib.animation as animation

camera = Camera()

def graph_loop(i):
    camera.capture() 
    yar = camera.getPixels(True)
    ax1.clear()
    if args.auto_scale == False:
        ax1.set_ylim((0, 3000), auto=False)
    if args.scale_x is not None:
        ax1.plot(xscale, yar)
    else:
        ax1.plot(yar)

# read command line arguments
# we expect camera.py -p <serial port> -e <exposure time in ms>
parser = argparse.ArgumentParser(description="EPC901 Live View 1.0")
parser.add_argument("-p", help="serial port of camera, for example /dev/serial0 or COM3", dest="port", required=True)
parser.add_argument("-e", help="exposure time in microseconds", dest="exposure", type=int, default=1000)
parser.add_argument("-a", help="automatically scale based on image content", dest="auto_scale", action="store_true", default=False)
parser.add_argument("-sx", help="scale x axis given wavelengths w1, w2 at pixels px1 and px2", dest="scale_x", metavar="px1,w1,px2,w2")
args = parser.parse_args()

camera.open(args.port)
camera.setExposure(args.exposure)

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

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

ani = animation.FuncAnimation(fig, graph_loop, interval=100)
plt.show()
camera.close()
