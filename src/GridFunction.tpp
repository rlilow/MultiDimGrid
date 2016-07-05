#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// public

template <std::size_t Dim>
MultiDimGrid::GridFunction<Dim>::GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const double funcValue) :
	CoordAxes(copy_coordinate_axes(coordAxisPointers)),
	IndexStrides(compute_index_strides(CoordAxes)),
	GridPointNumber( IndexStrides[0] * CoordAxes[0]->point_number() ),	// the 0-th index stride is given by the product of the numbers of points of all coordinate axes except for the 0-th
	FunctionValues(GridPointNumber, funcValue)	// assign the value 'funcValue' to every grid point
{
	static_assert(Dim != 0, "MultiDimGrid::GridFunction Error: Number of dimensions is zero");
}

template <std::size_t Dim>
MultiDimGrid::GridFunction<Dim>::GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const Function<Dim>& func) :
	CoordAxes(copy_coordinate_axes(coordAxisPointers)),
	IndexStrides(compute_index_strides(CoordAxes)),
	GridPointNumber( IndexStrides[0] * CoordAxes[0]->point_number() ),	// the 0-th index stride is given by the product of the numbers of points of all coordinate axes except for the 0-th
	FunctionValues()
{
	static_assert(Dim != 0, "MultiDimGrid::GridFunction Error: Number of dimensions is zero");
	
	for ( std::size_t index = 0; index < GridPointNumber; ++index )	// iterate through the whole index range, determine the corresponding coordinates for each index value, and evaluate 'func' at these coordinates
	{
		const Coordinates<Dim> coords = coordinates_at_index_unchecked(index);
		
		FunctionValues.push_back( func(coords) );
	}
}

template <std::size_t Dim>
template <class Class>
MultiDimGrid::GridFunction<Dim>::GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const MemberFunctionPointer<Dim, Class>  memberFuncPointer, Class& object) :
	CoordAxes(copy_coordinate_axes(coordAxisPointers)),
	IndexStrides(compute_index_strides(CoordAxes)),
	GridPointNumber( IndexStrides[0] * CoordAxes[0]->point_number() ),	// the 0-th index stride is given by the product of the numbers of points of all coordinate axes except for the 0-th
	FunctionValues()
{
	static_assert(Dim != 0, "MultiDimGrid::GridFunction Error: Number of dimensions is zero");
	
	for ( std::size_t index = 0; index < GridPointNumber; ++index ) // iterate through the whole index range, determine the corresponding coordinates for each index value, and evaluate 'object.*memberFuncPointer' at these coordinates
	{
		const Coordinates<Dim> coords = coordinates_at_index_unchecked(index);
		
		FunctionValues.push_back( (object.*memberFuncPointer)(coords) );
	}
}

template <std::size_t Dim>
template <class Class>
MultiDimGrid::GridFunction<Dim>::GridFunction (const CoordinateAxisPointers<Dim>& coordAxisPointers, const ConstMemberFunctionPointer<Dim, Class>  constMemberFuncPointer, const Class& constObject) :
	CoordAxes(copy_coordinate_axes(coordAxisPointers)),
	IndexStrides(compute_index_strides(CoordAxes)),
	GridPointNumber( IndexStrides[0] * CoordAxes[0]->point_number() ),	// the 0-th index stride is given by the product of the numbers of points of all coordinate axes except for the 0-th
	FunctionValues()
{
	static_assert(Dim != 0, "MultiDimGrid::GridFunction Error: Number of dimensions is zero");
	
	for ( std::size_t index = 0; index < GridPointNumber; ++index ) // iterate through the whole index range, determine the corresponding coordinates for each index value, and evaluate 'object.*memberFuncPointer' at these coordinates
	{
		const Coordinates<Dim> coords = coordinates_at_index_unchecked(index);
		
		FunctionValues.push_back( (constObject.*constMemberFuncPointer)(coords) );
	}
}

template <std::size_t Dim>
MultiDimGrid::GridFunction<Dim>::GridFunction (const GridFunction& otherGridFunction) :
	CoordAxes(copy_coordinate_axes(otherGridFunction.CoordAxes)),
	IndexStrides(otherGridFunction.IndexStrides),
	GridPointNumber(otherGridFunction.GridPointNumber),
	FunctionValues(otherGridFunction.FunctionValues)
{
	static_assert(Dim != 0, "MultiDimGrid::GridFunction Error: Number of dimensions is zero");
}

