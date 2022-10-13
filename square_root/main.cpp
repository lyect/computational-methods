// ############################
// Find square root of a number
// ############################

#include <iostream>
#include <iomanip>
#include <cmath>

#define EPS 1e-9
#define MAX_STEPS 10000
#define PRINT_ERROR_DIGITS 9

/*
	This program solves following equation
		x*x - number = 0
	using iterative method in two different ways:
		1) x*x - number = 0
				|
				V
		   x*x = number
				|
				V
		   x = number / x

		2) x*x - number = 0
				|
				V
		   x*x = number
				|
				V
		   2*x*x = number + x*x
				|
				V
		   x = (number + x*x) / (2*x)
				|
				V
		   x = (number / x + x) / 2
*/

double f1(const double &x, const double &sv) {
	return sv / x;
}

double f2(const double &x, const double &sv) {
	return (sv / x + x) / 2;
}

// Function for comparing numbers and zero with error.
bool greaterThanZero(const double &x) {
	return x <= -EPS || x >= EPS;
}

int main() {
	
	// squaredValue is the value whose square root will be found.
	// startValue is the value from which the iteration will start.
	double squaredValue;
	double startValue;

	std::cout << "Enter the value whose square root you want to find:\n";
	std::cin >> squaredValue;

	std::cout << "Enter the value you want to start iterate from:\n";
	std::cin >> startValue;

	std::cout << "std::sqrt(" << squaredValue << ") = " << std::sqrt(squaredValue);
	std::cout << " -- the right answer.\n";

	bool methodDiverges;
	double x;
	double xPrev;
	double stepDelta;
	int stepCount; // Iteration step counting is needed because
				   //	iterative method may diverge.
	double x2;  // x which was found after 2 iterations.
	double x10; // x which was found after 10 iterations.

	// ################
	// # FIRST METHOD #
	// ################

	std::cout << "\n----- FIRST METHOD -----\n\n";

	methodDiverges = false;
	xPrev = startValue;
	stepDelta = 1; // With this, while loop will not end before it starts.
	stepCount = 0;

	while (greaterThanZero(stepDelta) && stepCount < MAX_STEPS) {
		x = f1(xPrev, squaredValue);

		if (std::abs(x) > std::abs(xPrev)) {
			methodDiverges = true;
		}

		stepDelta = xPrev - x;
		xPrev = x;

		++stepCount;

		if (stepCount == 2) {
			x2 = x;
		}
		if (stepCount == 10) {
			x10 = x;
		}
	}

	// Set precision.
	std::cout << std::fixed << std::setprecision(PRINT_ERROR_DIGITS);

	// Print first method's results.
	if (methodDiverges) {
		std::cout << "Bad start value or function - method does not converge!\n";
	}
	else {
		if (stepCount < 2) {
			std::cout << "x2 was not set, because first method ended after ";
			std::cout << stepCount;
			std::cout << " step(s).\n";
		}
		else {
			std::cout << "x2 = " << x2 << "\n";
		}

		if (stepCount < 10) {
			std::cout << "x10 was not set, because first method ended after ";
			std::cout << stepCount;
			std::cout << " step(s).\n";
		}
		else {
			std::cout << "x10 = " << x10 << "\n";
		}

		std::cout << "After " << stepCount << " step(s), this value was estimated:\n";
		std::cout << "\tsqrt(" << squaredValue << ") ~ " << x << "\n";
	}

	// #################
	// # SECOND METHOD #
	// #################

	std::cout << "\n----- SECOND METHOD -----\n\n";

	methodDiverges = false;
	x = startValue + 1;
	xPrev = startValue;
	stepDelta = 1;
	stepCount = 0;

	while (greaterThanZero(stepDelta) && stepCount < MAX_STEPS) {
		x = f2(xPrev, squaredValue);

		if (std::abs(x) > std::abs(xPrev)) {
			methodDiverges = true;
		}

		stepDelta = xPrev - x;
		xPrev = x;

		++stepCount;

		if (stepCount == 2) {
			x2 = x;
		}
		if (stepCount == 10) {
			x10 = x;
		}
	}

	// Print second method's results.
	if (methodDiverges) {
		std::cout << "Bad start value or function - method does not converge!\n";
	}
	else {
		std::cout << std::fixed << std::setprecision(PRINT_ERROR_DIGITS);

		if (stepCount < 2) {
			std::cout << "x2 was not set, because second method ended after ";
			std::cout << stepCount;
			std::cout << " step(s).\n";
		}
		else {
			std::cout << "x2 = " << x2 << "\n";
		}

		if (stepCount < 10) {
			std::cout << "x10 was not set, because second method ended after ";
			std::cout << stepCount;
			std::cout << " step(s).\n";
		}
		else {
			std::cout << "x10 = " << x10 << "\n";
		}

		std::cout << "After " << stepCount << " step(s), this value was estimated:\n";
		std::cout << "\tsqrt(" << squaredValue << ") ~ " << x << "\n";
	}
	return 0;
}