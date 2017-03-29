import numpy as np, matplotlib.pyplot as plt

figure_folder = "figures/"

def gf1():
	filename = "gammaFunctional_stats.txt"

	data = np.loadtxt("%s" % filename,skiprows=4)

	file = open("%s" % filename)
	lines = file.readlines()
	acceptanceRate = lines[0].split()[-1]
	NCor = lines[1].split()[-1]
	NCf = lines[2].split()[-1]
	NTherm = lines[3].split()[-1]

	plt.errorbar(data[:,0],data[:,1],(data[:,2]),fmt="-o",ecolor="r",color="0")
	plt.title(r"MC acceptance rate: $%1.3f%%$, $N_{Cor}=%g$, $N_{Cf}=%g$, $N_{Therm}=%g$" % (float(acceptanceRate),int(NCor),int(NCf),int(NTherm)),fontsize=17)
	plt.xlabel(r"$t$",fontsize="18")
	plt.ylabel(r"$\Delta E$",fontsize="18")
	plt.grid()
	plt.savefig(figure_folder + "%s.png" % filename.split('.')[0],dpi=300)

def gf2():
	filename = "gammaFunctional_stats2.txt"

	data = np.loadtxt("%s" % filename,skiprows=4)

	file = open("%s" % filename)
	lines = file.readlines()
	acceptanceRate = lines[0].split()[-1]
	NCor = lines[1].split()[-1]
	NCf = lines[2].split()[-1]
	NTherm = lines[3].split()[-1]

	plt.figure()
	plt.errorbar(data[:,0],data[:,1],(data[:,2]),fmt="-o",ecolor="r",color="0")
	plt.title(r"MC acceptance rate: $%1.3f%%$, $N_C=%g$, $N_{Cf}=%g$, $N_{Therm}=%g$" % (float(acceptanceRate),int(NCor),int(NCf),int(NTherm)),fontsize=17)
	plt.xlabel(r"$t$",fontsize="18")
	plt.ylabel(r"$\Delta E$",fontsize="18")
	plt.grid()
	plt.savefig(figure_folder + "%s.png" % filename.split('.')[0],dpi=300)

if __name__ == '__main__':
	gf1()
	gf2()
	plt.show()