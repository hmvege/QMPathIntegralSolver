import numpy as np, matplotlib.pyplot as plt, sys
import multiprocessing, time

# class Bootstrap:
# 	"""
# 	Class for creating a bootstrap sample.
# 	"""
# 	def __call__(self, N_BS, data, index_lists=[]):
# 		N = len(data)
# 		if len(index_lists) == 0: # Allows user to send in a predefined list if needed
# 			index_lists = np.random.randint(N, size=(N_BS, N))
# 		return data[index_lists]

def Boot(N_BS, data, index_lists=[]):
	N = len(data)
	if len(index_lists) == 0: # Allows user to send in a predefined list if needed
		index_lists = np.random.randint(N, size=(N_BS, N))
	return data[index_lists]

def BootParallell(data, index_lists):
	return data[index_lists]


class HOAnalysis:
	"""
	Class for analysing the quantum harmonic oscillator solved with the Metropolis algorithm
	"""
	def __init__(self, filename, parallel=True, numprocs=4):
		print "Loading data from %s" % filename
		self.data = np.transpose(np.loadtxt(filename).astype(float))
		self.N_data_points, self.N_sample_points = self.data.shape
		self.parallel = parallel
		self.numprocs = numprocs
		print "Data loaded"

	def bootstrap(self, N_BS, bootstrap_statistic=lambda x:x):
		bs = np.zeros((self.N_data_points, N_BS))

		pre_time = time.clock()
		index_lists = np.random.randint(self.N_sample_points, size=(N_BS,self.N_sample_points))
		post_time = time.clock()
		par_time = (post_time - pre_time)
		print 'Randtime: %g' % par_time
				
		pre_time = time.clock()
		for i in xrange(self.N_data_points):
			for j in xrange(N_BS):
				bs[i,j] = bootstrap_statistic(Boot(N_BS,self.data[i],index_lists[j]))
		# self.bs = bs
		post_time = time.clock()
		par_time = (post_time - pre_time)
		print 'bs time: %g' % par_time

		# pre_time = time.clock()
		# pool = multiprocessing.Pool(processes=self.numprocs)
		# for i in xrange(self.N_data_points):
		# 	# input_values = np.split(np.asarray([self.data[i][index_list] for index_list in index_lists]),self.numprocs)
		# 	bs_array = pool.map(bootstrap_statistic, np.asarray([self.data[i][index_list] for index_list in index_lists]))
		# post_time = time.clock()
		# par_time = (post_time - pre_time)
		# print 'bs(PARALLEL) time: %g' % par_time

		self.bs = bs

	def stats(self, function=lambda x:x):
		function_data = function(self.bs)
		self.deltaE = np.mean(function_data, axis=1)
		self.deltaE_std = np.std(function_data, axis=1)
		# plt.errorbar(np.arange(self.N_data_points),y=self.deltaE,yerr=self.deltaE_std)

	def fit_data(self, n_fit=5):
		data_fit = np.concatenate([self.deltaE[:n_fit],-self.deltaE[-n_fit:]])
		data_fit_std = np.concatenate([self.deltaE_std[:n_fit],self.deltaE_std[-n_fit:]])
		data_points = np.arange(n_fit*2)

		fit, cov = np.polyfit(data_points, data_fit, 1, cov=True, w=1.0/data_fit_std)
		errors = np.sqrt(np.diag(cov)) # Calculating errors from the covariance matrix
		
		x = np.linspace(0,n_fit*2,100)
		y = np.polyval(fit,x)

		print "a = %g, b = %g" % (fit[0],fit[1])
		print "a_err = %.6g, b_err = %.6g" % (errors[0],errors[1])

		y_lim = 1.3
		plt.plot(x,y)
		plt.errorbar(np.arange(len(data_fit)),y=data_fit,yerr=data_fit_std,fmt=".")
		plt.ylim(-y_lim,y_lim)
		plt.title(r"$f(x) = ax+b, a=%g\pm %.6g, b=%g\pm%.6g$" % (fit[0],errors[0],fit[1],errors[1]))
		plt.xlabel(r"N")
		plt.ylabel(r"$\Delta E$")

	def plot_data(self, x=None):
		if not x: x = np.arange(self.N_data_points)
		plt.errorbar(x,y=self.deltaE,yerr=self.deltaE_std,fmt="-o",ecolor="r",color="0")
		plt.title(r"MC acceptance rate: $%1.3f%%$, $N_{Cor}=%g$, $N_{Cf}=%g$, $N_{Therm}=%g$" % (float(acceptanceRate),int(NCor),int(NCf),int(NTherm)),fontsize=17)
		plt.xlabel(r"$t$",fontsize="18")
		plt.ylabel(r"$\Delta E$",fontsize="18")
		plt.grid()
		plt.savefig(figure_folder + "%s.png" % filename.split('.')[0],dpi=300)

def energy(g):
	a = 0.5
	return np.log(g/np.roll(g,-1,axis=0))/a

def gf1(figure_folder, filename):
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

def gf2(figure_folder, filename):
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
	figure_folder = "figures/"
	# gf1(figure_folder, "gammaFunctional_stats.txt")
	# gf2(figure_folder, "gammaFunctional_stats2.txt")
	G1Analysis = HOAnalysis("gammaFunctional_gamma.txt")
	G1Analysis.bootstrap(int(1e4),np.mean)
	G1Analysis.stats(energy)
	G1Analysis.fit_data()
	plt.show()