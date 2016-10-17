#include "cpp.hpp"

CPP_Plugin::CPP_Plugin
()
: comment_started_( false )
{
	QStringList tmp = { "*.c", "*.C", "*.cpp", "*.h", "*.hpp" } ;

	extensions_ = tmp ;
	language_ = "C++" ;

	rank_ = USHRT_MAX - 1 ; // default value
}

CPP_Plugin::~CPP_Plugin
()
{

}

CC_Line
CPP_Plugin::get_type
(
	const std::string & line
)
{
	CC_Line flag ;

	bool commented = false ;
	bool initialize_before = false ;

	std::string copy = line ;

	std::string::iterator itStart = copy.begin() ;
	std::string::iterator itStop = copy.end() ;

	if( comment_started_ == true )
	{
		comment_started_ = false ;
		initialize_before = true ;
	}

	while( itStart != itStop ) // erase all comments
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
				comment_started_ = true ; // signal a non-ended comment
			}

			commented = true ; // signal a comment
			copy.erase( itStart, itStop ) ; // erase a comment
		}
	}

	itStart = ffo_iterator( copy, "//" ) ; // line comment
	if( itStart != copy.end() )
	{
		commented = true ; // signal a comment
		copy.erase( itStart, copy.end() ) ;
	}

	if( copy.empty() )
	{
		flag.type = ONLY_COMMENT ; // commented
	}
	else
	{
		if( commented )
		{
			flag.type = MIX_LINE ; // mixed
		}
		else
		{
			flag.type = NO_COMMENT ; // uncommented
		}
	}

	return flag ;
}

std::string::iterator
CPP_Plugin::ffo_iterator
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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

	Q_EXPORT_PLUGIN2( plugin_CPP, CPP_Plugin )

#endif
