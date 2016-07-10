///
/// \file file.cpp
/// \brief Source code for file object
/// \author David Feraud
/// \version 1
///

#include "file.hpp"

// --- CONSTRUCTORS ---

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
    const unsigned int & all_lines,
    const unsigned int & nbr_comments,
    const unsigned int & nbr_mix,
    const unsigned int & nbr_uncomments
)
{
    all_lines_ = all_lines ;
    nbr_comments_ = nbr_comments ;
    nbr_mix_ = nbr_mix ;
    nbr_uncomments_ = nbr_uncomments ;
}
