import numpy as np
from scipy import fftpack
from matplotlib import pyplot as plt


# Seed the random number generator
np.random.seed(1234)

time_step = 0.1
period = 5.

time_vec = np.arange(0, 20, time_step)
sig = (np.sin(2 * np.pi / period * time_vec) + 0.5 * np.random.randn(time_vec.size))
#print(time_vec)
#print(sig)
plt.figure(figsize=(6, 5))
plt.plot(time_vec, sig, label='Original signal')

# The FFT of the signal
sig_fft = fftpack.fft(sig)
#print(sig_fft)
#print(sig_fft .shape)
# And the power (sig_fft is of complex dtype)
power = np.abs(sig_fft)
#print(power.shape)
# The corresponding frequencies
print(sig.size)
sample_freq = fftpack.fftfreq(sig.size, d=time_step) 
# 1000, 0.02 -> 1/ 0.02 = 50, 50/1000 = 0.05 
# 200, 0.1 -> 1/0.1 = 10, 10/200 = 0.05 
# 0.05 -25 ~ 25
print(sample_freq)

# Plot the FFT power
plt.figure(figsize=(6, 5))
plt.plot(sample_freq, power)
plt.xlabel('Frequency [Hz]')
plt.ylabel('plower')

# Find the peak frequency: we can focus on only the positive frequencies
pos_mask = np.where(sample_freq > 0)
freqs = sample_freq[pos_mask]
peak_freq = freqs[power[pos_mask].argmax()]

# Check that it does indeed correspond to the frequency that we generate
# the signal with
np.allclose(peak_freq, 1./period)

# An inner plot to show the peak frequency
axes = plt.axes([0.55, 0.3, 0.3, 0.5])
plt.title('Peak frequency')
plt.plot(freqs[:8], power[:8])
plt.setp(axes, yticks=[])
# scipy.signal.find_peaks_cwt can also be used for more advanced  peak detection



high_freq_fft = sig_fft.copy()
high_freq_fft[np.abs(sample_freq) > peak_freq] = 0
filtered_sig = fftpack.ifft(high_freq_fft)

plt.figure(figsize=(6, 5))
plt.plot(time_vec, sig, label='Original signal')
plt.plot(time_vec, filtered_sig, linewidth=3, label='Filtered signal')
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')

plt.legend(loc='best')

#plt.show()