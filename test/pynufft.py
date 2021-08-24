from pynufft import NUFFT
import numpy
A = NUFFT()
om = numpy.random.randn(10,2)
Nd = (64,64)
Kd = (128,128)
Jd = (6,6)
A.plan(om, Nd, Kd, Jd)

x = numpy.random.randn(*Nd)
y = A.forward(x)