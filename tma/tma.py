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
h = 1 / (N - 1)

def A(row_index):
	if row_index == N:
		return 0
	return 1

def B(row_index):
	if row_index == 0:
		return 0
	return 1

def C(row_index):
	if row_index == 0 or row_index == N:
		return 1
	return -2

def D(row_index):
	if row_index == 0:
		return y0
	if row_index == N:
		return y1
	return -2 * h**2

def getSystem():

	matrix = []
	vector = []

	for i in range(0, N + 1):
		matrix.append([0] * (N + 1))
		vector.append(0)

		if 0 <= i - 1 <= N:
			matrix[i][i - 1] = A(i)
		if 0 <= i <= N:
			matrix[i][i] =	 C(i)
			vector[i] = D(i)
		if 0 <= i + 1 <= N:
			matrix[i][i + 1] = B(i)

	return (matrix, vector)

def TMA(matrix, vector):
	alpha = [0] * (N + 1)
	beta = [0] * (N + 1)
	x = [0] * (N + 1)

	if (matrix[0][0] == 0):
		print("Can not solve the system!");
		return None

	alpha[1] = -matrix[0][1] / matrix[0][0]
	beta[1] = vector[0] / matrix[0][0]

	for i in range(2, N + 1):
		div = matrix[i - 1][i - 2] * alpha[i - 1] + matrix[i - 1][i - 1]
		if div == 0:
			print("Can not solve the system!");
			return None

		alpha[i] = -matrix[i - 1][i] / div
		beta[i] = (vector[i - 1] - matrix[i - 1][i - 2] * beta[i - 1]) / div

	if matrix[N][N] + matrix[N][N - 1] * alpha[N] == 0:
		print("Can not solve the system!");
		return None

	x[N] = (vector[N] - matrix[N][N - 1] * beta[N]) / (matrix[N][N] + matrix[N][N - 1] * alpha[N]);
	for i in range(N - 1, -1, -1):
		x[i] = alpha[i + 1] * x[i + 1] + beta[i + 1]

	return x

def drawPlot(approx):

	X = np.linspace(0, 1, N + 1)
	exact = [-x*x+(y1-y0+1)*x+y0 for x in X]

	approx_plot = plt.plot(X, approx)
	exact_plot = plt.plot(X, exact)

	plt.legend(["approx", "exact"])


	plt.savefig("TMA.png")

def main():

	matrix, vector = getSystem()
	X = TMA(matrix, vector)
	drawPlot(X)

main()
