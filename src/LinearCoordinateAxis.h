#ifndef MULTIDIMGRID_LINEAR_COORDINATE_AXIS_H
#define MULTIDIMGRID_LINEAR_COORDINATE_AXIS_H

#include "CoordinateAxis.h"

#include <vector>

namespace MultiDimGrid
{
	/**
	 * \brief Class implementing a linearly spaced coordinate axis.
	 * 
	 * The interpolation weights correspond to a linear interpolation and the integration weights to a summed trapezoidal
	 * quadrature rule.
	 * 
	 * Author: Robert Lilow, ITA, ZAH, Heidelberg University (2016)
	 */
	class LinearCoordinateAxis : public CoordinateAxis
	{
	public:
		/**
		 * Constructor instantiating a linearly spaced coordinate axis with lower coordinate limit \a lowerCoordinateLimit,
		 * upper coordinate limit \a upperCoordinateLimit and number of axis intervals \a intervalNumber.
		 */
		LinearCoordinateAxis (double lowerCoordinateLimit, double upperCoordinateLimit, std::size_t intervalNumber);
		
		double coordinate_unchecked (std::size_t axisPoint) const;
		
		double integration_weight_unchecked (std::size_t axisPoint) const;
		
		double interpolation_weight_unchecked (double coord) const;
		
		std::size_t nearest_lower_axis_point_unchecked (double coord) const;
		
		std::size_t nearest_higher_axis_point_unchecked (double coord) const;
		
		LinearCoordinateAxis* clone () const;
	
	private:
		/**
		 * Vector containing all the coordinate values.
		 */
		std::vector<double> Coordinates;
		
		/**
		 * Vector containing the integration weights of each axis point.
		 */
		std::vector<double> IntegrationWeights;
		
		/**
		 * Initializes the values of the coordinates in LinearCoordinateAxis::Coordinates.
		 */
		void initialize_coordinates ();
		
		/**
		 * Initializes the values of the integration weights in LinearCoordinateAxis::IntegrationWeights.
		 */
		void initialize_integration_weights ();
	};
}

#endif