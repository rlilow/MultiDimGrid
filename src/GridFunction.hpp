#ifndef MULTIDIMGRID_GRID_FUNCTION_H
#define MULTIDIMGRID_GRID_FUNCTION_H

#include "CoordinateAxis.hpp"

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

namespace MultiDimGrid
{
	/**
	 * Arrays of integer values of length \a Length are implemented as \c std::size_t \c std::arrays.
	 */
	template <std::size_t Length>
	using IntegerArray = std::array<std::size_t, Length>;
	
	/**
	 * Arrays of real values of length \a Length are implemented as \c double \c std::arrays.
	 */
	template <std::size_t Length>
	using DoubleArray = std::array<double, Length>;
	
	/**
	 * A grid point is expected to be specified as a MultiDimGrid::IntegerArray<\a Dim>.
	 */
	template <std::size_t Dim>
	using GridPoint = IntegerArray<Dim>;
	
	/**
	 * Coordinates are expected to be specified as a MultiDimGrid::DoubleArray<\a Dim>.
	 */
	template <std::size_t Dim>
	using Coordinates = DoubleArray<Dim>;
	
	/**
	 * The coordinate axes of a grid are expected to be specified as a CoordinateAxis pointer \c std::array of length
	 * \a Dim.
	 */
	template <std::size_t Dim>
	using CoordinateAxisPointers = std::array<const CoordinateAxis*, Dim>;
	
	/**
	 * Functions that shall be discretized into a GridFunction are expected to be of this form: They depend on a reference
	 * to some MultiDimGrid::Coordinates<\a Dim> \a coords and return a \c double.
	 * 
	 * Note that the use of \c std::function allows to pass even function objects as well as lambda functions to a GridFunction
	 * as long as they are of this form.
	 */
	template <std::size_t Dim>
	using Function = std::function<double(const Coordinates<Dim>& coords)>;
	
	/**
	 * Pointers to member functions that shall be discretized into a GridFunction are expected to be of this form: They
	 * point to a method of some class \a Class which depends on a reference to some MultiDimGrid::Coordinates<\a Dim>
	 * \a coords and returns a \c double.
	 */
	template <std::size_t Dim, class Class>
	using MemberFunctionPointer = double(Class::*)(const Coordinates<Dim>& coords);
	
	/**
	 * Pointers to \c const member functions that shall be discretized into a GridFunction are expected to be of this
	 * form: They point to a \c const method of some class \a Class which depends on a reference to some MultiDimGrid::Coordinates<\a Dim>
	 * \a coords and returns a \c double.
	 */
	template <std::size_t Dim, class Class>
	using ConstMemberFunctionPointer = double(Class::*)(const Coordinates<Dim>& coords) const;
	
	/**
	 * \brief Class providing a discrete function defined on a multi-dimensional coordinate grid. 
	 *
	 * The grid is spanned up by an arbitrary number of coordinate axes with each axis having a custom range and spacing.
	 * It allows to read and modify the function value at each grid point individually and also is able to perform an interpolation
	 * of the discrete function values that is multi-linear in the coordinate spacings for any coordinates within the range
	 * of the grid. Furthermore, it allows to extract the coordinates and integration weights of each grid point.
	 * 
	 * Author: Robert Lilow (2016)
	 */
	template <std::size_t Dim>
	class GridFunction
	{
	public:
		/**
		 * Constructor instantiating a discrete function defined on a grid spanned up by several coordinate axes pointed
		 * to by the \a coordAxisPointers with constant function value \a funcValue.
		 */
		GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, double funcValue);
		
