#include "C.hpp"

extern "C"
IPlugin *
load
()
{
	return new C ;
}

C::C
()
: IPlugin()
, comment_started( false )
{
	
}

C::~C
()
{
}

unsigned short
C::analyze
(
	const std::string & line
)
{
	bool commented = false ;
	bool initialize_before = false ;
	
	unsigned short result ;
	std::string copy = line ;
	
	std::string::iterator itStart = copy.begin() ;
	std::string::iterator itStop = copy.end() ;
	
	if( comment_started == true )
	{
		comment_started = false ;
		initialize_before = true ;
	}
	
	while( itStart != itStop )
	{
		if( initialize_before == false )
		{
			itStart = ffo_iterator( copy, "/*" ) ;
		}
		else
		{
			initialize_before = false ;
		}
		
		itStop = ffo_iterator( copy, "*/" ) ;
		
		if( itStart != itStop )
		{
			if( itStop != copy.end() )
			{
				itStop += 2 ;
			}
			else
			{
				comment_started = true ; // signal a non-ended comment
			}
			
			commented = true ; // signal a comment
			copy.erase( itStart, itStop ) ; // erase a comment
		}
	}
	
	if( copy.empty() )
	{
		result = 1 ; // commented
	}
	else
	{
		if( commented )
		{
			result = 2 ; // mixed
		}
		else
		{
			result = 3 ; // uncommented
		}
	}
	
	return result ;
}

std::string::iterator
C::ffo_iterator
(
	std::string & line,
	const std::string & string
)
const
{
	std::string::iterator it ;
	size_t position ;
	
	position = line.find_first_of( string ) ;
	
	if( position < line.length() )
	{
		it = line.begin() + position ;
	}
	else
	{
		it = line.end() ;
	}
	
	return it ;
}
