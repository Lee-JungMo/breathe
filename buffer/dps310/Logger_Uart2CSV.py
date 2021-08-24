# coding: utf-8
# Author: Joonhwa Choi, pass2774@gmail.com
# Last update: 2021-02-04

import numpy as np
import random
import time


import csv
import os
import os.path
import math
import glob
import pandas as pd




########################################################################
## UART Serial Communication
## Ref:
## [Parsing] https://stackoverflow.com/questions/15039211/arduino-to-python-serial-read-split
## [Basig ] https://tre2man.tistory.com/204  
# Install pyserial package
from serial import Serial

sudoPassword = '12341234'
command = 'chmod 777 /dev/ttyACM0'
ser = Serial('/dev/ttyACM0', 115200) # Window # COM3 is for my computer
#ser = Serial('COM8',1000000) # Window
#ser = Serial('dev/ttyACM0',115200) #리눅스



max_points = 200 #

data_result=np.zeros(max_points, dtype=np.float); #원래 max_points+1
data_calib = [0,0]
#b_RNN_ready=False


# For absolute path notation, designate like : 'C:/Users/Joonhwa Choi/Dropbox/내 PC (DESKTOP-5H5JVFN)/Desktop/PDFC_AI_prototype/PDFC_AI_prototype/Data_RNN_result.csv'
fname_RNNresult='Data_RNN_result.csv'
if not os.path.isfile(fname_RNNresult):
    try:
        open(fname_RNNresult,"x")
    except:
        pass
#"x" - Create - will create a file, returns an error if the file exist
#"a" - Append - will create a file if the specified file does not exist
#"w" - Write - will create a file if the specified file does not exist

with open("uart_log.csv","w") as f: # this is for write on csv file.
    pass

print("Uart Logger started!!")
data_calib[0]=0
time_stamp_start=time.time()





#while True:
#    try:
#    #if ser.readable():
#        serialdata = ser.readline()
#        #print(serialdata)
#        #data_decoded= serialdata.decode()[:len(serialdata)-2].split(',') # -2 : exclude \r\n
#        #data_calib[0]=(float(data_decoded[0])-MIN_TF)/(MAX_TF-MIN_TF)
#        #data_calib[1]=(float(data_decoded[1])-MIN_TR)/(MAX_TR-MIN_TR)
#        data_decoded= serialdata.decode()[:len(serialdata)-1] # -1 : exclude \n
#        print(data_decoded)
#        data_temp=(float(data_decoded))
#        if (data_temp>100 and data_temp<4500):
#            data_calib[0]=(data_temp-1613)*1

#        #with open("uart_log.csv","a") as f:
#        #    writer = csv.writer(f,delimiter=",", lineterminator='\n')
#        #    writer.writerow([time.time()-time_stamp_start,data_calib[0],data_calib[1]])


#    except:
#        pass

    #Transmit the calculated target value
    #ser.write(bytes("%d, %d \n" % (data_calib[0],data_calib[1]), encoding='ascii'))
    #time.sleep(0.0001)




with open("uart_log.csv","w") as f:    # 결론은
    while True: # loop
        try:
        #if ser.readable():
            serialdata = ser.readline()
            #print(serialdata)
            #data_decoded= serialdata.decode()[:len(serialdata)-2].split(',') # -2 : exclude \r\n
            #data_calib[0]=(float(data_decoded[0])-MIN_TF)/(MAX_TF-MIN_TF)
            #data_calib[1]=(float(data_decoded[1])-MIN_TR)/(MAX_TR-MIN_TR)
            #data_decoded= serialdata.decode()[:len(serialdata)-1] # -1 : exclude \n
            
            data_decoded= serialdata.decode()[0:len(serialdata)-2].split(',') # -2 : exclude \r\n

            print(data_decoded[0])

            #data_decoded=(int(data_decoded))
            #if (data_decoded>500 and data_decoded<5000):
            #writer = csv.writer(f,delimiter=",", lineterminator='\n')
            #writer.writerow([time.time()-time_stamp_start,data_decoded[0],data_decoded[1],data_decoded[2],data_decoded[3]]) # adc pin number
#Slow ... 70% of the data are lost
#                data_temp=(float(data_decoded))
#            if (data_temp>800 and data_temp<4500):
#                data_calib[0]=(data_temp-1613)*1
#                writer = csv.writer(f,delimiter=",", lineterminator='\n')
#                writer.writerow([time.time()-time_stamp_start,data_calib[0],data_calib[1]])
        except KeyboardInterrupt:
            print('stop')







