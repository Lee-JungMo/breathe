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
import sys

sudoPassword = '12341234'
command = 'chmod 777 /dev/ttyACM0'
p = os.system('echo %s|sudo -S %s' % (sudoPassword, command))
ser = Serial('/dev/ttyACM0', 115200)
sample_T = 0.0001

def main():

    file_tsv = "data/ecg" + str(sys.argv[1]) + ".tsv"
    #file_csv = "ecg" + str(sys.argv[1]) + ".csv"
    #with open("ecg2.tsv", "w", newline='', encoding ='utf-8') as g:
    with open(file_tsv, "w") as g:
        makewrite = csv.writer(g, delimiter = '\t')
        time_0 = time.time()
        t_before = 0
        t_after = 0
        n = 0
        try:
            while n < 800000:
                data = []
                if ser.readable():
                    t_present = time.time() - time_0
                    t_after = t_present
                    if t_after - t_before > sample_T:
                        data.append(str(t_present))
                        serialdata = ser.readline()
                        #print(serialdata)
                        data_decoded= serialdata.decode()[:len(serialdata)-1].split(',')
                        print(data_decoded[0])
                        if(int(data_decoded[0]) < 3000):
                            #print(type(data_decoded))
                            data.append(str(data_decoded[0]))
                            #print(data)
                            makewrite.writerow(data)
                            t_before = t_after
                            n = n + 1
                    else:
                        pass
                else:
                    print('cannot open serial')

            else: 
                print('end')
                quit() 
        except KeyboardInterrupt:
            print('stop')

#def convert():
#    dfs = pd.read_csv(file_tsv, sep='\t', chunksize = 50)
#    for df in dfs:
#        df.to_csv(file_csv, sep=',', mode='a')

if __name__ == "__main__":
    main()
    #convert()
