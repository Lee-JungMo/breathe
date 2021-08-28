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

    para_tsv = "data/pressure_para_result" + str(sys.argv[3]) + ".tsv"
    with open(para_tsv, "w") as p:
        makewrite = csv.writer(p, delimiter = '\t')
        first = []
        makewrite.writerow(first)

        for file_number in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
            file_tsv = "data/pressure" + str(file_number) + ".tsv"
            time = []
            ecg = []
            with open(file_tsv, "r") as f:
                reader = csv.reader(f, delimiter = '\t')
                for txt in reader:
                    time.append(float(txt[0]))
                    ecg.append(float(txt[1]))
            sig = np.array(ecg)
            timenp = np.array(time)
            
            #x = sig[100:len(sig)-100]
            #t = timenp[100:len(sig)-100]
            x = sig[10000:15000]
            t = timenp[10000:15000]

            plt.figure(figsize=(20,5))
            plt.plot(t, x)

            plt.show()

if __name__ == "__main__":
    main()
