#include "../MultiDimGrid.h"

#include <cmath>
#include <iostream>

/**
 * MultiDimGrid demo:
 * 
 * Discretization of the function f(x) = f(x0,x1) = x0 * log(x1) in the range from 1 to 1000 in both coordinates with 10
 * axis intervals each.
 * 
 * MultiDimGrid allows to perform this discretization on differently spaced grids. Additionally, it is able to handle various
 * ways of implementing this function.
 * 
 * To get an idea of all these different possibilities, 3 exemplary cases are demonstrated:
 * 
 *  1) x0-spacing:     linear
 *     x1-spacing:     linear
 *     implementation: global function
 * 
 *  2) x0-spacing:     linear
 *     x1-spacing:     logarithmic
 *     implementation: function object
 * 
 *  3) x0-spacing:     logarithmic
 *     x1-spacing:     logarithmic
 *     implementation: member function
 * 
 * Once a function is discretized, MultiDimGrid provides four basic operations:
 *  - accessing the function value at a given grid point 
 *  - extracting the coordinates at a given grid point
 *  - extracting the integration weight at a given grid point
 *  - computing the interpolated function value at given coordinates
 * 
 * To demonstrate these operations, the coordinates as well as the function value at the grid point [2,7] are extracted,
 * and the interpolated function value at the coordinates (30,650) is computed. Furthermore, the integral of f(x) over the
 * specified coordinate range is estimated by iterating over all grid points and summing up the function values multiplied
 * by their respective integration weights.
 * 
 * As f is linear in x0 and logarithmic in x1, the interpolation in case 2 is exact while it has a small errror in cases
 * 1 and 3. On the other hand, in case 1 the rough estimate for the integral is closest to the actual value.
 */

double global_function_1 (const MultiDimGrid::Coordinates<2>& x)	// function implementation of test case 1
{
	const double x0 = x[0];
	const double x1 = x[1];
	
	return x0 * std::log(x1);
}

class FunctionObject2	// function implementation of test case 2
{
public:
	FunctionObject2 () {};
	
	double operator() (const MultiDimGrid::Coordinates<2>& x) const
	{
		const double x0 = x[0];
		const double x1 = x[1];
		
		return x0 * std::log(x1);
	};
};

class Class3	// function implementation of test case 3
{
public:
	Class3 () {};
	
	double member_function_3 (const MultiDimGrid::Coordinates<2>& x) const
	{
		const double x0 = x[0];
		const double x1 = x[1];
		
		return x0 * std::log(x1);
	};
};

double test_function (const MultiDimGrid::Coordinates<4>& x)	// function implementation of test case 1
{
	const double x0 = x[0];
	const double x1 = x[1];
	const double x2 = x[2];
	const double x3 = x[3];
	
	return x0 * x1 * x2 * x3;
}

