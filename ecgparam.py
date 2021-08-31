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
from astropy.timeseries import LombScargle

def main():

    para_tsv = "data/ecg_para_result" + str(sys.argv[3]) + ".tsv"
    with open(para_tsv, "w") as p:
        makewrite = csv.writer(p, delimiter = '\t')
        first = []
        first.append("avg")
        first.append("SDNN")
        first.append("SDSD")
        first.append("RMSSD")
        first.append("LF_HF_ratio")
        first.append("file_number")
        makewrite.writerow(first)

        for file_number in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
            time = []
            ecg = []
            file_tsv = "data/ecg" + str(file_number) + ".tsv"
            time_step = 0.0001
            LF = [0.05, 0.15]
            HF = [0.15, 0.4]

            with open(file_tsv, "r") as f:
                reader = csv.reader(f, delimiter = '\t')
                for txt in reader:
                    time.append(float(txt[0]))
                    ecg.append(float(txt[1]))
            sig = np.array(ecg)
            timenp = np.array(time)
            
            x = sig[100:len(sig)-100]
            #x = sig[30000:40000]
            peaks, _ = find_peaks(x, distance=100)

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
            
            modify_peak_t = np.delete(peak_t, len(modify_peaks)-1)
            for step in np.diff(modify_peaks):
                dif_t.append(timenp[step]) 
            
            plt.figure(figsize=(6,5))
            plt.plot(modify_peak_t, dif_t)

            plt.figure(figsize=(20,5))
            frequency, power = LombScargle(modify_peak_t, dif_t).autopower(minimum_frequency=0.001,maximum_frequency=1.000, samples_per_peak=50)
            plt.plot(frequency, power)

            LF_power = 0.0
            HF_power = 0.0

            for n in range(len(frequency)):
                if frequency[n] > LF[0] and frequency[n] < LF[1]:
                    LF_power = LF_power + power[n]
                if frequency[n] > HF[0] and frequency[n] < HF[1]:
                    HF_power = HF_power + power[n]
            
            LF_HF_ratio = LF_power/HF_power
            avg = np.mean(dif_t)
            SDNN = np.std(dif_t)
            dif_dif_t = np.diff(dif_t)
            SDSD = np.std(dif_dif_t)
            RMSSD = ((dif_dif_t ** 2).sum() / len(dif_dif_t)) ** 0.5 

            param = []
            param.append(str(avg))
            param.append(str(SDNN))
            param.append(str(SDSD))
            param.append(str(RMSSD))
            param.append(str(LF_HF_ratio))
            param.append(str(file_number))
            makewrite.writerow(param)

            plt.show()

if __name__ == "__main__":
    main()
