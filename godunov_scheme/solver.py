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
	# Max and Min values are used for plotting
	maxValue = uApprx.max()
	minValue = uApprx.min()

	# Generate values for X
	xValues = np.linspace(LEFT_X_EDGE, RIGHT_X_EDGE, uApprx.shape[1])

	# Make DIFFERENT byte buffers for transforming plots to PIL.Image objects
	bytesBuffer = []
	for i in range(uApprx.shape[0]):
		bytesBuffer.append(io.BytesIO())

	# Buffer for PIL.Image objects
	framesBuffer = []

	print("\tGenerating frames...")
	# Draw a plot for every row from approximate and exact solution
	for rowNumber, (yApprxValues, yExactValues) in enumerate(zip(uApprx, uExact)):
		plt.ylim(minValue - 0.25, maxValue + 0.25)         # Set y-axis limits
		plt.plot(xValues, yApprxValues)                    # Draw approximate solution
		plt.plot(xValues, yExactValues)                    # Draw exact solution
		plt.savefig(bytesBuffer[rowNumber], format="jpeg") # Save plot to the buffer

		# Read image from buffer (it will be read as PIL.Image object) and
		#	store it to the frame buffer
		framesBuffer.append(Image.open(bytesBuffer[rowNumber]))

		plt.cla() # Clear plot

	# Concatenate all the previously generated images into gif file
	imageio.mimsave("./" + GIFFilename + '.gif', framesBuffer)
	print("\tGenerated GIF:\n\t\t\"./" + GIFFilename + ".gif\"")
	
	# Close buffers
	for i in range(uApprx.shape[0]):
		bytesBuffer[i].close()

# "Shelf" function
def v(x : float) -> float:
	if x < EPS:
		return 3.0
	return 1.0

# "Running shelf" function
def u(x : float, a : float, t : float) -> float:
	return v(x - a * t)

def solve(xNodesNumber : int, r : float, a : float) -> np.ndarray:
	# Length of segments by which approximation segment was divided
	h = (RIGHT_X_EDGE - LEFT_X_EDGE) / (xNodesNumber - 1)
	# Length of time steps
	t = r * h / a

	# Determine how many by how many segments time segment will be divided
	tNodesNumber = ceil((RIGHT_T_EDGE - LEFT_T_EDGE) / t) + 1
	# Array for storing solution
	uApprx = np.zeros((tNodesNumber, xNodesNumber))

	# Fill initial values
	for xi in range(0, xNodesNumber, 1):
		uApprx[0][xi] = v(LEFT_X_EDGE + xi * h)

	# Fill other layers using Godunov's scheme
	for ti in range(1, tNodesNumber, 1):
		# The leftmost value of "running shelf" is always v(LEFT_X_EDGE)
		# Because for all x such that x < 0, v(x) = 3
		uApprx[ti][0] = v(LEFT_X_EDGE)
		# Fill remain of the row
		for xi in range(1, xNodesNumber, 1):
			uApprx[ti][xi] = \
				uApprx[ti - 1][xi] - r * (uApprx[ti - 1][xi] - uApprx[ti - 1][xi - 1])

	return uApprx

def exact(xNodesNumber : int, r : float, a : float) -> np.ndarray:
	# Length of segments by which approximation segment was divided
	h = (RIGHT_X_EDGE - LEFT_X_EDGE) / (xNodesNumber - 1)
	# Length of time steps
	t = r * h / a

	# Determine how many by how many segments time segment will be divided
	tNodesNumber = ceil((RIGHT_T_EDGE - LEFT_T_EDGE) / t) + 1
	# Array for storing solution
	uExact = np.zeros((tNodesNumber, xNodesNumber))

	# Fill array
	for ti in range(tNodesNumber): 
		for xi in range(xNodesNumber):
			uExact[ti][xi] = u(LEFT_X_EDGE + xi * h, a, LEFT_T_EDGE + ti * t)

	return uExact

def main() -> None:
	# Parameters for different solutions
	xNodesNumberValues = [11, 101] # Number of nodes, so number of segments will be one less
	rValues = [0.25, 0.5, 1, 1.25] # Parameter for approximation
	aValues = [5]                  # "Speed" of function u(x, t)

	# Solve for every combination of parameters
	for combination in product(xNodesNumberValues, rValues, aValues):

		xNodesNumber = combination[0]
		r = combination[1]
		a = combination[2]

		print("Solving with parameters: xNodesNumber={}, r={}, a={}".format(
			xNodesNumber, r, a
		))

		uApprx = solve(xNodesNumber, r, a) # Get approximated solution
		uExact = exact(xNodesNumber, r, a) # Get exact solution
		
		print("\tSolved! Making GIF...")

		GIFFilename = "_".join([str(xNodesNumber - 1), str(r), str(a)])

		makeGIF(uApprx, uExact, GIFFilename)

main()