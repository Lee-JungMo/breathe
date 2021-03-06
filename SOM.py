# Self Organizing Map

# Importing the libraries
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
import csv

# Importing the dataset
#dataset = pd.read_csv('data/Credit_Card_Applications.csv')
#X = dataset.iloc[:, :-1].values
#y = dataset.iloc[:, -1].values

X = []
y = []
para_tsv = "data/para_result" + str(sys.argv[1]) + ".tsv"
with open(para_tsv, "r") as p:
    reader = csv.reader(p, delimiter = '\t')
    k = 0
    for txt in reader:
        if k == 0:
            k = k + 1
            pass
        else:
            data = []
            for n in range(0,5):
                data.append(float(txt[n]))
            X.append(data)
            y.append(float(txt[5]))
X = np.array(X)
y = np.array(y)

# Feature Scaling
from sklearn.preprocessing import MinMaxScaler
sc = MinMaxScaler(feature_range = (0, 1))
X = sc.fit_transform(X)

# Training the SOM
from minisom import MiniSom
som = MiniSom(x = 10, y = 10, input_len = 15, sigma = 1.0, learning_rate = 0.5)
som.random_weights_init(X)
som.train_random(data = X, num_iteration = 1000)

# Visualizing the results
from pylab import bone, pcolor, colorbar, plot, show
bone()
pcolor(som.distance_map().T)
colorbar()
markers = ['o', 's']
colors = ['r', 'g']
for i, x in enumerate(X):
    w = som.winner(x)
    plot(w[0] + 0.5,
         w[1] + 0.5,
         markers[y[i]],
         markeredgecolor = colors[y[i]],
         markerfacecolor = 'None',
         markersize = 10,
         markeredgewidth = 2)
show()

# Finding the frauds
mappings = som.win_map(X)
frauds = np.concatenate((mappings[(8,1)], mappings[(6,8)]), axis = 0)
frauds = sc.inverse_transform(frauds)