template <std::size_t Dim>
MultiDimGrid::Coordinates<Dim> MultiDimGrid::GridFunction<Dim>::coordinates (const GridPoint<Dim>& gridPoint) const
{
	Coordinates<Dim> coords;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		check_axis_point(axisPoint, axis, "coordinates");
		
		coords[i_axis] = axis->coordinate_unchecked(axisPoint);	// for each axis read off the coordinate of the corresponding axis point
	}
	
	return coords;
}

template <std::size_t Dim>
MultiDimGrid::Coordinates<Dim> MultiDimGrid::GridFunction<Dim>::coordinates_unchecked (const GridPoint<Dim>& gridPoint) const
{
	Coordinates<Dim> coords;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		coords[i_axis] = axis->coordinate_unchecked(axisPoint);	// for each axis read off the coordinate of the corresponding axis point
	}
	
	return coords;
}

template <std::size_t Dim>
MultiDimGrid::Coordinates<Dim> MultiDimGrid::GridFunction<Dim>::coordinates_at_index (const std::size_t index) const
{
	check_index(index, "coordinates_at_index");
	
	return coordinates_at_index_unchecked(index);
}

template <std::size_t Dim>
MultiDimGrid::Coordinates<Dim> MultiDimGrid::GridFunction<Dim>::coordinates_at_index_unchecked (const std::size_t index) const
{
	Coordinates<Dim> coords;
	
	std::size_t reducedIndex = index;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )	// if the index is divided by the outermost (i_axis=0) stride, the result is given by the value of the outermost axis point plus a rest that is smaller than one, but integer division in C++ omits this rest automatically; if one then subtracts the product of this axis point value and its stride to get an reduced index, this procedure can be repeated for the inner axes
	{
		const std::size_t stride = IndexStrides[i_axis];
		
		const std::size_t axisPoint = reducedIndex / stride;
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		coords[i_axis] = axis->coordinate_unchecked(axisPoint);	// read off the coordinate of the corresponding axis point
		
		reducedIndex -= axisPoint * stride;
	}
	
	return coords;
}

template <std::size_t Dim>
MultiDimGrid::DoubleArray<Dim> MultiDimGrid::GridFunction<Dim>::integration_weights (const GridPoint<Dim>& gridPoint) const
{
	DoubleArray<Dim> weights;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		check_axis_point(axisPoint, axis, "integration_weight");
		
		weights[i_axis] = axis->integration_weight_unchecked(axisPoint);	// read off the integration weight of the corresponding axis point
	}
	
	return weights;
}

template <std::size_t Dim>
MultiDimGrid::DoubleArray<Dim> MultiDimGrid::GridFunction<Dim>::integration_weights_unchecked (const GridPoint<Dim>& gridPoint) const
{
	DoubleArray<Dim> weights;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		weights[i_axis] = axis->integration_weight_unchecked(axisPoint);	// read off the integration weight of the corresponding axis point
	}
	
	return weights;
}

template <std::size_t Dim>
MultiDimGrid::DoubleArray<Dim> MultiDimGrid::GridFunction<Dim>::integration_weights_at_index (const std::size_t index) const
{
	check_index(index, "integration_weight_at_index");
	
	return integration_weights_at_index_unchecked(index);
}

template <std::size_t Dim>
MultiDimGrid::DoubleArray<Dim> MultiDimGrid::GridFunction<Dim>::integration_weights_at_index_unchecked (const std::size_t index) const
{
	DoubleArray<Dim> weights;
	
	std::size_t reducedIndex = index;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )	// if the index is divided by the outermost (i_axis=0) stride, the result is given by the value of the outermost axis point plus a rest that is smaller than one, but integer division in C++ omits this rest automatically; if one then subtracts the product of this axis point value and its stride to get an reduced index, this procedure can be repeated for the inner axes
	{
		const std::size_t stride = IndexStrides[i_axis];
		
		const std::size_t axisPoint = reducedIndex / stride;
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		weights[i_axis] = axis->integration_weight_unchecked(axisPoint);	// read off the integration weight of the corresponding axis point
		
		reducedIndex -= axisPoint * stride;
	}
	
	return weights;
}

