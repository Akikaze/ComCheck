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

	return description ;
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

	if( block_comment_ == true )
	{
		block_comment_ = false ;
		initialize_before = true ;
	}
	else
	{
		block_description_ = UNDEFINED ;
	}

	while( itStart != itStop ) // erase all block comments
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
				block_comment_ = true ; // signal a non-ended comment
			}

			commented = true ; // signal a comment

			flag.description = get_description( std::string( itStart, itStop ) ) ;
			copy.erase( itStart, itStop ) ; // erase a comment

			if( block_description_ != UNDEFINED )
			{
				if( flag.description == NORMAL )
				{
					flag.description = block_description_ ;
				}
			}

			if( block_comment_ == true )
			{
				block_description_ = flag.description ;
			}
		}
	}

	itStart = ffo_iterator( copy, "//" ) ; // line comment
	if( itStart != copy.end() )
	{
		commented = true ; // signal a comment

		flag.description = get_description( std::string( itStart, copy.end() ) ) ;
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
