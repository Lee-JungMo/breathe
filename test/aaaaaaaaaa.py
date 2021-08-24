from pynfft.nfft import NFFT

plan = NFFT([16, 16], 92)
print(plan.M)
print(plan.N)