		/**
		 * Constructor instantiating a discrete function defined on a grid spanned up by several coordinate axes pointed
		 * to by the \a coordAxisPointers with the function value of each grid point set to the value of the MultiDimGrid::Function
		 * \a func at the coordinates of this grid point.
		 */
		GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const Function<Dim>& func);
		
		/**
		 * Constructor instantiating a discrete function defined on a grid spanned up by several coordinate axes pointed
		 * to by the \a coordAxisPointers with the function value of each grid point set to the value of some member
		 * function at the coordinates of this grid point. The member function is characterized by a MultiDimGrid::MemberFunctionPointer
		 * \a memberFuncPointer as well as an instance \a object of its host class \a Class.
		 */
		template <class Class>
		GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const MemberFunctionPointer<Dim, Class> memberFuncPointer, Class& object);
		
		/**
		 * Constructor instantiating a discrete function defined on a grid spanned up by several coordinate axes pointed
		 * to by the \a coordAxisPointers with the function value of each grid point set to the value of some \c const
		 * member function at the coordinates of this grid point. The \c const member function is characterized by a
		 * MultiDimGrid::ConstMemberFunctionPointer \a constMemberFuncPointer as well as a \c const instance \a constObject
		 * of its host class \a Class.
		 */
		template <class Class>
		GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const ConstMemberFunctionPointer<Dim, Class> constMemberFuncPointer, const Class& constObject);
		
		/**
		 * Copy-constructor taking care of properly copying the coordinate axes pointed to by the elements of
		 * GridFunction::CoordAxes from the GridFunction \a otherGridFunction.
		 */
		GridFunction (const GridFunction& otherGridFunction);
		
		/**
		 * Returns the coordinates of the grid point \a gridPoint.
		 */
		Coordinates<Dim> coordinates (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Returns the coordinates of the grid point \a gridPoint.
		 * 
		 * In contrast to GridFunction::coordinates, this method does not check if \a gridPoint is within the range of the
		 * grid. It is thus slightly faster, but unsafe!
		 */
		Coordinates<Dim> coordinates_unchecked (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Returns the coordinates of the grid point with index \a index.
		 */
		Coordinates<Dim> coordinates_at_index (std::size_t index) const;
		
		/**
		 * Returns the coordinates of the grid point with index \a index.
		 * 
		 * In contrast to GridFunction::coordinates_at_index, this method does not check if \a index is within the range
		 * of the grid. It is thus slightly faster, but unsafe!
		 */
		Coordinates<Dim> coordinates_at_index_unchecked (std::size_t index) const;
		
		/**
		 * Returns the integration weights corresponding to the individual coordinate axes at the grid point \a gridPoint.
		 */
		DoubleArray<Dim> integration_weights (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Returns the integration weights corresponding to the individual coordinate axes at the grid point \a gridPoint.
		 * 
		 * In contrast to GridFunction::integration_weight, this method does not check if \a gridPoint is within the range
		 * of the grid. It is thus slightly faster, but unsafe!
		 */
		DoubleArray<Dim> integration_weights_unchecked (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Returns the integration weights corresponding to the individual coordinate axes at the grid point with index
		 * \a index.
		 */
		DoubleArray<Dim> integration_weights_at_index (std::size_t index) const;
		
		/**
		 * Returns the integration weights corresponding to the individual coordinate axes at the grid point with index
		 * \a index.
		 * 
		 * In contrast to GridFunction::integration_weight_at_index, this method does not check if \a index is within the
		 * range of the grid. It is thus slightly faster, but unsafe!
		 */
		DoubleArray<Dim> integration_weights_at_index_unchecked (std::size_t index) const;
		
		/**
		 * Gives access to the function value at the grid point \a gridPoint by returning a \c reference to it.
		 * 
		 * Contrary to GridFunction::value(const GridPoint<Dim>& gridPoint) const, this allows to modify the function value
		 * and will be called if the instance of GridFunction is not \c const-qualified.
		 */
		double& value (const GridPoint<Dim>& gridPoint);
		
		/**
		 * Gives access to the function value at the grid point \a gridPoint by returning a \c const_reference to it.
		 * 
		 * Contrary to GridFunction::value(const GridPoint<Dim>& gridPoint), this does not allow to modify the function
		 * value and will be called if the instance of GridFunction is \c const-qualified.
		 */
		const double& value (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Gives access to the function value at the grid point \a gridPoint by returning a \c reference to it.
		 * 
		 * Contrary to GridFunction::value_unchecked(const GridPoint<Dim>& gridPoint) const, this allows to modify the function
		 * value and will be called if the instance of GridFunction is not \c const-qualified.
		 * 
		 * In contrast to GridFunction::value(const GridPoint<Dim>& gridPoint), this method does not check if \a gridPoint
		 * is within the range of the grid. It is thus slightly faster, but unsafe!
		 */
		double& value_unchecked (const GridPoint<Dim>& gridPoint);
		
		/**
		 * Gives access to the function value at the grid point \a gridPoint by returning a \c const_reference to it.
		 * 
		 * Contrary to GridFunction::value_unchecked(const GridPoint<Dim>& gridPoint), this does not allow to modify the
		 * function value and will be called if the instance of GridFunction is \c const-qualified.
		 * 
		 * In contrast to GridFunction::value(const GridPoint<Dim>& gridPoint) const, this method does not check if \a gridPoint
		 * is within the range of the grid. It is thus slightly faster, but unsafe!
		 */
		const double& value_unchecked (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Overloads the bracket operator to give access to the function value at the grid point \a gridPoint by returning
		 * a \c reference to it.
		 * 
		 * Contrary to GridFunction::operator[](const GridPoint<Dim>& gridPoint) const, this allows to modify the function
		 * value and will be called if the instance of GridFunction is not \c const-qualified.
		 * 
		 * This provides the same functionality as GridFunction::value(const GridPoint<Dim>& gridPoint).
		 */
		double& operator[] (const GridPoint<Dim>& gridPoint);
		
		/**
		 * Overloads the bracket operator to give access to the function value at the grid point \a gridPoint by returning
		 * a \c const_reference to it.
		 * 
		 * Contrary to GridFunction::operator[](const GridPoint<Dim>& gridPoint), this does not allow to modify the function
		 * value and will be called if the instance of GridFunction is \c const-qualified.
		 * 
		 * This provides the same functionality as GridFunction::value(const GridPoint<Dim>& gridPoint) const.
		 */
		const double& operator[] (const GridPoint<Dim>& gridPoint) const;
		
		/**
		 * Gives access to the function value at the grid point with index \a index by returning a \c reference to it.
		 * 
		 * Contrary to GridFunction::value_at_index(std::size_t index) const, this allows to modify the function value and
		 * will be called if the instance of GridFunction is not \c const-qualified.
		 */
		double& value_at_index (std::size_t index);
		
		/**
		 * Gives access to the function value at the grid point with index \a index by returning a \c const_reference to
		 * it.
		 * 
		 * Contrary to GridFunction::value_at_index(std::size_t index), this does not allow to modify the function value
		 * and will be called if the instance of GridFunction is \c const-qualified.
		 */
		const double& value_at_index (std::size_t index) const;
		
		/**
		 * Gives access to the function value at the grid point with index \a index by returning a reference to it.
		 * 
		 * Contrary to GridFunction::value_at_index_unchecked(std::size_t index) const, this allows to modify the function
		 * value and will be called if the instance of GridFunction is not \c const-qualified.
		 * 
		 * In contrast to GridFunction::value_at_index(std::size_t index), this method does not check if \a index is within
		 * the range of the grid. It is thus slightly faster, but unsafe!
		 */
		double& value_at_index_unchecked (std::size_t index);
		
		/**
		 * Gives access to the function value at the grid point with index \a index by returning a \c const_reference to
		 * it.
		 * 
		 * Contrary to GridFunction::value_at_index_unchecked(std::size_t index), this does not allow to modify the function
		 * value and will be called if the instance of GridFunction is \c const-qualified.
		 * 
		 * In contrast to GridFunction::value_at_index(std::size_t index) const, this method does not check if \a index
		 * is within the range of the grid. It is thus slightly faster, but unsafe!
		 */
		const double& value_at_index_unchecked (std::size_t index) const;
		
		/**
		 * Returns the interpolated function value of the discrete function at the coordinates \a coords.
		 */
		double interpolate (const Coordinates<Dim>& coords) const;
		
		/**
		 * Returns the interpolated function value of the discrete function at the coordinates \a coords.
		 * 
		 * In contrast to GridFunction::interpolate, this method does not check if \a coords is within the range of the
		 * grid. It is thus slightly faster, but unsafe!
		 */
		double interpolate_unchecked (const Coordinates<Dim>& coords) const;
		
		/**
		 * Overloads the paranthesis operator to return the interpolated function value of the discrete function at the
		 * coordinates \a coords.
		 * 
		 * This provides the same functionality as GridFunction::interpolate.
		 */
		double operator() (const Coordinates<Dim>& coords) const;
		
		/**
		 * Returns the index differences between neighbouring grid points along each coordinate axis.
		 * 
		 * This might be useful, if one wants to work with indices of grid points instead of the grid points themselves.
		 */
		IntegerArray<Dim> index_strides () const;
		
		/**
		 * Returns the total number of grid points.
		 * 
		 * This might be useful, if one wants to work with indices of grid points instead of the grid points themselves.
		 */
		double point_number () const;
		
		/**
		 * Assignment operator taking care of properly copying the coordinate axes pointed to by the elements of GridFunction::CoordAxes
		 * from the GridFunction \a otherGridFunction.
		 */
		GridFunction& operator= (const GridFunction& otherGridFunction);
		
		/**
		 * Destructor deleting the coordinate axes pointed to by the elements of GridFunction::CoordAxes.
		 */
		~GridFunction ();
		
	private:
		/**
		 * Coordinate axes spanning up the grid.
		 */
		CoordinateAxisPointers<Dim> CoordAxes;
		
		/**
		 * Index differences between neighbouring grid points along each coordinate axis.
		 * 
		 * If there are n axes, the indices of the grid points (i_0, ... , i_j, ... , i_(n-1)) and (i_0, ... , i_j + 1, ... , i_(n-1)),
		 * for example, differ by (num_axis_pts_(j+1) * ... * num_axis_pts_(n-1)). This value is stored in GridFunction::IndexStrides[j].
		 */
		IntegerArray<Dim> IndexStrides;
		
		/**
		 * Total number of grid points.
		 */
		std::size_t GridPointNumber;
		
		/**
		 * Internal 1-dimensional storage of the function values at each grid point.
		 * 
		 * The function values are stored in a nested fashion. If there are n axes, this means that the function value at
		 * the grid point (i_0, ... , i_(n-1)) is stored at the vector element with index (i_0 * num_axis_pts_1 * ... * num_axis_pts_(n-1) + ... + i_(n-2) * num_axis_pts_(n-1) + i_(n-1)).
		 */
		std::vector<double> FunctionValues;
		
		/**
		 * Creates copies of the coordinate axes pointed to by the \a coordAxisPointers and returns MultiDimGrid::CoordinateAxisPointers
		 * to them.
		 */
		CoordinateAxisPointers<Dim> copy_coordinate_axes (const CoordinateAxisPointers<Dim>& coordAxisPointers) const;
		
		/**
		 * Computes and returns the index stride values corresponding the coordinate axes pointed to by the \a coordAxisPointers.
		 */
		IntegerArray<Dim> compute_index_strides (const CoordinateAxisPointers<Dim>& coordAxisPointers) const;
		
		/**
		 * Implements the interpolation in a nested fashion by recursively calling itself. In each recursion step the 1-dimensional
		 * interpolation with respect to one of the coordinates \a coords is performed. \a i_axis denotes the nesting level,
		 * i.e. the axis along which the 1-dimensional interpolation is currently performed, and \a gridPoint stores the
		 * grid point from which the two neighbouring grid points lying on this axis are constructed.
		 */
		double internal_recursive_interpolation (const Coordinates<Dim>& coords, const GridPoint<Dim>& gridPoint, std::size_t i_axis) const;
		
		/**
		 * Implements the interpolation in a nested fashion by recursively calling itself. In each recursion step the 1-dimensional
		 * interpolation with respect to one of the coordinates \a coords is performed. \a i_axis denotes the nesting level,
		 * i.e. the axis along which the 1-dimensional interpolation is currently performed, and \a gridPoint stores the
		 * grid point from which the two neighbouring grid points lying on this axis are constructed.
		 * 
		 * In contrast to GridFunction::internal_recursive_interpolation, this method does not check if \a coords are within
		 * the range of the grid. It is thus slightly faster, but unsafe!
		 */
		double internal_recursive_interpolation_unchecked (const Coordinates<Dim>& coords, const GridPoint<Dim>& gridPoint, std::size_t i_axis) const;
		
		/**
		 * Checks if the axis point \a axisPoint of the coordinate axis pointed to by \a axis is out of range. If that is
		 * the case, an error message is written to the standard output and the program is terminated. The error message
		 * contains \a location, which specifies in which member the point range is checked.
		 */
		void check_axis_point (std::size_t axisPoint, const CoordinateAxis* axis, const char* location) const;
		
		/**
		 * Checks if the coordinate \a coord of the coordinate axis pointed to by \a axis is out of range. If that is the
		 * case, an error message is written to the standard output and the program is terminated. The error message contains
		 * \a location, which specifies in which member the coordinate range is checked.
		 */
		void check_coordinate (double coord, const CoordinateAxis* axis, const char* location) const;
		
		/**
		 * Checks if the index \a index is out of range. If that is the case, an error message is written to the standard
		 * output and the program is terminated. The error message contains \a location, which specifies in which member
		 * the index range is checked.
		 */
		void check_index (std::size_t index, const char* location) const;
	};
}

#include "GridFunction.tpp"	// template implementations can not be compiled separately

#endif