#include "LinearCoordinateAxis.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// public

MultiDimGrid::LinearCoordinateAxis::LinearCoordinateAxis (const double lowerCoordinateLimit, const double upperCoordinateLimit, const std::size_t intervalNumber) :
	CoordinateAxis(lowerCoordinateLimit, upperCoordinateLimit, intervalNumber),
	Coordinates(),
	IntegrationWeights()
{
	if ( IntervalNumber == 0 )
	{
		std::cout << std::endl
				  << " MultiDimGrid::LinearCoordinateAxis Error: Number of axis intervals is zero" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	initialize_coordinates();
	initialize_integration_weights();
}

double MultiDimGrid::LinearCoordinateAxis::coordinate_unchecked (const std::size_t axisPoint) const
{
	return Coordinates[axisPoint];
}

double MultiDimGrid::LinearCoordinateAxis::integration_weight_unchecked (const std::size_t axisPoint) const
{
	return IntegrationWeights[axisPoint];
}

double MultiDimGrid::LinearCoordinateAxis::interpolation_weight_unchecked (const double coord) const
{
	const double nearestSmallerCoordinate = coordinate_unchecked( nearest_lower_axis_point_unchecked(coord) );
	const double nearestLargerCoordinate = coordinate_unchecked( nearest_higher_axis_point_unchecked(coord) );
	
	if ( nearestSmallerCoordinate < nearestLargerCoordinate )
	{
		return (coord - nearestSmallerCoordinate) / (nearestLargerCoordinate - nearestSmallerCoordinate);	// the linear interpolation weight is given by the coordinate distance to the nearest lower axis point relative to the coordinate distance between the two neighbouring axis points
	}
	else
	{
		return 0.0;
	}
}

std::size_t MultiDimGrid::LinearCoordinateAxis::nearest_lower_axis_point_unchecked (const double coord) const
{
	const double interpolatedAxisPoint = (coord - LowerCoordinateLimit) / ( UpperCoordinateLimit - LowerCoordinateLimit ) * IntervalNumber;	// inverse the mapping from axis points to coordinate values
	
	return std::size_t( std::floor(interpolatedAxisPoint) );	// round down the interpolated axis point
}

std::size_t MultiDimGrid::LinearCoordinateAxis::nearest_higher_axis_point_unchecked (const double coord) const
{
	const double interpolatedAxisPoint = (coord - LowerCoordinateLimit) / ( UpperCoordinateLimit - LowerCoordinateLimit ) * IntervalNumber;	// inverse the mapping from axis points to coordinate values
	
	return std::size_t( std::ceil(interpolatedAxisPoint) );	// round up the interpolated axis point
}

MultiDimGrid::LinearCoordinateAxis* MultiDimGrid::LinearCoordinateAxis::clone () const
{
	return new LinearCoordinateAxis(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// protected

////////////////////////////////////////////////////////////////////////////////////////////////////
// private

void MultiDimGrid::LinearCoordinateAxis::initialize_coordinates ()
{
	Coordinates.push_back( LowerCoordinateLimit ); // explicitly push back LowerCoordinateLimit to ensure that there are no rounding errors

	const double equidistantSeparation = (UpperCoordinateLimit - LowerCoordinateLimit) / IntervalNumber;
	
	for ( std::size_t i_axisPoint = 1; i_axisPoint < IntervalNumber; ++i_axisPoint )
	{
		Coordinates.push_back( LowerCoordinateLimit + i_axisPoint * equidistantSeparation );
	}

	Coordinates.push_back( UpperCoordinateLimit ); // explicitly push back UpperCoordinateLimit to ensure that there are no rounding errors
}

void MultiDimGrid::LinearCoordinateAxis::initialize_integration_weights ()
{
	const double trapezoidalQuadratureWeight = (UpperCoordinateLimit - LowerCoordinateLimit) / IntervalNumber;	// trapezoidal quadrature rule weight
	
	for ( std::size_t i_axisPoint = 0; i_axisPoint < PointNumber; ++i_axisPoint )
	{
		IntegrationWeights.push_back( trapezoidalQuadratureWeight );
	}
	
	IntegrationWeights[0] /= 2.0;	// the boundary points have half this weight
	IntegrationWeights[IntervalNumber] /= 2.0;
}