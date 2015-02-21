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

sample_rate = 1000.0
nsamples = 500

# The Nyquist rate of the signal.
nyq_rate = sample_rate / 2.0

# The desired width of the transition from pass to stop,
# relative to the Nyquist rate.  We'll design the filter
# with a 4 Hz transition width.
width = 4.0/nyq_rate

# The desired attenuation in the stop band, in dB.
ripple_db = 60.0

# Compute the order and Kaiser parameter for the FIR filter.
N, beta = kaiserord(ripple_db, width)

# The cutoff frequency of the filter.
cutoff_hz = 10.0

# Use firwin with a Kaiser window to create a lowpass FIR filter.
taps = firwin(N, cutoff_hz/nyq_rate, window=('kaiser', beta))

# Use lfilter to filter x with the FIR filter.
filtered_x = lfilter(taps, 1.0, x)


figure(1)
plot(times, voltage)
xlabel("Time [s]")
ylabel("Voltage [mV]")
title("Load Cell Voltage v. Time")
grid(True)

show()
