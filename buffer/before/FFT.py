from __future__ import division
import matplotlib.pyplot as plt
from scipy.misc import electrocardiogram
from scipy.signal import find_peaks
import numpy as np
import csv
import sys
import random
import time
import os
import os.path
import math
import glob
import pandas as pd
from pandas import read_csv
from scipy import fftpack
from scipy.signal import butter, lfilter
from scipy.signal import lombscargle
from astropy.timeseries import LombScargle

def main():

    time = []
    ecg = []
    file_tsv = "ecg" + str(sys.argv[1]) + ".tsv"
    time_step = 0.0001
    start = 1000
    end = 9000

    with open(file_tsv, "r") as f:
        reader = csv.reader(f, delimiter = '\t')
        for txt in reader:
            time.append(float(txt[0]))
            ecg.append(float(txt[1]))
    sig = np.array(ecg)
    timenp = np.array(time)
    #print(timenp)
    
    x = sig[100:len(sig)-100]
    peaks, _ = find_peaks(x, distance=600)

    modify_peaks = []
    for index in range(len(peaks)):
        if x[peaks[index]] > x.mean() * 1.3:
            modify_peaks.append(peaks[index])
    
    plt.figure(figsize=(20,5))
    plt.plot(x)
    plt.plot(modify_peaks, x[modify_peaks], "x")
    plt.plot(np.zeros_like(x), "--", color="gray")
    
    dif_t = []
    peak_t = []
    for step in modify_peaks:
        peak_t.append(timenp[step])

    #print(peak_t)
    modify_peak_t = np.delete(peak_t, len(modify_peaks)-1)
    for step in np.diff(modify_peaks):
        dif_t.append(timenp[step]) 
    
    plt.figure(figsize=(6,5))
    plt.plot(modify_peak_t, dif_t)

    n = len(modify_peak_t)
    duration = modify_peak_t.ptp()
    freqs = np.linspace(1/duration/1000, n/duration, 1000*n)
    print(len(freqs))
    periodogram = lombscargle(modify_peak_t, dif_t, freqs)
    kmax = periodogram.argmax()
    print(freqs[kmax])
    #print(n, duration, 1/duration/100, n/duration*2)

    plt.figure(figsize=(20,5))
    plt.plot(freqs, np.sqrt(4*periodogram/(100*n)))
    plt.xlabel('Frequency (rad/s)')
    plt.grid()
    plt.axvline(freqs[kmax], color='r', alpha=0.25)
    plt.show()

if __name__ == "__main__":
    main()
