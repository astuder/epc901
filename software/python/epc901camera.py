# Wrapper library for EPC901 breakout board

import serial
import sys
from textwrap import wrap

class Camera:    
    ser = serial.Serial()
    def open(self, port, speed=115200):
        try:
            self.ser = serial.Serial(port, speed, timeout=1)            
            self.sendCommand("echo off")
            while self.ser.in_waiting:
                self.ser.readline()     # flush any extra lines
            self.sendCommand("reset sensor")
        except Exception as e:
            print("Failed to connect to sensor on port ", port)
            print("Exception: " + str(e))
            sys.exit()

    def close(self):
        self.sendCommand("echo on")
        self.ser.close()

    def sendCommand(self, command):
        cmd_str = "@{}\n".format(command).encode("utf-8")
        self.ser.write(cmd_str)
        return self.ser.readline().decode("utf-8")

    def setExposure(self, exposure):
        exposure_str = "exposure {}".format(exposure)
        self.sendCommand(exposure_str)

    def setBurst(self, frames, interval):
        burst_str = "burst frames {}".format(frames)
        self.sendCommand(burst_str)
        burst_str = "burst interval {}".format(interval)
        self.sendCommand(burst_str)

    def capture(self):
        self.sendCommand("capture")

    def captureBurst(self):
        self.sendCommand("burst on")
        self.sendCommand("capture")
        self.sendCommand("burst off")

    def getPixels(self, last=False):
        transfer_cmd = "transfer"
        if last==True:
            # get pixels of last frame, discard the others
            transfer_cmd = "transfer last"
        frame_meta = self.sendCommand(transfer_cmd) # frame number, timestamp, exposure
        if frame_meta is None or frame_meta.startswith("ERROR"):
            # no pixels available
            return
        else:
            a, b, c = frame_meta.split(",")
            self.frame_number = int(a)
            self.frame_timestamp = int(b)
            self.frame_exposure = int(c)
        frame_data = [int(x, 16) for x in wrap(self.ser.readline().decode("utf-8"), 3)]
        return frame_data
