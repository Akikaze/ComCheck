///
/// \file std.hpp
/// \brief Standard file
/// \author David Feraud
/// \version 1
///

#ifndef STD_HPP
#define STD_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// --- ARRAY ---

///
/// \brief Display an array
/// \param os Output stream
/// \param array Array
/// \return Output stream which includes each array element
///
template < typename T, size_t N >
std::ostream &
operator<<
(
	std::ostream & os,
	const std::array< T, N > & array
)
{
	typename std::array< T, N >::const_iterator cit ;

	os << "---------" << std::endl ;

	for( cit = array.cbegin() ; cit != array.cend() ; ++cit )
	{
		os << *cit << std::endl ;
	}

	os << "---------" ;
	return os ;
}

///
/// \brief Operator +=
/// \param a1 First array for addition and compute result
/// \param a2 Second array for addition
///
template < typename T, size_t N >
std::array< T, N >
operator+=
(
	std::array< T, N > & a1,
	std::array< T, N > & a2
)
{
	for( size_t n = 0 ; n < N ; ++n )
	{
		a1[ n ] += a2[ n ] ;
	}
	
	return a1 ;
}

// --- POINTER ---

///
/// \brief Comparator for pointers
/// 
/// Define operator() in order to be used by std::sort
///
struct PointerComparator
{
	///
	/// \brief Operator() for pointers comparison
	/// \param l Left pointer
	/// \param r Right pointer
	/// \return True if l < r and false otherwise
	///
	template  < typename T >
	bool
	operator()
	(
		const T * l,
		const T * r
	)
	{
		return ( *l < *r ) ;
	}
} ;

// --- VECTOR ---

///
/// \brief Display a vector
/// \param os Output stream
/// \param vector Vector
/// \return Output stream which includes each vector element
///
template < typename T >
std::ostream &
operator<<
(
	std::ostream & os,
	const std::vector< T > & vector
)
{
	typename std::vector< T >::const_iterator cit ;

	os << "---------" << std::endl ;

	for( cit = vector.cbegin() ; cit != vector.cend() ; ++cit )
	{
		os << *cit << std::endl ;
	}

	os << "---------" ;
	return os ;
}

#endif // STD_HPP
