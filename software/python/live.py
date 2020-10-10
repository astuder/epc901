# Live view for EPC901 breakout board

from epc901camera import Camera
import sys
import argparse
import matplotlib.pyplot as plt
import matplotlib.animation as animation

camera = Camera()

def loop(i):
    camera.capture() 
    yar = camera.getPixels(True)
    ax1.clear()
    if args.auto_scale == False:
        ax1.set_ylim((0, 3000), auto=False)
    ax1.plot(yar)

# read command line arguments
# we expect camera.py -p <serial port> -e <exposure time in ms>
parser = argparse.ArgumentParser(description="EPC901 Live View 0.1")
parser.add_argument("-p", help="serial port of camera, for example /dev/serial0 or COM3", dest="port", required=True)
parser.add_argument("-e", help="exposure time in milliseconds", dest="exposure", type=int, default=1000)
parser.add_argument("-a", help="automatically scale based on image content", dest="auto_scale", action="store_true", default=False)
args = parser.parse_args()

camera.open(args.port)
camera.setExposure(args.exposure)

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

ani = animation.FuncAnimation(fig, loop, interval=100)
plt.show()
camera.close()
