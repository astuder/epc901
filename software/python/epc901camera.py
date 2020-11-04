# Wrapper library for epc901 breakout board
#
# Copyright 2020 by Adrian Studer
#
# Licensed under the terms of MIT License, all text here must be included in any redistribution.
# See the LICENSE-SW file for details.

import serial
import sys
from textwrap import wrap

class Camera:    
    ser = serial.Serial()
    def open(self, port, speed=115200):
        try:
            self.ser = serial.Serial(port, speed, timeout=30)            
            self.sendCommand("echo off")
            while self.ser.in_waiting:
                self.ser.readline()     # flush any extra lines
            self.sendCommand("reset camera")
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

    def setTriggerSource(self, source):
        self.sendCommand("trigger on")
        trig_str = "trigger source {}".format(source)
        self.sendCommand(trig_str)

    def setTriggerDelay(self, delay):
        trig_str = "trigger delay {}".format(delay)
        self.sendCommand(trig_str)

    def setTriggerDirection(self, direction):
        trig_str = "trigger direction {}".format(direction)
        self.sendCommand(trig_str)

    def setTriggerLevel(self, level):
        trig_str = "trigger level {}".format(level)
        self.sendCommand(trig_str)

    def setTriggerZone(self, x1, y1, x2, y2):
        trig_str = "trigger zone {} {} {} {}".format(x1, y1, x2, y2)
        self.sendCommand(trig_str)

    def capture(self):
        self.sendCommand("capture")

    def captureBurst(self, fast=False):
        if fast==True:
            self.sendCommand("burst fast")
        else:
            self.sendCommand("burst on")
        self.sendCommand("capture")
        self.sendCommand("burst off")

    def getPixels(self, last=False):
        transfer_cmd = "transfer"
        if last==True:
            # get pixels of last frame, discard the others
            transfer_cmd = "transfer last"
        frame_meta = self.sendCommand(transfer_cmd) # frame number, timestamp, exposure
        while frame_meta.startswith("BUSY"):
            frame_meta = self.sendCommand(transfer_cmd) # retry if camera is busy

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
