#include "SinglePointCoordinateAxis.hpp"

#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// public

MultiDimGrid::SinglePointCoordinateAxis::SinglePointCoordinateAxis (const double coordinate) :
	CoordinateAxis(coordinate, coordinate, 0),
	Coordinate(coordinate)
{}

double MultiDimGrid::SinglePointCoordinateAxis::coordinate_unchecked (const std::size_t axisPoint) const
{
	return Coordinate;
}

double MultiDimGrid::SinglePointCoordinateAxis::integration_weight_unchecked (const std::size_t axisPoint) const
{
	return 0.0;	// the integral over a single point vanishes
}

double MultiDimGrid::SinglePointCoordinateAxis::interpolation_weight_unchecked (const double coord) const
{
	return 0.0;	// the only valid 'coord' is the single axis point itself
}

std::size_t MultiDimGrid::SinglePointCoordinateAxis::nearest_lower_axis_point_unchecked (const double coord) const
{
	return 0;	// the only point on the axis
}

std::size_t MultiDimGrid::SinglePointCoordinateAxis::nearest_higher_axis_point_unchecked (const double coord) const
{
	return 0;	// the only point on the axis
}

MultiDimGrid::SinglePointCoordinateAxis* MultiDimGrid::SinglePointCoordinateAxis::clone () const
{
	return new SinglePointCoordinateAxis(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// protected

////////////////////////////////////////////////////////////////////////////////////////////////////
// private