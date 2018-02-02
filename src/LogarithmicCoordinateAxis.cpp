#include "LogarithmicCoordinateAxis.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// public

MultiDimGrid::LogarithmicCoordinateAxis::LogarithmicCoordinateAxis (const double lowerCoordinateLimit, const double upperCoordinateLimit, const std::size_t intervalNumber) :
	CoordinateAxis(lowerCoordinateLimit, upperCoordinateLimit, intervalNumber),
	Coordinates(),
	IntegrationWeights()
{
	if ( LowerCoordinateLimit <= 0.0 )
	{
		std::cout << std::endl
				  << " MultiDimGrid::LogarithmicCoordinateAxis Error: Lower coordinate limit is not positive" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	LowerLogLimit = std::log10(lowerCoordinateLimit);
	UpperLogLimit = std::log10(upperCoordinateLimit);
	
	initialize_coordinates();
	initialize_integration_weights();
}

double MultiDimGrid::LogarithmicCoordinateAxis::coordinate_unchecked (const std::size_t axisPoint) const
{
	return Coordinates[axisPoint];
}

double MultiDimGrid::LogarithmicCoordinateAxis::integration_weight_unchecked (const std::size_t axisPoint) const
{
	return IntegrationWeights[axisPoint];
}

double MultiDimGrid::LogarithmicCoordinateAxis::interpolation_weight_unchecked (const double coord) const
{
	const double logarithmicCoordinate = std::log10(coord);	// to interpolate logarithmically, one has to perform a change of variables
	
	const double nearestSmallerLog = std::log10( coordinate_unchecked( nearest_lower_axis_point_unchecked(coord) ) );
	const double nearestLargerLog = std::log10( coordinate_unchecked( nearest_higher_axis_point_unchecked(coord) ) );
	
	if ( nearestSmallerLog < nearestLargerLog )
	{
		return (logarithmicCoordinate - nearestSmallerLog) / (nearestLargerLog - nearestSmallerLog);	// then one can easily evaluate the corresponding linear interpolation weight
	}
	else
	{
		return 0.0;
	}
}

std::size_t MultiDimGrid::LogarithmicCoordinateAxis::nearest_lower_axis_point_unchecked (const double coord) const
{
	const double logarithmicCoordinate = std::log10(coord);
	
	const double interpolatedAxisPoint = (logarithmicCoordinate - LowerLogLimit) / ( UpperLogLimit - LowerLogLimit ) * IntervalNumber;	// inverse the mapping from axis points to coordinate values
	
	return std::size_t( std::floor(interpolatedAxisPoint) );	// round down the interpolated axis point
}

std::size_t MultiDimGrid::LogarithmicCoordinateAxis::nearest_higher_axis_point_unchecked (const double coord) const
{
	const double logarithmicCoordinate = std::log10(coord);
	
	const double interpolatedAxisPoint = (logarithmicCoordinate - LowerLogLimit) / ( UpperLogLimit - LowerLogLimit ) * IntervalNumber;	// inverse the mapping from axis points to coordinate values
	
	return std::size_t( std::ceil(interpolatedAxisPoint) );	// round up the interpolated axis point
}

MultiDimGrid::LogarithmicCoordinateAxis* MultiDimGrid::LogarithmicCoordinateAxis::clone () const
{
	return new LogarithmicCoordinateAxis(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// protected

////////////////////////////////////////////////////////////////////////////////////////////////////
// private

void MultiDimGrid::LogarithmicCoordinateAxis::initialize_coordinates ()
{
	Coordinates.push_back( LowerCoordinateLimit ); // explicitly push back LowerCoordinateLimit to ensure that there are no rounding errors
		
	const double logarithmicSeparation = (UpperLogLimit - LowerLogLimit) / IntervalNumber;
	
	for ( std::size_t i_axisPoint = 1; i_axisPoint < IntervalNumber; ++i_axisPoint )
	{
		const double logarithmicCoordinate = LowerLogLimit + i_axisPoint * logarithmicSeparation;
		
		Coordinates.push_back( std::pow(10.0, logarithmicCoordinate) );
	}
	
	Coordinates.push_back( UpperCoordinateLimit ); // explicitly push back UpperCoordinateLimit to ensure that there are no rounding errors
}

void MultiDimGrid::LogarithmicCoordinateAxis::initialize_integration_weights ()
{
	const double trapezoidalQuadratureWeight = (UpperLogLimit - LowerLogLimit) / IntervalNumber;	// trapezoidal quadrature rule weight after change of variables
	
	for ( std::size_t i_axisPoint = 0; i_axisPoint < PointNumber; ++i_axisPoint )
	{
		IntegrationWeights.push_back( jacobi_determinant(i_axisPoint) * trapezoidalQuadratureWeight );	// to get the full integration weight at an axis point the trapezoidal quadrature rule weight has to be multiplied with the jacobi determinant at that point
	}
	
	IntegrationWeights[0] /= 2.0;	// the boundary points have half this weight
	IntegrationWeights[IntervalNumber] /= 2.0;
}

double MultiDimGrid::LogarithmicCoordinateAxis::jacobi_determinant (const std::size_t axisPoint) const
{
	return Coordinates[axisPoint] * std::log(10.0);
}