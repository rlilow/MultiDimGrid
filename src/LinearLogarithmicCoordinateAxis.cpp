#include "LinearLogarithmicCoordinateAxis.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// public

MultiDimGrid::LinearLogarithmicCoordinateAxis::LinearLogarithmicCoordinateAxis (const double lowerCoordinateLimit, const double spacingThresholdValue, const double upperCoordinateLimit, const std::size_t linearIntervalNumber, const std::size_t logarithmicIntervalNumber) :
	CoordinateAxis(lowerCoordinateLimit, upperCoordinateLimit, linearIntervalNumber+logarithmicIntervalNumber),
	SpacingThresholdValue(spacingThresholdValue),
	LinearIntervalNumber(linearIntervalNumber),
	LogarithmicIntervalNumber(logarithmicIntervalNumber),
	LinAxis(lowerCoordinateLimit, spacingThresholdValue, linearIntervalNumber),
	LogAxis(spacingThresholdValue, upperCoordinateLimit, logarithmicIntervalNumber)
{}

double MultiDimGrid::LinearLogarithmicCoordinateAxis::coordinate_unchecked (const std::size_t axisPoint) const
{
	if ( axisPoint > LinearIntervalNumber )
	{
		return LogAxis.coordinate_unchecked(axisPoint - LinearIntervalNumber);
	}
	else
	{
		return LinAxis.coordinate_unchecked(axisPoint);
	}
}

double MultiDimGrid::LinearLogarithmicCoordinateAxis::integration_weight_unchecked (const std::size_t axisPoint) const
{
	if ( axisPoint > LinearIntervalNumber )
	{
		return LogAxis.integration_weight_unchecked(axisPoint - LinearIntervalNumber);
	}
	else if ( axisPoint < LinearIntervalNumber )
	{
		return LinAxis.integration_weight_unchecked(axisPoint);
	}
	else
	{
		return LinAxis.integration_weight_unchecked(axisPoint) + LogAxis.integration_weight_unchecked(axisPoint - LinearIntervalNumber);
	}
}

double MultiDimGrid::LinearLogarithmicCoordinateAxis::interpolation_weight_unchecked (const double coord) const
{
	if ( coord > SpacingThresholdValue )
	{
		return LogAxis.interpolation_weight_unchecked(coord);
	}
	else
	{
		return LinAxis.interpolation_weight_unchecked(coord);
	}
}

std::size_t MultiDimGrid::LinearLogarithmicCoordinateAxis::nearest_lower_axis_point_unchecked (const double coord) const
{
	if ( coord > SpacingThresholdValue )
	{
		return LogAxis.nearest_lower_axis_point_unchecked(coord) + LinearIntervalNumber;
	}
	else
	{
		return LinAxis.nearest_lower_axis_point_unchecked(coord);
	}
}

std::size_t MultiDimGrid::LinearLogarithmicCoordinateAxis::nearest_higher_axis_point_unchecked (const double coord) const
{
	if ( coord > SpacingThresholdValue )
	{
		return LogAxis.nearest_higher_axis_point_unchecked(coord) + LinearIntervalNumber;
	}
	else
	{
		return LinAxis.nearest_higher_axis_point_unchecked(coord);
	}
}

MultiDimGrid::LinearLogarithmicCoordinateAxis* MultiDimGrid::LinearLogarithmicCoordinateAxis::clone () const
{
	return new LinearLogarithmicCoordinateAxis(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// protected

////////////////////////////////////////////////////////////////////////////////////////////////////
// private