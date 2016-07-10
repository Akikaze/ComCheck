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
		os << std::distance( array.cbegin(), cit ) << " : "  << *cit << std::endl ;
	}

	os << "---------" ;
	return os ;
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
		os << std::distance( vector.cbegin(), cit ) << " : " << *cit << std::endl ;
	}

	os << "---------" ;
	return os ;
}

#endif // STD_HPP
