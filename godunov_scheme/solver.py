import matplotlib.pyplot as plt
from itertools import product
from PIL import Image
from math import ceil
import numpy as np
import imageio
import io

# Constants
EPS =  1e-9

LEFT_X_EDGE = -10
RIGHT_X_EDGE = 10

LEFT_T_EDGE = 0
RIGHT_T_EDGE = 1

def makeGIF(uApprx : np.ndarray, uExact : np.ndarray, GIFFilename: str) -> None:

	maxValue = uApprx.max()
	minValue = uApprx.min()

	xValues = np.linspace(LEFT_X_EDGE, RIGHT_X_EDGE, uApprx.shape[1])

	bytesBuffer = []
	for i in range(uApprx.shape[0]):
		bytesBuffer.append(io.BytesIO())

	framesBuffer = []

	print("\tGenerating frames...")
	for rowNumber, (yApprxValues, yExactValues) in enumerate(zip(uApprx, uExact)):
		plt.ylim(minValue - 0.25, maxValue + 0.25)
		plt.plot(xValues, yApprxValues)
		plt.plot(xValues, yExactValues)
		plt.savefig(bytesBuffer[rowNumber], format="jpeg")
		framesBuffer.append(Image.open(bytesBuffer[rowNumber]))
		plt.cla()

	imageio.mimsave("./" + GIFFilename + '.gif', framesBuffer)
	print("\tGenerated GIF:\n\t\t\"./" + GIFFilename + ".gif\"")
	
	for i in range(uApprx.shape[0]):
		bytesBuffer[i].close();

def v(x : float) -> float:
	if x < EPS:
		return 3.0
	return 1.0

def u(x : float, a : float, t : float) -> float:
	return v(x - a * t)

def solve(xNodesNumber : int, r : float, a : float) -> np.ndarray:
	h = (RIGHT_X_EDGE - LEFT_X_EDGE) / (xNodesNumber - 1)
	t = r * h / a

	tNodesNumber = ceil((RIGHT_T_EDGE - LEFT_T_EDGE) / t) + 1

	uApprx = np.zeros((tNodesNumber, xNodesNumber))

	for xi in range(0, xNodesNumber, 1):
		uApprx[0][xi] = v(LEFT_X_EDGE + xi * h)

	for ti in range(1, tNodesNumber, 1): 
		uApprx[ti][0] = v(LEFT_X_EDGE)
		for xi in range(1, xNodesNumber, 1):
			uApprx[ti][xi] = \
				uApprx[ti - 1][xi] - r * (uApprx[ti - 1][xi] - uApprx[ti - 1][xi - 1])

	return uApprx

def exact(xNodesNumber : int, r : float, a : float) -> np.ndarray:
	h = (RIGHT_X_EDGE - LEFT_X_EDGE) / (xNodesNumber - 1)
	t = r * h / a

	tNodesNumber = ceil((RIGHT_T_EDGE - LEFT_T_EDGE) / t) + 1

	uExact = np.zeros((tNodesNumber, xNodesNumber))

	for ti in range(tNodesNumber): 
		for xi in range(xNodesNumber):
			uExact[ti][xi] = u(LEFT_X_EDGE + xi * h, a, LEFT_T_EDGE + ti * t)

	return uExact

def main() -> None:

	xNodesNumberValues = [11, 101]
	rValues = [0.25, 0.5, 1, 1.25]
	aValues = [5]

	for combination in product(xNodesNumberValues, rValues, aValues):

		xNodesNumber = combination[0]
		r = combination[1]
		a = combination[2]

		print("Solving with parameters: xNodesNumber={}, r={}, a={}".format(
			xNodesNumber, r, a
		))

		uApprx = solve(xNodesNumber, r, a)
		uExact = exact(xNodesNumber, r, a);
		
		print("\tSolved! Making GIF...");

		GIFFilename = "_".join([str(xNodesNumber - 1), str(r), str(a)])

		makeGIF(uApprx, uExact, GIFFilename)

main()