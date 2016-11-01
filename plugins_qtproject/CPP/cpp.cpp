#include "cpp.hpp"

CPP_Plugin::CPP_Plugin
()
: block_comment_( false )
, block_description_( UNDEFINED )
{
	set_prefix() ;

	QStringList tmp = { "*.c", "*.C", "*.cpp", "*.h", "*.hpp" } ;

	extensions_ = tmp ;
	language_ = "C++" ;

	rank_ = USHRT_MAX - 1 ; // default value
}

CPP_Plugin::~CPP_Plugin
()
{

}

CC_Desc
CPP_Plugin::get_description
(
	const std::string & comment
)
{
	CC_Desc description = NORMAL ;

	if( !( prefix_.empty() ) )
	{
		QList< QPair< QString, CC_Desc > >::iterator itList ;
		std::string::iterator itString ;

		std::string copy = comment ;

		// erase /* or //
		if( ffo_iterator( copy, "/*" ) == copy.begin() ||
			ffo_iterator( copy, "//" ) == copy.begin() )
		{
			copy = copy.substr( 2 ) ;
		}

		// erase */
		if( ffo_iterator( copy, "*/" ) == copy.end() - 2 )
		{
			copy = copy.substr( 0, copy.length() - 2 ) ;
		}

		if( !( copy.empty() ) )
		{
			itList = prefix_.begin() ;

			while( itList != prefix_.end() && itString != copy.begin() )
			{
				itString = ffo_iterator( copy, ( *itList ).first.toStdString() ) ;

				if( itString != copy.begin() )
				{
					++itList ;
				}
			}

			if( itList != prefix_.end() )
			{
				description = ( *itList ).second ;
			}
		}
		else
		{
			// if the comment line is empty, it's a useless comment
			description = USELESS ;
		}

		// if the comment contain a ;, { or },  I consider that it is a dead code
		if( description == NORMAL &&
			( ffo_iterator( copy, ";" ) != copy.end() ||
			  ffo_iterator( copy, "{" ) != copy.end() ||
			  ffo_iterator( copy, "}" ) != copy.end() ) )
		{
			description = USELESS ;
		}
	}

	return description ;
}

CC_Line
CPP_Plugin::get_type
(
	const std::string & line
)
{
	CC_Line flag ;

	bool commented = false ; // signal presence of any type of comments

	std::string copy = line ;

	// line could not be empty, thus it's impossible that copy.begin() equal copy.end()
	std::string::iterator itStart = copy.begin() ;
	std::string::iterator itStop = copy.end() ;

	bool comment_started = block_comment_ ;
	bool comment_ended = false ;

	// bypass the search of block comments if a line comment is defined before
	if( block_comment_ == false &&
		ffo_iterator( copy, "//" ) < ffo_iterator( copy, "/*" ) )
	{
		itStart = copy.end() ;
	}

	while( itStart != copy.end() )
	{
		if( !( comment_started ) )
		{
			itStart = ffo_iterator( copy, "/*" ) ;
			comment_started = ( itStart != copy.end() ) ;
		}

		itStop = ffo_iterator( copy, "*/" ) ;
		if( itStop != copy.end() )
		{
			comment_ended = true ;
			itStop += 2 ; // point after the end balise
		}

		if( comment_started )
		{
			commented = true ;

			CC_Desc description = get_description( std::string( itStart, itStop ) ) ;

			if( block_description_ < HEADER || description == USELESS )
			{
				flag.description = description ;
			}
			else
			{
				flag.description = block_description_ ;
			}

			copy.erase( itStart, itStop ) ; // erase the comment
			block_comment_ = !( comment_ended ) ;

			if( comment_ended )
			{
				block_description_ = UNDEFINED ;
			}
			else
			{
				block_description_ = flag.description ;
			}
		}

		// reset signals
		comment_started = false ;
		comment_ended = false ;
	}

	itStart = ffo_iterator( copy, "//" ) ;
	if( itStart != copy.end() )
	{
		commented = true ;

		flag.description = get_description( std::string( itStart, copy.end() ) ) ;
		copy.erase( itStart, copy.end() ) ;
	}

	if( commented )
	{
		if( copy.empty() )
		{
			flag.type = ONLY_COMMENT ;
		}
		else
		{
			flag.type = MIX_LINE ;
		}
	}
	else
	{
		flag.type = NO_COMMENT ;
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

	position = line.find( string ) ;

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
