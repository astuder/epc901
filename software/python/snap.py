# Snap single image from EPC901 breakout board

from epc901camera import Camera
import sys
import argparse

# read command line arguments
# we expect camera.py -p <serial port> -e <exposure time in ms>
parser = argparse.ArgumentParser(description="EPC901 Live View 0.1")
parser.add_argument("-p", help="serial port of camera, for example /dev/serial0 or COM3", dest="port", required=True)
parser.add_argument("-e", help="exposure time in milliseconds", dest="exposure", type=int, default=1000)
parser.add_argument("-a", help="automatically scale based on image content", dest="auto_scale", action="store_true", default=False)
parser.add_argument("-q", help="quiet mode, don't show window with graph", dest="quiet", action="store_true", default=False)
parser.add_argument("-gpng", help="file to save graph of data (PNG)", dest="graph_file")
parser.add_argument("-png", help="file to save image (PNG)", dest="png_file")
args = parser.parse_args()

camera = Camera()
camera.open(args.port)
camera.setExposure(args.exposure)
camera.capture()
pixels = camera.getPixels(True)
camera.close()

if args.png_file:
    from PIL import Image
    png = Image.new("L", (len(pixels), 1))
    if args.auto_scale == True:
        max_val = max(pixels)
        min_val = min(pixels)
        scale = 256/(max_val - min_val)
        png.putdata(pixels, scale=scale, offset=-min_val*scale)
    else:
        png.putdata(pixels, scale=256/3000)
    png.save(args.png_file)

if args.quiet == False or args.graph_file:
    import matplotlib
    import matplotlib.pyplot as plt

    if args.quiet == True:
        matplotlib.use("Agg")

    fig = plt.figure()
    ax1 = fig.add_subplot(1,1,1)
    if args.auto_scale == False:
        ax1.set_ylim((0, 3000), auto=False)
    ax1.plot(pixels)

    if args.graph_file:
        plt.savefig(args.graph_file, format="png")

    if args.quiet == False:
        plt.show()
