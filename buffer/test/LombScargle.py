import numpy as np
import random

rand = np.random.default_rng(42)
t = 100 * rand.random(100)
y = np.sin(2 * np.pi * t) + 0.1 * rand.standard_normal(100)

from astropy.timeseries import LombScargle
frequency, power = LombScargle(t, y).autopower()

import matplotlib.pyplot as plt  
plt.plot(frequency, power)     