from __future__ import division
import matplotlib.pyplot as plt
#from scipy.misc import electrocardiogram
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

def main():

    time = []
    ecg = []
    file_tsv = "ecg" + str(sys.argv[1]) + ".tsv"
    peak_tsv = "peak" + str(sys.argv[1]) + ".tsv"
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
    
    #x = sig[100:len(sig)-100]
    x = sig[440000:470000]
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
    modify_peak_t = modify_peak_t[10:len(modify_peaks)-11]
    for step in np.diff(modify_peaks):
        dif_t.append(timenp[step]) 
    
    dif_t = dif_t[10:len(dif_t)-10]

    print(len(modify_peak_t), len(dif_t))
    with open(peak_tsv, "w") as p:
        makewrite = csv.writer(p, delimiter = '\t')
        for n in range(len(dif_t)):
            data = []
            data.append(str(modify_peak_t[n]))
            data.append(str(dif_t[n]))
            #data = data.decode('utf-8')
            #print(data)
            makewrite.writerow(data)

    plt.figure(figsize=(6,5))
    plt.plot(modify_peak_t, dif_t)

    avg = np.mean(dif_t)
    SDNN = np.std(dif_t)

    dif_dif_t = np.diff(dif_t)
    SDSD = np.std(dif_dif_t)

    RMSSD = ((dif_dif_t ** 2).sum() / len(dif_dif_t)) ** 0.5 

    print("avg : " + str(avg) + ", SDNN : " + str(SDNN) + ", SDSD : " + str(SDSD) + ", RMSSD : " + str(RMSSD))

    plt.show()

if __name__ == "__main__":
    main()
