#ifndef STD_HPP
#define STD_HPP

/*
Include every Qt object that is required by ComCheck.
It could be not optimised but it's way simpler to use.

But this method increase the time for maintainance because we need to check
at every moment if the modification of a code requires a new include or if
we could withdraw an include without creating errors.
*/

#include <array>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

/*
This operator is going to be used when I would add two arrays where columns are identical.
It's going to simplify the gathering of every data from files in the report.

The operator just add data from a column of the second array in the first array at the same column.
It is used to create the report.
*/

///
/// \brief Operator += for array
/// \param a First array
/// \param b Second array
/// \return Array a that add b to a
///
template< class T, std::size_t N >
std::array< T, N > &
operator+=
(
	std::array< T, N > & a,
	std::array< T, N > & b
)
{
	for( unsigned int i = 0 ; i < N ; ++i )
	{
		a[ i ] += b[ i ] ;
	}

	return a ;
}


#endif // STD_HPP
