// #####################
// Cubic equation solver
// #####################

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <utility>

#define EPS 1e-9
#define PRINT_ERROR_DIGITS 5

double a = 0;
double b = 0;
double c = 0;

// Cubic equation in general form represented as
//		x^3 + a * x^2 + b * x + c
// Its derivative can be written as
//		3 * x^2 + 2a * x + b
double function(const double &x) {
	return x * x * x + a * x * x + b * x + c;
}


// Functions for comparing numbers and zero with error

bool lessThanZero(const double &x) {
	return x <= -EPS;
}

bool equalToZero(const double &x) {
	return -EPS < x && x < EPS;
}

bool greaterThanZero(const double &x) {
	return x >= EPS;
}

// Get rid of -0.00000
double prettify(const double &x){
	double print_error = std::pow(10, -PRINT_ERROR_DIGITS);
	if (-print_error < x && x < print_error) {
		return 0.0;
	}
	return x;
}

// Binary searching u such that f(u) = v.
// And u located somewhere on (l; r).
// Function is increasing!
double binarySearchIncreasingF0(double l, double r, const double &v){
	while (l <= r - EPS){ // while l < r

		double m = (l + r) / 2.0;
		if (equalToZero(function(m))) return m;

		if (greaterThanZero(function(m))){
			r = m;
		}
		else {
			l = m;
		}
	}

	return (l + r) / 2;
}

// Binary searching u such that f(u) = v.
// And u located somewhere on (l; r).
// Function is decreasing!
double binarySearchDecreasingF0(double l, double r, const double &v){
	while (l <= r - EPS){ // while l < r

		double m = (l + r) / 2.0;
		if (equalToZero(function(m))) return m;

		if (greaterThanZero(function(m))){
			l = m;
		}
		else {
			r = m;
		}
	}

	return (l + r) / 2;
}

// Binary searching u such that f(u) = v.
// And u located somewhere on (-inf; r).
// Function is increasing!
double lBinarySearchF0(const double &r, const double &v) {

	// trying to find a number where function <= 0
	double l = std::min(r, -1.0); // -1.0 may be greater than r. It might cause errors.
	while (greaterThanZero(function(l))){
		l *= 2.0;
	}

	return binarySearchIncreasingF0(l, r, v);
}

// Binary searching u such that f(u) = v.
// And u located somewhere on (l; +inf).
// Function is increasing!
double rBinarySearchF0(const double &l, const double &v) {

	// trying to find a number where function >= 0
	double r = std::max(l, 1.0); // 1.0 may be less than l. It might cause errors.
	while (lessThanZero(function(r))){
		r *= 2.0;
	}

	return binarySearchIncreasingF0(l, r, v);
}

int main() {

	std::cout << "Enter the coefficients a, b, c:\n";
	std::cin >> a >> b >> c;

	std::vector<std::pair<double, int>> roots;

	// Descriminant of the derivative.
	double derivative_D = (2 * a) * (2 * a) - 4 * 3 * b;

	// Descriminant < 0 => number of roots = 0 => function only increases.
	// 		OR
	// Descriminant = 0 => function increases almost everywhere (it has a "step").
	//
	// So, there is only one root (since function is polynomial, 
	//  intersection with X axis is guaranteed).

	if (lessThanZero(derivative_D) || equalToZero(derivative_D)) {

		double f0 = function(0.0);

		// Root is located somewhere on (0; +inf)
		if (lessThanZero(f0)) {
			roots.push_back({rBinarySearchF0(0.0, 0.0), 3});
		}

		// Root is located somewhere on (-inf; 0)
		if (greaterThanZero(f0)) {
			roots.push_back({lBinarySearchF0(0.0, 0.0), 3});
		}

		// 0 is a root
		if (equalToZero(f0)) {
			roots.push_back({0.0, 3});
		}
	}

	if (greaterThanZero(derivative_D)) {
		// "alpha" and "beta" (alpha < beta) are points of function fracture.
		// These solutions are made from derivative coefficients.
		const double alpha = (-2 * a - std::sqrt(derivative_D)) / (2 * 3);
		const double beta  = (-2 * a + std::sqrt(derivative_D)) / (2 * 3);

		const double fa = function(alpha);
		const double fb = function(beta);
		/*
		1 root

		   ^
		   |                  /
		   |                 /
		   |        _       /
		   |       /^\     /
		   |      / | \   /
		   |     /  |  \_/
		   |    /   |   ^
		   |   /    |   |
		---+--@-----|---|------>
           | /   alpha beta
           |/
		*/

		if (greaterThanZero(fa) && greaterThanZero(fb)) {
			roots.push_back({lBinarySearchF0(alpha, 0.0), 1});
		}

		/*
		2 roots

		   ^
		   |                  /
		   |                 /
		   |        _       /
		   |       /^\     /
		   |      / | \   /
		   |     /  |  \ /
		---+----@-------@------>
		   |   /    |   ^
		   |  /     |   |
           | /   alpha beta
           |/
		*/

		if (greaterThanZero(fa) && equalToZero(fb)) {
			roots.push_back({lBinarySearchF0(alpha, 0.0), 1});
			roots.push_back({beta, 2});
		}

		// Due to calculations with an error, it may turn out that
		// 	function(alpha) == 0 AND function(beta) == 0
		if (equalToZero(fa) && equalToZero(fb)) {
			roots.push_back({(alpha + beta) / 2, 1});
		}

		/*
		3 roots

		   ^
		   |                  /
		   |                 /
		   |        _       /
		   |       /^\     /
		---+------@-|-@---@---->
		   |     /  |  \_/
		   |    /   |   ^
		   |   /    |   |
		   |  /     |   |
           | /   alpha beta
           |/
		*/

		if (greaterThanZero(fa) && lessThanZero(fb)) {
			roots.push_back({lBinarySearchF0(alpha, 0.0), 1});
			roots.push_back({binarySearchDecreasingF0(alpha, beta, 0.0), 1});
			roots.push_back({rBinarySearchF0(beta, 0.0), 1});
		}

		/*
		2 roots

		   ^
		   |                  /
		   |                 /
		---+--------@-------@-->
		   |       /^\     /
		   |      / | \   /
		   |     /  |  \_/
		   |    /   |   ^
		   |   /    |   |
		   |  /     |   |
           | /   alpha beta
           |/
		*/

		if (equalToZero(fa) && lessThanZero(fb)) {
			roots.push_back({alpha, 2});
			roots.push_back({rBinarySearchF0(beta, 0.0), 1});
		}

		/*
		1 root

		   ^
		   |                  /
		---+-----------------@->
		   |        _       /
		   |       /^\     /
		   |      / | \   /
		   |     /  |  \_/
		   |    /   |   ^
		   |   /    |   |
		   |  /     |   |
           | /   alpha beta
           |/
		*/

		if (lessThanZero(fa) && lessThanZero(fb)) {
			roots.push_back({rBinarySearchF0(beta, 0.0), 1});
		}
	}

	std::cout << "Found " << roots.size() << " root(s):\n";

	std::cout << std::fixed << std::setprecision(PRINT_ERROR_DIGITS);
	for (auto root : roots) {
		double pretty_root = prettify(root.first);
		int multiplicity = root.second;
		std::cout << pretty_root << ", multiplicity = " << multiplicity << "\n";
	}

	return 0;
}