template <std::size_t Dim>
double& MultiDimGrid::GridFunction<Dim>::value (const GridPoint<Dim>& gridPoint)
{
	std::size_t index = 0;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		check_axis_point(axisPoint, axis, "value");
		
		index += axisPoint * IndexStrides[i_axis];	// the index associated to a grid point is found by multiplying all its individual axis point numbers with their corresponding index stride values and summing up the results
	}
	
	return FunctionValues[index];
}

template <std::size_t Dim>
const double& MultiDimGrid::GridFunction<Dim>::value (const GridPoint<Dim>& gridPoint) const
{
	std::size_t index = 0;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		const CoordinateAxis* axis = CoordAxes[i_axis];
		
		check_axis_point(axisPoint, axis, "value");
		
		index += axisPoint * IndexStrides[i_axis];	// the index associated to a grid point is found by multiplying all its individual axis point numbers with their corresponding index stride values and summing up the results
	}
	
	return FunctionValues[index];
}

template <std::size_t Dim>
double& MultiDimGrid::GridFunction<Dim>::value_unchecked (const GridPoint<Dim>& gridPoint)
{
	std::size_t index = 0;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		index += axisPoint * IndexStrides[i_axis];	// the index associated to a grid point is found by multiplying all its individual axis point numbers with their corresponding index stride values and summing up the results
	}
	
	return FunctionValues[index];
}

template <std::size_t Dim>
const double& MultiDimGrid::GridFunction<Dim>::value_unchecked (const GridPoint<Dim>& gridPoint) const
{
	std::size_t index = 0;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )
	{
		const std::size_t axisPoint = gridPoint[i_axis];
		
		index += axisPoint * IndexStrides[i_axis];	// the index associated to a grid point is found by multiplying all its individual axis point numbers with their corresponding index stride values and summing up the results
	}
	
	return FunctionValues[index];
}

template <std::size_t Dim>
double& MultiDimGrid::GridFunction<Dim>::operator[] (const GridPoint<Dim>& gridPoint)
{
	return value(gridPoint);
}

template <std::size_t Dim>
const double& MultiDimGrid::GridFunction<Dim>::operator[] (const GridPoint<Dim>& gridPoint) const
{
	return value(gridPoint);
}

template <std::size_t Dim>
double& MultiDimGrid::GridFunction<Dim>::value_at_index (const std::size_t index)
{
	check_index(index, "value_at_index");
	
	return value_at_index_unchecked(index);
}

template <std::size_t Dim>
const double& MultiDimGrid::GridFunction<Dim>::value_at_index (const std::size_t index) const
{
	check_index(index, "value_at_index");
	
	return value_at_index_unchecked(index);
}

template <std::size_t Dim>
double& MultiDimGrid::GridFunction<Dim>::value_at_index_unchecked (const std::size_t index)
{
	return FunctionValues[index];
}

template <std::size_t Dim>
const double& MultiDimGrid::GridFunction<Dim>::value_at_index_unchecked (const std::size_t index) const
{
	return FunctionValues[index];
}

template <std::size_t Dim>
double MultiDimGrid::GridFunction<Dim>::interpolate (const Coordinates<Dim>& coords) const
{
	const GridPoint<Dim> gridPoint = {};
	
	return internal_recursive_interpolation(coords, gridPoint, Dim);	// to interpolate the grid function, one has to average over its values at all the 2^'Dim' grid points nearest to 'coords', using the appropriate interpolation weights; these points and the weights are constructed recursively, starting from an initial default point 'gridPoint'
}

template <std::size_t Dim>
double MultiDimGrid::GridFunction<Dim>::interpolate_unchecked (const Coordinates<Dim>& coords) const
{
	const GridPoint<Dim> gridPoint = {};
	
	return internal_recursive_interpolation_unchecked(coords, gridPoint, Dim);	// to interpolate the grid function, one has to average over its values at all the 2^'Dim' grid points nearest to 'coords', using the appropriate interpolation weights; these points and the weights are constructed recursively, starting from an initial default point 'gridPoint'
}

