import matplotlib.pyplot as plt
from scipy.misc import electrocardiogram
from scipy.signal import find_peaks
import numpy as np
import csv

def main():

    time = []
    ecg = []

    with open("ecg3.tsv", "r") as f:
        reader = csv.reader(f, delimiter = '\t')
        for txt in reader:
            time.append(float(txt[0]))
            ecg.append(float(txt[1]))
    sig = np.array(ecg)
    timenp = np.array(time)
    
    x = sig[100:len(sig)-100]
    #print(type(x))
    #print(x)
    peaks, _ = find_peaks(x, distance=600)

    #print(sig.mean())
    modify_peaks = []
    for index in range(len(peaks)):
        #print(x[peaks[index]])
        if x[peaks[index]] > x.mean() * 1.3:
            #print(index)
            modify_peaks.append(peaks[index])
    #print(np.diff(peaks)[0])
    dif_t = []
    for step in np.diff(modify_peaks):
        dif_t.append(timenp[step])

    plt.figure(figsize=(6,5))
    plt.plot(dif_t)
    
    #print(timenp[n])
    plt.figure(figsize=(20,5))
    plt.plot(x)
    plt.plot(modify_peaks, x[modify_peaks], "x")
    plt.plot(np.zeros_like(x), "--", color="gray")
    
    plt.show()

if __name__ == "__main__":
    main()
