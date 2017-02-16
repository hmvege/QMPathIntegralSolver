import numpy as np, matplotlib.pyplot as plt


filename = "gammaFunctional.txt"

data = np.loadtxt("build/%s" % filename,skiprows=4)

file = open("build/%s" % filename)
lines = file.readlines()
acceptanceRate = lines[0].split()[-1]
NCor = lines[1].split()[-1]
NCf = lines[2].split()[-1]
NTherm = lines[3].split()[-1]

# Bad function fitting
# A = 1
# deltaE = 1.0
# a = 0.5
# t = np.linspace(0,20,21)*a
# T = 10
# def G(x):
# 	# return A*np.cosh(deltaE*(t-T))
# 	return A*(1-np.tanh(t-T/2.0)) -1
# plt.plot(t,G(t))

plt.errorbar(data[:,0],data[:,1],(data[:,2]),fmt="-o",ecolor="r",color="0")
plt.title("MC acceptance rate: %s, NCor=%s, NCf=%s, NTherm=%s" % (acceptanceRate,NCor,NCf,NTherm))
plt.xlabel(r"$t$",fontsize="18")
plt.ylabel(r"$\Delta E$",fontsize="18")
plt.grid()
plt.savefig("%s.png" % filename.split('.')[0],dpi=300)
plt.show()