int main()
{
	const double lowerCoordinateLimit = 1.0;	// lower limit of the coordinate range
	const double upperCoordinateLimit = 1000.0;	// upper limit of the coordinate range
	const std::size_t intervalNumber = 10;		// number of intervals
	
	const MultiDimGrid::LinearCoordinateAxis linAxis(lowerCoordinateLimit, upperCoordinateLimit, intervalNumber);		// linearly spaced coordinate axis
	const MultiDimGrid::LogarithmicCoordinateAxis logAxis(lowerCoordinateLimit, upperCoordinateLimit, intervalNumber);	// logarithmically spaced coordinate axis
	
	const MultiDimGrid::CoordinateAxisPointers<2> axes1 = {&linAxis, &linAxis};	// coordinate axes used in test case 1
	const MultiDimGrid::CoordinateAxisPointers<2> axes2 = {&linAxis, &logAxis};	// coordinate axes used in test case 2
	const MultiDimGrid::CoordinateAxisPointers<2> axes3 = {&logAxis, &logAxis};	// coordinate axes used in test case 3
	
	const FunctionObject2 funcObj2;	// instance of 'FunctionObject2' used in test case 2
	const Class3 instance3;			// instance of 'Class3' used in test case 3
	const MultiDimGrid::GridFunction<2> gridFunc1(axes1, global_function_1);						// constructing the discretized function used in test case 1
	const MultiDimGrid::GridFunction<2> gridFunc2(axes2, funcObj2);									// constructing the discretized function used in test case 2
	const MultiDimGrid::GridFunction<2> gridFunc3(axes3, &Class3::member_function_3, instance3);	// constructing the discretized function used in test case 3 (for member functions a different constructor syntax has to be used)
	
	const MultiDimGrid::GridPoint<2> g = {2, 7};			// grid point of interest
	const MultiDimGrid::Coordinates<2> x = {30.0, 650.0};	// coordinates of interest
	
	const MultiDimGrid::Coordinates<2> y1 = gridFunc1.coordinates(g);	// coordinates at the grid point of interest in test case 1
	const MultiDimGrid::Coordinates<2> y2 = gridFunc2.coordinates(g);	// coordinates at the grid point of interest in test case 2
	const MultiDimGrid::Coordinates<2> y3 = gridFunc3.coordinates(g);	// coordinates at the grid point of interest in test case 3
	
	const double f1_g = gridFunc1.value(g);	// function value at the grid point of interest in test case 1
	const double f2_g = gridFunc2.value(g);	// function value at the grid point of interest in test case 2
	const double f3_g = gridFunc3[g];		// function value at the grid point of interest in test case 3 (the bracket operator is equivalent to the method 'value')
	
	const double f1_x = gridFunc1.interpolate(x);	// interpolated function value at the coordinates of interest in test case 1
	const double f2_x = gridFunc2.interpolate(x);	// interpolated function value at the coordinates of interest in test case 2
	const double f3_x = gridFunc3(x);				// interpolated function value at the coordinates of interest in test case 3 (the parenthesis operator is equivalent to the method 'interpolate')
	
	double integral1 = 0.0;	// the estimated value of the integral in test case 1 will be written to this variable
	double integral2 = 0.0;	// the estimated value of the integral in test case 2 will be written to this variable
	double integral3 = 0.0;	// the estimated value of the integral in test case 3 will be written to this variable
	
	for ( std::size_t i_x0=0; i_x0<intervalNumber; ++i_x0)	// computing the estimated values of the integral of f(x)...
	{
		for ( std::size_t i_x1=0; i_x1<intervalNumber; ++i_x1)
		{
			const MultiDimGrid::GridPoint<2> g_i = {i_x0, i_x1};	// ...by iterating over all grid points...
			
			integral1 += gridFunc1.value(g_i) * gridFunc1.integration_weight(g_i);	// ...and summing up the function values multiplied by their respective integration weights in test case 1,...
			integral2 += gridFunc2.value(g_i) * gridFunc2.integration_weight(g_i);	// ...test case 2 and...
			integral3 += gridFunc3.value(g_i) * gridFunc3.integration_weight(g_i);	// ...test case 3
		}
	}
	
	std::cout << std::scientific	// write the coordinates as well as the function values at the grid point of interest, the interpolated as well as the exact function values at the coordinates of interest, and the estimated as well as the exact values of the integral of f(x) to the standard output
			  << std::endl
			  << "Test case 1: On the lin-lin grid the grid point of interest has the coordinates (" << y1[0] << ", " << y1[1] << ") and the function value " << f1_g << "." << std::endl
			  << "Test case 2: On the lin-log grid the grid point of interest has the coordinates (" << y2[0] << ", " << y2[1] << ") and the function value " << f2_g << "." << std::endl
			  << "Test case 3: On the log-log grid the grid point of interest has the coordinates (" << y3[0] << ", " << y3[1] << ") and the function value " << f3_g << "." << std::endl
			  << std::endl
			  << "Test case 1: On the lin-lin grid, the interpolated function value at the coordinates of interest is " << f1_x					<< "." << std::endl
			  << "Test case 2: On the lin-log grid, the interpolated function value at the coordinates of interest is " << f2_x					<< "." << std::endl
			  << "Test case 3: On the log-log grid, the interpolated function value at the coordinates of interest is " << f3_x					<< "." << std::endl
			  << "The exact function value at the coordinates of interest is                                          " << global_function_1(x)	<< "." << std::endl
			  << std::endl
			  << "Test case 1: On the lin-lin grid, the estimated value of the integral is " << integral1											<< "." << std::endl
			  << "Test case 2: On the lin-log grid, the estimated value of the integral is " << integral2											<< "." << std::endl
			  << "Test case 3: On the log-log grid, the estimated value of the integral is " << integral3											<< "." << std::endl
			  << "The exact value of the integral is                                       " << 999999.0/2.0 * (1000.0*std::log(1000.0) - 999.0)	<< "." << std::endl
			  << std::endl;
	
	return 0;
}