template <std::size_t Dim>
double MultiDimGrid::GridFunction<Dim>::operator() (const Coordinates<Dim>& coords) const
{
	return interpolate(coords);
}

template <std::size_t Dim>
MultiDimGrid::IntegerArray<Dim> MultiDimGrid::GridFunction<Dim>::index_strides () const
{
	return IndexStrides;
}

template <std::size_t Dim>
double MultiDimGrid::GridFunction<Dim>::point_number () const
{
	return GridPointNumber;
}

template <std::size_t Dim>
MultiDimGrid::GridFunction<Dim>& MultiDimGrid::GridFunction<Dim>::operator= (const GridFunction& otherGridFunction)
{
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )	// iterate through the different coordinate axes and assign each of them to the corresponding axis of 'otherGridFunction'
	{
		*(CoordAxes[i_axis]) = *(otherGridFunction.CoordAxes[i_axis]);
	}
	
	IndexStrides = otherGridFunction.IndexStrides;
	GridPointNumber = otherGridFunction.GridPointNumber;
	FunctionValues = otherGridFunction.FunctionValues;
	
	return *this;
}

template <std::size_t Dim>
MultiDimGrid::GridFunction<Dim>::~GridFunction ()
{
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )	// iterate through the different coordinate axes and delete each of them
	{
		delete CoordAxes[i_axis];
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// protected

////////////////////////////////////////////////////////////////////////////////////////////////////
// private

template <std::size_t Dim>
MultiDimGrid::CoordinateAxisPointers<Dim> MultiDimGrid::GridFunction<Dim>::copy_coordinate_axes (const MultiDimGrid::CoordinateAxisPointers<Dim>& coordAxisPointers) const
{
	CoordinateAxisPointers<Dim> coordAxes;
	
	for ( std::size_t i_axis = 0; i_axis < Dim; ++i_axis )	// iterate through the different coordinate axes and copy them using the 'CoordinateAxis::clone' method
	{
		coordAxes[i_axis] = coordAxisPointers[i_axis]->clone();
	}
	
	return coordAxes;
}

template <std::size_t Dim>
MultiDimGrid::IntegerArray<Dim> MultiDimGrid::GridFunction<Dim>::compute_index_strides (const MultiDimGrid::CoordinateAxisPointers<Dim>& coordAxisPointers) const
{
	IntegerArray<Dim> indexStrides;
	
	indexStrides[Dim-1] = 1;	// stride of the innermost coordinate in the nested 1-dimensional storage is '1'
	
	for ( std::size_t i_axis_reverse = 2; i_axis_reverse <= Dim; ++i_axis_reverse )	// iterate through the remaining coordinate axes in reverse order
	{
		const std::size_t i_axis = Dim - i_axis_reverse;
		
		indexStrides[i_axis] = indexStrides[i_axis + 1] * coordAxisPointers[i_axis + 1]->point_number();	// the corresponding strides are found iteratively: the stride of any coordinate is given by the stride of the coordinate one nesting level deeper times the number of points of that cordinate's axis
	}
	
	return indexStrides;
}

template <std::size_t Dim>
double MultiDimGrid::GridFunction<Dim>::internal_recursive_interpolation (const Coordinates<Dim>& coords, const GridPoint<Dim>& gridPoint, const std::size_t i_axis) const
{
	if ( i_axis > 0 )	// all the neighbouring grid points of 'coords' are constructed by recursively building up a binary tree with each recursion/branching level corresponding to one of the 'Dim' axes
	{
		const std::size_t i_nextAxis = i_axis - 1;
		
		const double coord = coords[i_nextAxis];
		
		const CoordinateAxis* axis = CoordAxes[i_nextAxis];
		
		check_coordinate(coord, axis, "interpolate");
		
		GridPoint<Dim> lowerGridPoint = gridPoint;		// in the recursion step with index 'i_axis' the grid point of the parent node (whose axis points on the axes with indices larger or equal to 'i_axis' are already fixed) is used to construct two child nodes...
		GridPoint<Dim> higherGridPoint = gridPoint;
		
		lowerGridPoint[i_nextAxis] = axis->nearest_lower_axis_point_unchecked(coord);	// ...by setting their axis points on the axis with index 'i_axis-1' to the nearest lower and higher axis points of the corresponding coordinate 'coord', respectively
		higherGridPoint[i_nextAxis] = axis->nearest_higher_axis_point_unchecked(coord);
		
		const double lowerValue = internal_recursive_interpolation(coords, lowerGridPoint, i_nextAxis);	// then, the values at these two points are found by performing the same procedure on the next recursion level
		const double higherValue = internal_recursive_interpolation(coords, higherGridPoint, i_nextAxis);
		
		const double interpolationWeight = axis->interpolation_weight_unchecked(coord);
		
		return lowerValue * (1.0 - interpolationWeight) + higherValue * interpolationWeight;	// these values and the interpolation weight corresponding to the current axis and coordinate are then used to perform a one dimensional interpolation
	}
	else
	{
		return value(gridPoint);	// at the innermost recursion level, when all axis points of a grid point are constructed, the value at this grid point is returned
	}
}

template <std::size_t Dim>
double MultiDimGrid::GridFunction<Dim>::internal_recursive_interpolation_unchecked (const Coordinates<Dim>& coords, const GridPoint<Dim>& gridPoint, const std::size_t i_axis) const
{
	if ( i_axis > 0 )	// all the neighbouring grid points of 'coords' are constructed by recursively building up a binary tree with each recursion/branching level corresponding to one of the 'Dim' axes
	{
		const std::size_t i_nextAxis = i_axis - 1;
		
		const double coord = coords[i_nextAxis];
		
		const CoordinateAxis* axis = CoordAxes[i_nextAxis];
		
		GridPoint<Dim> lowerGridPoint = gridPoint;		// in the recursion step with index 'i_axis' the grid point of the parent node (whose axis points on the axes with indices larger or equal to 'i_axis' are already fixed) is used to construct two child nodes...
		GridPoint<Dim> higherGridPoint = gridPoint;
		
		lowerGridPoint[i_nextAxis] = axis->nearest_lower_axis_point_unchecked(coord);	// ...by setting their axis points on the axis with index 'i_axis-1' to the nearest lower and higher axis points of the corresponding coordinate 'coord', respectively
		higherGridPoint[i_nextAxis] = axis->nearest_higher_axis_point_unchecked(coord);
		
		const double lowerValue = internal_recursive_interpolation_unchecked(coords, lowerGridPoint, i_nextAxis);	// then, the values at these two points are found by performing the same procedure on the next recursion level
		const double higherValue = internal_recursive_interpolation_unchecked(coords, higherGridPoint, i_nextAxis);
		
		const double interpolationWeight = axis->interpolation_weight_unchecked(coord);
		
		return lowerValue * (1.0 - interpolationWeight) + higherValue * interpolationWeight;	// these values and the interpolation weight corresponding to the current axis and coordinate are then used to perform a one dimensional interpolation
	}
	else
	{
		return value(gridPoint);	// at the innermost recursion level, when all axis points of a grid point are constructed, the value at this grid point is returned
	}
}

template <std::size_t Dim>
void MultiDimGrid::GridFunction<Dim>::check_axis_point (const std::size_t axisPoint, const CoordinateAxis* axis, const char* location) const
{
	if ( (axisPoint < 0) || (axisPoint >= axis->point_number()) )
	{
		std::cout << std::endl
				  << " MultiDimGrid::GridFunction::" + std::string(location) + " Error: Point not within range of grid" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

template <std::size_t Dim>
void MultiDimGrid::GridFunction<Dim>::check_coordinate (const double coord, const CoordinateAxis* axis, const char* location) const
{
	if ( (coord < axis->lower_coordinate_limit()) || (coord > axis->upper_coordinate_limit()) )
	{
		std::cout << std::endl
				  << " MultiDimGrid::GridFunction::" + std::string(location) + " Coordinates not within range of grid" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

template <std::size_t Dim>
void MultiDimGrid::GridFunction<Dim>::check_index (const std::size_t index, const char* location) const
{
	if ( index >= GridPointNumber )
	{
		std::cout << std::endl
				  << " MultiDimGrid::GridFunction::" + std::string(location) + " Index not within range of grid" << std::endl
				  << std::endl;
		
		exit(EXIT_FAILURE);
	}
}