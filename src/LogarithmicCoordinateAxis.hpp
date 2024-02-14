#ifndef MULTIDIMGRID_LOGARITHMIC_COORDINATE_AXIS_H
#define MULTIDIMGRID_LOGARITHMIC_COORDINATE_AXIS_H

#include "CoordinateAxis.hpp"

#include <vector>

namespace MultiDimGrid
{
	/**
	 * \brief Class implementing a logarithmically spaced coordinate axis.
	 * 
	 * The interpolation weights correspond to a logarithmic interpolation and the integration weights to a summed trapezoidal
	 * quadrature rule under the change of variables x -> log_10(x).
	 * 
	 * Author: Robert Lilow (2016)
	 */
	class LogarithmicCoordinateAxis : public CoordinateAxis
	{
	public:
		/**
		 * Constructor instantiating a logarithmically spaced coordinate axis with lower coordinate limit \a lowerCoordinateLimit,
		 * upper coordinate limit \a upperCoordinateLimit and number of axis intervals \a intervalNumber.
		 */
		LogarithmicCoordinateAxis (double lowerCoordinateLimit, double upperCoordinateLimit, std::size_t intervalNumber);
		
		double coordinate_unchecked (std::size_t axisPoint) const;
		
		double integration_weight_unchecked (std::size_t axisPoint) const;
		
		double interpolation_weight_unchecked (double coord) const;
		
		std::size_t nearest_lower_axis_point_unchecked (double coord) const;
		
		std::size_t nearest_higher_axis_point_unchecked (double coord) const;
		
		LogarithmicCoordinateAxis* clone () const;
	
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
		 * Logarithm of the lower coordinate limit.
		 */
		double LowerLogLimit;
		
		/**
		 * Logarithm of the upper coordinate limit.
		 */
		double UpperLogLimit;
		
		/**
		 * Initializes the values of the coordinates in LogarithmicCoordinateAxis::Coordinates.
		 */
		void initialize_coordinates ();
		
		/**
		 * Initializes the values of the integration weights in LogarithmicCoordinateAxis::IntegrationWeights.
		 */
		void initialize_integration_weights ();
		
		/**
		 * Returns the value of the jacobi determinant corresponding to the change of variables x -> log_10(x) at the axis
		 * point \a axisPoint.
		 */
		double jacobi_determinant (std::size_t axisPoint) const;
	};
}

#endif