# Plot Data from Load Cell 
# Copyright (c) UCLA Rocket Project 2015-2016
# All code licensed under the MIT License
# 13 February 2015

from numpy import cos, sin, pi, absolute, arange
from scipy.signal import kaiserord, lfilter, firwin, freqz
from pylab import figure, clf, plot, xlabel, ylabel, xlim, \
                  ylim, title, grid, axes, show

f = open("sample-formatted.txt", 'r')
times = []
voltage = []

# Loop over lines
i = 0
for line in f:
    # Even: time
    if i % 2 == 0:
        times.append(line)
    # Odd: voltage
    else:
        voltage.append(line)

    i += 1

figure(1)
plot(times, voltage)
xlabel("Time [s]")
ylabel("Voltage [mV]")
title("Load Cell Voltage v. Time")
grid(True)

show()
