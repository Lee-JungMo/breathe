from serial import Serial 
import numpy as np
import random
import time
import csv
import os
import os.path
import math
import glob
import pandas as pd
from pandas import read_csv
import matplotlib.pyplot as plt
from scipy import fftpack
from scipy.signal import butter, lfilter
import sys

def butter_bandpass(lowcut, highcut, fs, order = 5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype = 'band')
    return b,a

def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    b,a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b,a,data)
    return y

def main():

    time_step = 0.0001
    time = []
    ecg = []
    file_tsv = "ecg" + str(sys.argv[1]) + ".tsv"

    with open(file_tsv, "r") as f:
        reader = csv.reader(f, delimiter = '\t')
        for txt in reader:
            time.append(float(txt[0]))
            ecg.append(float(txt[1]))

    #signal = np.array([time, ecg])
    start = 1000
    #end = len(time) - 1000
    end = 9000
    sig = np.array(ecg)
    timenp = np.array(time)
    plt.figure(figsize=(20,5))
    plt.plot(timenp[start:end], sig[start:end])
    
    #print (sig)
    #fecg = butter_bandpass_filter(sig, 0.01, 2, int(1/time_step), 5)
    #plt.plot(timenp[start:end], fecg[start:end])

    #print(signal.shape)
    sig_fft = fftpack.fft(sig)
    #print(signal_fft.shape)
    sig_power = np.abs(sig_fft)
    #print(signal_power.shape)
    sample_freq = fftpack.fftfreq(sig.size, d=time_step)
    #print(sample_freq.shape)

    plt.figure(figsize=(6,5))
    plt.plot(sample_freq, sig_power)
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('power')

    pos_mask = np.where(sample_freq > 0)
    freqs = sample_freq[pos_mask]
    peak_freq = freqs[sig_power[pos_mask].argmax()]

    print(peak_freq)
    axes = plt.axes([0.55, 0.3, 0.3, 0.5])
    plt.title('Peak frequency')
    plt.plot(freqs[1:1000], sig_power[1:1000])
    plt.setp(axes, yticks=[])

    high_freq_fft = sig_fft.copy()
    high_freq_fft[np.abs(sample_freq) > 200] = 0

    filtered_sig = fftpack.ifft(high_freq_fft)

    plt.figure(figsize=(20, 5))
    plt.plot(timenp[start:end], sig[start:end], label='Original signal')
    plt.plot(timenp[start:end], filtered_sig[start:end], linewidth=3, label='Filtered signal')
    plt.xlabel('Time [s]')
    plt.ylabel('Amplitude')
    plt.legend(loc='best')

    plt.show()
    
if __name__ == "__main__":
    main()
