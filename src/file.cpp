///
/// \file file.cpp
/// \brief Source code for file object
/// \author David Feraud
/// \version 1
///

#include "file.hpp"

// --- CONSTRUCTORS ---

File::File
()
{
	
}

File::File
(
    const std::string & name
)
: name_( name )
, all_lines_( 0 )
, nbr_comments_( 0 )
, nbr_mix_( 0 )
, nbr_uncomments_( 0 )
{

}

// --- DESTRUCTORS ---

File::~File
()
{

}

// --- METHODS ---

void
File::store_info
(
	std::array< unsigned int, 4 > & array
)
{
    all_lines_ = array[ 0 ] ;
    nbr_comments_ = array[ 1 ] ;
    nbr_mix_ = array[ 2 ] ;
    nbr_uncomments_ = array[ 3 ] ;
}
