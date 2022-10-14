// #########################
// Goofy integral calculator
// #########################

#include <iostream>
#include <iomanip>
#include <cmath>

// e^x
long double f(const long double &x){
	return std::exp(x);
}

// This function returns calculated definite integral of e^x
//	using trapezoidal rule
long double TrapezoidalRule(const long double &a, const long double &b, const int &interval_number) {
	long double definite_integral = 0;

	long double step = (b - a) / interval_number;
	for (long double x = a; x < b; x += step) {

		// Bounds of the trapeze
		long double x1 = x;
		long double x2 = x + step;

		// Area of the trapeze
		long double step_trapeze = step * (f(x1) + f(x2)) / 2;

		definite_integral += step_trapeze;
	}

	return definite_integral;
}

// This function returns calculated definite integral of e^x
//	using Simpson's rule
long double SimpsonsRule(const long double &a, const long double &b, const int &interval_number) {
	long double definite_integral = 0;

	long double step = (b - a) / interval_number;
	for (long double x = a; x < b; x += step) {

		// Bounds of the segment
		long double x1 = x;
		long double x2 = x + step;

		// Area of that segment
		long double step_segment = step * (f(x1) + 4 * f((x1 + x2) / 2) + f(x2)) / 6;

		definite_integral += step_segment;
	}

	return definite_integral;
}

long double get_trapeze_error_power(const long double &a, const long double &b, const int &interval_number) {

	long double true_integral = f(b) - f(a);

	long double full_step_integral = TrapezoidalRule(a, b, interval_number);
	long double half_step_integral = TrapezoidalRule(a, b, 2 * interval_number);

	long double full_step_error = full_step_integral - true_integral;
	long double half_step_error = half_step_integral - true_integral;

	return std::log2(full_step_error / half_step_error);
}

long double get_simpson_error_power(const long double &a, const long double &b, const int &interval_number) {

	long double true_integral = f(b) - f(a);

	long double full_step_integral = SimpsonsRule(a, b, interval_number);
	long double half_step_integral = SimpsonsRule(a, b, 2 * interval_number);

	long double full_step_error = std::abs(full_step_integral - true_integral);
	long double half_step_error = std::abs(half_step_integral - true_integral);

	return std::log2(full_step_error / half_step_error);
}

int main() {
	
	long double a = 0;
	long double b = 10;

	int interval_number;

	// Set precision of the output
	std::cout << std::fixed << std::setprecision(9);

	std::cout << "True integral:\n\t" << f(b) - f(a) << "\n";

	interval_number = 100;
	std::cout << "Integration from " << a << " to " << b << " with 100 intervals:\n";
	std::cout << "\tTrapeze: " << TrapezoidalRule(a, b, interval_number) << "\n";
	std::cout << "\tSimpson: " << SimpsonsRule(a, b, interval_number) << "\n";

	interval_number = 1000;
	std::cout << "Integration from " << a << " to " << b << " with 1000 intervals:\n";
	std::cout << "\tTrapeze: " << TrapezoidalRule(a, b, interval_number) << "\n";
	std::cout << "\tSimpson: " << SimpsonsRule(a, b, interval_number) << "\n";

	// Integration steps are changed fro error's power computation.
	// Because "long double" is too imprecise for these calculations.

	// Trapezoidal rule error
	std::cout << "Power of the trapezoidal rule error with 100 intervals:\n";
	std::cout << "\t" << get_trapeze_error_power(a, b, interval_number) << "\n";
	std::cout << "Power of the trapezoidal rule error with 1000 intervals:\n";
	std::cout << "\t" << get_trapeze_error_power(a, b, interval_number) << "\n";

	// Simpson's rule error
	interval_number = 100;
	std::cout << "Power of the Simpson's rule error with 100 intervals:\n";
	std::cout << "\t" << get_simpson_error_power(a, b, interval_number) << "\n";
	interval_number = 1000;
	std::cout << "Power of the Simpson's rule error with 1000 intervals:\n";
	std::cout << "\t" << get_simpson_error_power(a, b, interval_number) << "\n";

	return 0;
}