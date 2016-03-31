#ifndef MULTIDIMGRID_LINEAR_LOGARITHMIC_COORDINATE_AXIS_H
#define MULTIDIMGRID_LINEAR_LOGARITHMIC_COORDINATE_AXIS_H

#include "CoordinateAxis.h"
#include "LinearCoordinateAxis.h"
#include "LogarithmicCoordinateAxis.h"

namespace MultiDimGrid
{
	/**
	 * \brief Class implementing a partially linearly and partially logarithmically spaced coordinate axis.
	 * 
	 * It is linearly spaced for coordinate values up to some intermediate value and logarithmically spaced for larger coordinate
	 * values. This is specifically handy if one needs a logarithmic axis but has to extend it down to 0 or some negative
	 * coordinate value.
	 * 
	 * The interpolation and integration weights correspond to the ones described in LinearCoordinateAxis and LinearLogarithmicCoordinateAxis,
	 * respectively.
	 */
	class LinearLogarithmicCoordinateAxis : public CoordinateAxis
	{
	public:
		/**
		 * Constructor instantiating a coordinate axis that is linearly spaced between the lower coordinate limit \a lowerCoordinateLimit
		 * and the intermediate value \a spacingThresholdValue, and logarithmically spaced between this intermediate value
		 * and the upper coordinate limit \a upperCoordinateLimit. The number of axis intervals is \a linearIntervalNumber
		 * in the linear part and \a logarithmicIntervalNumber in the logarithmic part.
		 */
		LinearLogarithmicCoordinateAxis (double lowerCoordinateLimit, double spacingThresholdValue, double upperCoordinateLimit, std::size_t linearIntervalNumber, std::size_t logarithmicIntervalNumber);
		
		double coordinate_unchecked (std::size_t axisPoint) const;
		
		double integration_weight_unchecked (std::size_t axisPoint) const;
		
		double interpolation_weight_unchecked (double coord) const;
		
		std::size_t nearest_lower_axis_point_unchecked (double coord) const;
		
		std::size_t nearest_higher_axis_point_unchecked (double coord) const;
		
		LinearLogarithmicCoordinateAxis* clone () const;
	
	private:
		/**
		 * Coordinate value separating the linear from the logarithmic part of the axis.
		 */
		double SpacingThresholdValue;
		
		/**
		 * Number of linearly spaced axis intervals.
		 */
		size_t LinearIntervalNumber;
		
		/**
		 * Number of logarithmically spaced axis intervals.
		 */
		size_t LogarithmicIntervalNumber;
		
		/**
		 * Linear part of the axis.
		 */
		LinearCoordinateAxis LinAxis;
		
		/**
		 * Logarithmic part of the axis.
		 */
		LogarithmicCoordinateAxis LogAxis;
	};
}

#endif