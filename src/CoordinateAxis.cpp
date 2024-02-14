#include "CoordinateAxis.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
// public

double MultiDimGrid::CoordinateAxis::coordinate (const std::size_t axisPoint) const
{
	check_axis_point(axisPoint, "coordinate");
	
	return coordinate_unchecked(axisPoint);
}

double MultiDimGrid::CoordinateAxis::operator[] (const std::size_t axisPoint) const
{
	return coordinate(axisPoint);
}

double MultiDimGrid::CoordinateAxis::integration_weight (const std::size_t axisPoint) const
{
	check_axis_point(axisPoint, "integration_weight");
	
	return integration_weight_unchecked(axisPoint);
}

double MultiDimGrid::CoordinateAxis::interpolation_weight (const double coord) const
{
	check_coordinate(coord, "interpolation_weight");
	
	return interpolation_weight_unchecked(coord);
}

std::size_t MultiDimGrid::CoordinateAxis::nearest_lower_axis_point (const double coord) const
{
	check_coordinate(coord, "nearest_lower_axis_point");
	
	return nearest_lower_axis_point_unchecked(coord);
}

std::size_t MultiDimGrid::CoordinateAxis::nearest_higher_axis_point (const double coord) const
{
	check_coordinate(coord, "nearest_higher_axis_point");
	
	return nearest_higher_axis_point_unchecked(coord);
}

double MultiDimGrid::CoordinateAxis::lower_coordinate_limit () const
{
	return LowerCoordinateLimit;
}

double MultiDimGrid::CoordinateAxis::upper_coordinate_limit () const
{
	return UpperCoordinateLimit;
}

std::size_t MultiDimGrid::CoordinateAxis::interval_number () const
{
	return IntervalNumber;
}

std::size_t MultiDimGrid::CoordinateAxis::point_number () const
{
	return PointNumber;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// protected

MultiDimGrid::CoordinateAxis::CoordinateAxis (const double lowerCoordinateLimit, const double upperCoordinateLimit, const std::size_t intervalNumber) :
	LowerCoordinateLimit(lowerCoordinateLimit),
	UpperCoordinateLimit(upperCoordinateLimit),
	IntervalNumber(intervalNumber),
	PointNumber(IntervalNumber + 1)
{
	if ( UpperCoordinateLimit < LowerCoordinateLimit )
	{
		std::cout << std::endl
				  << " MultiDimGrid::CoordinateAxis Error: Upper coordinate limit is smaller than lower coordinate limit" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	if ( IntervalNumber < 0 )
	{
		std::cout << std::endl
				  << " MultiDimGrid::CoordinateAxis Error: Number of axis intervals is negative" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	if ( (PointNumber == 1) && (LowerCoordinateLimit != UpperCoordinateLimit) )
	{
		std::cout << std::endl
				  << " MultiDimGrid::CoordinateAxis Error: For a single-point axis the lower and upper coordinate limits have to agree" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	if ( (PointNumber != 1) && (LowerCoordinateLimit == UpperCoordinateLimit) )
	{
		std::cout << std::endl
				  << " MultiDimGrid::CoordinateAxis Error: For an axis with more than one axis point the upper coordinate limit has to be larger than the lower coordinate limit." << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

void MultiDimGrid::CoordinateAxis::check_axis_point (const std::size_t axisPoint, const char* location) const
{
	if ( (axisPoint < 0) || (axisPoint >= PointNumber) )
	{
		std::cout << std::endl
				  << " MultiDimGrid::CoordinateAxis::" + std::string(location) + " Error: Point not within range of axis" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

void MultiDimGrid::CoordinateAxis::check_coordinate (const double coord, const char* location) const
{
	if ( (coord < LowerCoordinateLimit) || (coord > UpperCoordinateLimit) )
	{
		std::cout << std::endl
				  << " MultiDimGrid::CoordinateAxis::" + std::string(location) + " Error: Coordinate not within range of axis" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// private