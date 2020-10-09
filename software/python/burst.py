# Snap single image from EPC901 breakout board

from epc901camera import Camera
import sys
import argparse

# read command line arguments
# we expect camera.py -p <serial port> -e <exposure time in ms>
parser = argparse.ArgumentParser(description="EPC901 Live View 0.1")
parser.add_argument("-p", help="serial port of camera, for example /dev/serial0 or COM3", dest="port", required=True)
parser.add_argument("-e", help="exposure time in milliseconds", dest="exposure", type=int, default=1000)
parser.add_argument("-f", help="number of frames in burst", dest="frames", type=int, default=10)
parser.add_argument("-i", help="interval in milliseconds between frames (0 = best effort)", dest="interval", type=int, default=0)
parser.add_argument("-a", help="automatically scale based on image content", dest="auto_scale", action="store_true", default=False)
parser.add_argument("-q", help="quiet mode, don't show window with graph", dest="quiet", action="store_true", default=False)
parser.add_argument("-gpng", help="file to save graph of data (PNG)", dest="graph_file")
parser.add_argument("-png", help="file to save image (PNG)", dest="png_file")
args = parser.parse_args()

pixels = []
timestamps = []
camera = Camera()
camera.open(args.port)
camera.setExposure(args.exposure)
camera.setBurst(args.frames, args.interval)
print("Recording images..")
camera.captureBurst()
print("Transfering image data", end="")
camera.getPixels()          # discard first frame as it's noisier than subsequent frames
p = camera.getPixels()
while p:
    if args.quiet == False:
        print(".", end="", flush=True)
    pixels.append(p)
    timestamps.append(camera.frame_timestamp)
    p = camera.getPixels()
camera.close()

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

if args.quiet == False or args.graph_file:
    
    import matplotlib.pyplot as plt
    from matplotlib import cm
    from mpl_toolkits.mplot3d import Axes3D
    import numpy as np

    if args.quiet == True:
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
    ax.plot_surface(y, x, np.transpose(data),
        cmap=cm.coolwarm, linewidth=0, antialiased=True)

    if args.graph_file:
        plt.savefig(args.graph_file, format="png")

    if args.quiet == False:
        plt.show()
