# Snap single image from EPC901 breakout board

from epc901camera import Camera
import sys
import argparse

# read command line arguments
# we expect camera.py -p <serial port> -e <exposure time in ms>
parser = argparse.ArgumentParser(description="EPC901 Live View 0.1")
parser.add_argument("-p", help="serial port of camera, for example /dev/serial0 or COM3", dest="port", required=True)
parser.add_argument("-e", help="exposure time in microseconds", dest="exposure", type=int, default=1000)
parser.add_argument("-t", help="enable trigger and select source", dest="trig_source", choices=["external", "level", "region"])
parser.add_argument("-tdir", help="direction for external and level trigger", dest="trig_direction", choices=["falling", "rising"])
parser.add_argument("-tl", help="brightness for level trigger (0-4095)", dest="trig_level", type=int)
parser.add_argument("-tr", help="rectangle for region trigger", dest="trig_region", metavar="x1,y1,x2,y2")
parser.add_argument("-td", help="delay in milliseconds after trigger", dest="trig_delay", type=int)
parser.add_argument("-a", help="automatically scale based on image content", dest="auto_scale", action="store_true", default=False)
parser.add_argument("-gq", help="quiet mode, don't show window with graph", dest="graph_quiet", action="store_true", default=False)
parser.add_argument("-gpng", help="file to save graph of data (PNG)", dest="graph_file")
parser.add_argument("-png", help="file to save image (PNG)", dest="png_file")
parser.add_argument("-csv", help="file to save data (CSV)", dest="csv_file")
args = parser.parse_args()

camera = Camera()
camera.open(args.port)
camera.setExposure(args.exposure)

if args.trig_source is not None:
    camera.setTriggerSource(args.trig_source)
if args.trig_level is not None:
    camera.setTriggerLevel(args.trig_level)
if args.trig_region is not None:
    x1,y1,x2,y2 = args.trig_region.split(",")
    camera.setTriggerRegion(x1, y1, x2, y2)
if args.trig_direction is not None:
    camera.setTriggerDirection(args.trig_direction)
if args.trig_delay is not None:
    camera.setTriggerDelay(args.trig_delay)

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

if args.csv_file:
    try:
        with open(args.csv_file, 'w') as csv:
            x = 0;
            for p in pixels:
                if x == 0:
                    csv.write("{}".format(p))
                else:
                    csv.write(",{}".format(p))
                x += 1
    except:
        print("Error: Failed opening CSV file for writing data.")
        sys.exit()

if args.graph_quiet == False or args.graph_file:
    import matplotlib
    import matplotlib.pyplot as plt

    if args.graph_quiet == True:
        matplotlib.use("Agg")

    fig = plt.figure()
    ax1 = fig.add_subplot(1,1,1)
    if args.auto_scale == False:
        ax1.set_ylim((0, 3000), auto=False)
    ax1.plot(pixels)

    if args.graph_file:
        plt.savefig(args.graph_file, format="png")

    if args.graph_quiet == False:
        plt.show()
