import matplotlib.pyplot as plt
from PIL import Image
from math import ceil
import numpy as np
import io

"""
u'' = -2
u_{j-1} - 2u_{j} + u_{j+1} = -2h^2

A = 1, C = -2, B = 1
"""

# BOUNDARY CONDITIONS
y0 = 0
y1 = 0

# GLOBAL VARIABLES
N = 1000
h = 1 / N

def TMA():
	alpha = [0] * (N + 1)
	beta = [0] * (N + 1)
	x = [0] * (N + 1)

	alpha[0] = 0
	beta[0] = 0

	for i in range(1, N + 1, 1):
		alpha[i] = -1 / (-2 + alpha[i - 1])
		beta[i] = ((-2 * h**2) + (-1) * beta[i - 1]) / (-2 + alpha[i - 1])

	x[N] = 0
	for i in range(N - 1, -1, -1):
		x[i] = alpha[i] * x[i + 1] + beta[i]

	return x

def drawPlot(approx):

	X_approx = np.linspace(0, 1, N + 1)
	X_exact = np.linspace(0, 1, 1001)

	exact = [-x*x+(y1-y0+1)*x+y0 for x in X_exact]

	approx_plot = plt.plot(X_approx, approx)
	exact_plot = plt.plot(X_exact, exact)

	plt.legend(["approx", "exact"])

	plt.savefig("TMA_1000.png")

def main():

	X = TMA()
	
	drawPlot(X)

main()
