#ifndef MULTIDIMGRID_COORDINATE_AXIS_H
#define MULTIDIMGRID_COORDINATE_AXIS_H

#include <cstddef>

namespace MultiDimGrid
{
	/**
	 * \brief Abstract base class for coordinate axes.
	 * 
	 * A coordinate axis consists out of a fixed number of axis points describing specifically spaced coordinate values
	 * in a certain range.
	 * 
	 * This class allows to extract the coordinate of each axis point and is able to determine the axis points nearest
	 * to some coordinate within the range of the axis. Furthermore, it provides the interpolation weights corresponding
	 * to an interpolation linear in the coordinate spacing as well as suitable integration weights.
	 * 
	 * Coordinate axes with specific coordinate spacings are implemented as classes derived from this base class.
	 * 
	 * Author: Robert Lilow, ITA, ZAH, Heidelberg University (2016)
	 */
	class CoordinateAxis
	{
	public:
		/**
		 * Returns the coordinate of the axis point \a axisPoint.
		 */
		double coordinate (std::size_t axisPoint) const;
		
		/**
		 * Returns the coordinate of the axis point \a axisPoint.
		 * 
		 * In contrast to CoordinateAxis::coordinate, this method does not check if \a axisPoint is within the range of
		 * the axis. It is thus slightly faster, but unsafe!
		 */
		virtual double coordinate_unchecked (std::size_t axisPoint) const = 0;
		
		/**
		 * Overloads the bracket operator to return the coordinate of the axis point \a axisPoint.
		 *
		 * This provides the same functionality as CoordinateAxis::coordinate.
		 */
		double operator[] (std::size_t axisPoint) const;
		
		/**
		 * Returns the integration weight of the axis point \a axisPoint.
		 */
		double integration_weight (std::size_t axisPoint) const;
		
		/**
		 * Returns the integration weight of the axis point \a axisPoint.
		 * 
		 * In contrast to CoordinateAxis::integration_weight, this method does not check if \a axisPoint is within the range
		 * of the axis. It is thus slightly faster, but unsafe!
		 */
		virtual double integration_weight_unchecked (std::size_t axisPoint) const = 0;
		
		/**
		 * Returns the interpolation weight of the coordinate \a coord corresponding to an interpolation linear in the
		 * coordinate spacing.
		 */
		double interpolation_weight (double coord) const;
		
		/**
		 * Returns the interpolation weight of the coordinate \a coord corresponding to an interpolation linear in the
		 * coordinate spacing.
		 * 
		 * In contrast to CoordinateAxis::interpolation_weight, this method does not check if \a coord is within the range
		 * of the axis. It is thus slightly faster, but unsafe!
		 */
		virtual double interpolation_weight_unchecked (double coord) const = 0;
		
		/**
		 * Returns the nearest axis point that has a coordinate smaller than or equal to \a coord.
		 */
		std::size_t nearest_lower_axis_point (double coord) const;
		
		/**
		 * Returns the nearest axis point that has a coordinate smaller than or equal to \a coord.
		 * 
		 * In contrast to CoordinateAxis::nearest_lower_axis_point, this method does not check if \a coord is within the
		 * range of the axis. It is thus slightly faster, but unsafe!
		 */
		virtual std::size_t nearest_lower_axis_point_unchecked (double coord) const = 0;
		
		/**
		 * Returns the nearest axis point that has a coordinate larger than or equal to \a coord.
		 */
		std::size_t nearest_higher_axis_point (double coord) const;
		
		/**
		 * Returns the nearest axis point that has a coordinate larger than or equal to \a coord.
		 * 
		 * In contrast to CoordinateAxis::nearest_higher_axis_point, this method does not check if \a coord is within the
		 * range of the axis. It is thus slightly faster, but unsafe!
		 */
		virtual std::size_t nearest_higher_axis_point_unchecked (double coord) const = 0;
		
		/**
		 * Returns the lower coordinate limit of the axis.
		 */
		double lower_coordinate_limit () const;
		
		/**
		 * Returns the upper coordinate limit of the axis.
		 */
		double upper_coordinate_limit () const;
		
		/**
		 * Returns the number of axis intervals.
		 */
		std::size_t interval_number () const;
		
		/**
		 * Returns the number of axis points.
		 */
		std::size_t point_number () const;
		
		/**
		 * Dynamically creates a copy of this coordinate axis, using the copy-constructor, and returns a pointer to
		 * it.
		 * 
		 * This is needed to create a copy of some specific coordinate axis derived from this base class given only
		 * a pointer to a general CoordinateAxis.
		 */
		virtual CoordinateAxis* clone () const = 0;
		
		/**
		 * Default destructor.
		 * 
		 * It is virtual to make sure that you can delete an instance of a specific coordinate axis derived from this
		 * base class through a pointer to a general CoordinateAxis.
		 */
		virtual ~CoordinateAxis () = default;
	
	protected:
		/**
		 * Constructor instantiating a general coordinate axis with lower coordinate limit \a lowerCoordinateLimit,
		 * upper coordinate limit \a upperCoordinateLimit and number of axis intervals \a intervalNumber.
		 */
		CoordinateAxis (double lowerCoordinateLimit, double upperCoordinateLimit, std::size_t intervalNumber);
		
		/**
		 * Default copy-constructor.
		 */
		CoordinateAxis (const CoordinateAxis& otherCoordinateAxis) = default;

        /**
		 * Default assignment operator.
		 */
        CoordinateAxis &operator=(const CoordinateAxis &otherCoordinateAxis) = default;

        /**
		 * Lower coordinate limit.
		 */
		double LowerCoordinateLimit;
		
		/**
		 * Upper coordinate limit.
		 */
		double UpperCoordinateLimit;
		
		/**
		 * Number of axis intervals.
		 */
		std::size_t IntervalNumber;
		
		/**
		 * Number of axis points.
		 */
		std::size_t PointNumber;
		
		/**
		 * Checks if the axis point \a axisPoint is out of range. If that is the case, an error message is written to the
		 * standard output and the program is terminated. The error message contains \a location, which specifies in which
		 * member the point range is checked.
		 */
		void check_axis_point (std::size_t axisPoint, const char* location) const;
		
		/**
		 * Checks if the coordinate \a coord is out of range. If that is the case, an error message is written to the standard
		 * output and the program is terminated. The error message contains \a location, which specifies in which member
		 * the coordinate range is checked.
		 */
		void check_coordinate (double coord, const char* location) const;
	};
}

#endif