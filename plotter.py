import numpy as np, matplotlib.pyplot as plt, sys
import multiprocessing, time

class HOAnalysis:
	"""
	Class for analysing the quantum harmonic oscillator solved with the Metropolis algorithm
	"""
	def __init__(self, filename, output_folder, parallel=True, numprocs=4):
		print "Loading data from %s" % filename
		self.data = np.transpose(np.loadtxt(output_folder + filename, skiprows=4).astype(float))
		self.output_folder = output_folder
		self.N_data_points, self.N_sample_points = self.data.shape
		self.parallel = parallel
		self.numprocs = numprocs
		self.filename = filename
		self.load_stats()
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
				bs[i,j] = bootstrap_statistic(self.data[i][index_lists[j]])
		post_time = time.clock()
		par_time = (post_time - pre_time)
		print 'bs time: %g' % par_time

		self.N_BS = N_BS
		self.bs = bs

	def bin(self,N_bins):
		"""
		Args:
			N_bins: size of the bootstrap averages
		"""
		bs_averaged = np.zeros((self.N_data_points),int(self.N_BS/N_bins))
		for n in xrange(self.N_data_points):
			for i in xrange(0,self.N_BS,N_bins):
				for j in xrange(0,N_bins):
					bs_averaged[n,i] = self.bs[n,i+j]
				bs_averaged[n,i] /= float(N_bins)
		print "Binning complete"
		self.bs = bs_averaged

	def load_stats(self):
		file = open(self.output_folder + "%s" % self.filename)
		lines = file.readlines()
		self.acceptanceRate = float(lines[0].split()[-1])
		self.NCor = int(lines[1].split()[-1])
		self.NCf = int(lines[2].split()[-1])
		self.NTherm = int(lines[3].split()[-1])
		file.close()
		return self.acceptanceRate, self.NCor, self.NCf, self.NTherm

	def stats(self, function=lambda x:x):
		function_data = function(self.bs)
		self.deltaE = np.mean(function_data, axis=1)
		self.deltaE_std = np.std(function_data, axis=1)
		# plt.errorbar(np.arange(self.N_data_points),y=self.deltaE,yerr=self.deltaE_std)

	def fit_data(self, n_fit=5):
		data_fit = np.concatenate([-self.deltaE[-n_fit:-1],self.deltaE[:n_fit]])
		data_fit_std = np.concatenate([self.deltaE_std[-n_fit:-1],self.deltaE_std[:n_fit]])
		data_points = np.arange(-len(data_fit)/2.,len(data_fit)/2.)

		fit, cov = np.polyfit(data_points, data_fit, 1, cov=True, w=1.0/data_fit_std)
		errors = np.sqrt(np.diag(cov)) # Calculating errors from the covariance matrix

		x = np.linspace(-len(data_fit)/2.,len(data_fit)/2.,100)
		y = np.polyval(fit,x)

		print "a = %g, b = %g" % (fit[0], fit[1])
		print "a_err = %.6g, b_err = %.6g" % (errors[0],errors[1])

		y_lim = 1.3
		plt.figure()
		plt.plot(x,y)
		plt.errorbar(data_points, y=data_fit, yerr=data_fit_std, fmt=".")
		plt.ylim(-y_lim,y_lim)
		plt.title(r"$f(x) = ax+b, a=%g\pm %.6g, b=%g\pm%.6g$" % (fit[0], errors[0], fit[1], errors[1]))
		plt.xlabel(r"N")
		plt.ylabel(r"$\Delta E$")
		plt.grid()
		plt.savefig(figure_folder + "%s_linear_fit.png" % self.filename.split('.')[0], dpi=300)

	def plot_data(self, x=None):
		if not x: x = np.arange(self.N_data_points)
		self.x = x
		plt.figure()
		plt.errorbar(x, y=self.deltaE, yerr=self.deltaE_std, fmt="-o", ecolor="r", color="0")
		plt.title(r"$N_{Cor}=%g$, $N_{Cf}=%g$, $N_{Therm}=%g$, $N_{Boot}=%g$" % (self.NCor, self.NCf, self.NTherm*self.NCor, self.N_BS), fontsize=17)
		plt.xlabel(r"$t$", fontsize="18")
		plt.ylabel(r"$\Delta E$", fontsize="18")
		plt.grid()
		plt.savefig(figure_folder + "%s.png" % self.filename.split('.')[0],dpi=300)

	def write_bs_data(self,fname):
		dat = np.zeros((len(self.x),3))
		for i in xrange(len(self.x)):
			dat[i,0] = self.x[i]
			dat[i,1] = self.deltaE[i]
			dat[i,2] = self.deltaE_std[i]
		np.savetxt(fname,dat,fmt=["%10g","%20.16g","%20.16g"])
		print "%s written" % fname

def energy(g):
	a = 0.5
	return np.log(g/np.roll(g,-1,axis=0))/a

def gf_plot(figure_folder, output_folder, filename, plot_range=[0,-1]):
	data = np.loadtxt(output_folder + "%s" % filename, skiprows=4)
	file = open(output_folder + "%s" % filename)
	lines = file.readlines()
	acceptanceRate = float(lines[0].split()[-1])
	NCor = int(lines[1].split()[-1])
	NCf = int(lines[2].split()[-1])
	NTherm = int(lines[3].split()[-1])
	file.close()
	plt.figure()
	plt.errorbar(	data[plot_range[0]:plot_range[-1],0],
					y 		= data[plot_range[0]:plot_range[-1],1],
					yerr	= (data[plot_range[0]:plot_range[-1],2]),
					fmt		= "-o",
					ecolor	= "r",
					color	= "0")
	plt.title(r"MC acceptance rate: $%1.3f%%$, $N_C=%g$, $N_{Cf}=%g$, $N_{Therm}=%g$" % (acceptanceRate,NCor,NCf,NTherm*NCor),fontsize=17)
	plt.xlabel(r"$t$",fontsize="18")
	plt.ylabel(r"$\Delta E$",fontsize="18")
	plt.grid()
	plt.savefig(figure_folder + "%s.png" % filename.split('.')[0],dpi=300)
	print figure_folder + "%s.png" % filename.split('.')[0], "figure created"

def run_G1Analysis():
	G1Analysis = HOAnalysis("gammaFunctional_gamma.txt", output_folder)
	G1Analysis.bootstrap(int(1e2)*5,np.mean)
	# G1Analysis.bin(10)
	G1Analysis.stats(energy)
	G1Analysis.fit_data()
	G1Analysis.plot_data()
	G1Analysis.write_bs_data("bootstrapped_data.txt")

if __name__ == '__main__':
	figure_folder = "figures/"
	output_folder = "output/"
	# gf_plot(figure_folder, output_folder, "gammaFunctional_stats.txt")
	# gf_plot(figure_folder, output_folder, "gammaFunctional_stats2.txt")
	gf_plot(figure_folder, output_folder, "improvedAction_stats.txt",plot_range=[0,5])
	# run_G1Analysis()
	plt.show()