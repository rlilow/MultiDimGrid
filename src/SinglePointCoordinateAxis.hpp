#ifndef MULTIDIMGRID_SINGLE_POINT_COORDINATE_AXIS_H
#define MULTIDIMGRID_SINGLE_POINT_COORDINATE_AXIS_H

#include "CoordinateAxis.hpp"

namespace MultiDimGrid
{
	/**
	 * \brief Class implementing a single-point coordinate axis.
	 * 
	 * This can be used in scenarios where a generic CoordinateAxis is expected though one is only interested in a single
	 * coordinate value.
	 * 
	 * Author: Robert Lilow (2016)
	 */
	class SinglePointCoordinateAxis : public CoordinateAxis
	{
	public:
		/**
		 * Constructor instantiating a single-axis-point coordinate axis with the coordinate value \a coordinate.
		 */
		SinglePointCoordinateAxis (double coordinate);
		
		double coordinate_unchecked (std::size_t axisPoint) const;
		
		double integration_weight_unchecked (std::size_t axisPoint) const;
		
		double interpolation_weight_unchecked (double coord) const;
		
		std::size_t nearest_lower_axis_point_unchecked (double coord) const;
		
		std::size_t nearest_higher_axis_point_unchecked (double coord) const;
		
		SinglePointCoordinateAxis* clone () const;
	
	private:
		/**
		 * The single coordinate value of the axis.
		 */
		double Coordinate;
	};
}

#endif