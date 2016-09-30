#include "console_ui.hpp"

unsigned int ConsoleUI::_cols_ = 0 ;
unsigned int ConsoleUI::_rows_ = 0 ;

#ifdef Q_OS_UNIX
struct winsize ConsoleUI::_w_ ;
#endif

#ifdef Q_OS_WIN
CONSOLE_SCREEN_BUFFER_INFO ConsoleUI::_csbi_ = CONSOLE_SCREEN_BUFFER_INFO() ;
#endif

ConsoleUI::ConsoleUI
(
	QObject * core,
	bool welcomed
)
: IUI( core )
, buffer_( QStringList() )
, current_folder_( nullptr )
, current_report_( nullptr )
, welcomed_( welcomed )
{

#ifdef Q_OS_UNIX
	// clean the console
	// system( "clear" ) ;

	// handle the signal of WINdow CHange
	signal( SIGWINCH, UNIX_console_size ) ;
#endif

#ifdef Q_OS_WIN
	// clean the console
	// system( "cls" ) ;
#endif

	console_size() ;

	if( _cols_ == 0 || _rows_ == 0 )
	{
		std::cout << "Impossible to get console size." << std::endl ;
		std::cout << "By default, the size will be 80x60." << std::endl ;

		_cols_ = 80 ;
		_rows_ = 60 ;
	}
}

ConsoleUI::~ConsoleUI
()
{

}

QString
ConsoleUI::align_line
(
	QString line,
	CUI_TextAlignment alignment
)
{
	if( alignment != CUI_LEFT )
	{
		QString blank = "" ;
		unsigned int spaces = _cols_ - line.length() ;

		if( alignment == CUI_CENTER )
		{
			spaces /= 2 ;
		}

		for( unsigned int i = 0 ; i < spaces ; ++i )
		{
			blank += ' ' ;
		}

		if( alignment == CUI_CENTER )
		{
			line = blank + line + blank ;
		}
		else
		{
			line = blank + line ;
		}
	}

	return line ;
}

void
ConsoleUI::bufferize_text
(
	QString text
)
{
	if( text.isEmpty() )
	{
		buffer_.push_back( "" ) ;
	}
	else
	{
		unsigned int pos_string = 0 ;

		if( ( unsigned int )( text.length() ) > _cols_ )
		{
			while( ( unsigned int )( text.length() ) > _cols_ )
			{
				unsigned int pos_real = 0 ;
				unsigned int pos_space = 0 ;
				pos_string = 0 ;

				while( pos_string < ( unsigned int )( text.length() ) && pos_real < _cols_ )
				{
					if( text[ pos_string ] == 27 )
					{
						do
						{
							++pos_string ;
						}
						while( text[ pos_string ] != QChar( '#' ) ) ;

						text.remove( pos_string, 1 ) ;
					}
					else
					{
						if( text[ pos_string ] == QChar( ' ' ) )
						{
							pos_space = pos_string ;
						}
					}

					++pos_real ;
					++pos_string ;
				}

				// weird case when the text line is bigger than possible but the real line is shorter
				if( pos_string == ( unsigned int )( text.length() ) && pos_real < _cols_ )
				{
					// it can appear if we use ton of colors in the same line
					break ;
				}

				buffer_.push_back( text.left( pos_space ) ) ;
				text.remove( 0, pos_space + 1 ) ;
			}
		}
		else
		{
			while( pos_string < ( unsigned int )( text.length() ) )
			{
				if( text[ pos_string ] == QChar( '#' ) )
				{
					text.remove( pos_string, 1 ) ;
				}
				else
				{
					++pos_string ;
				}
			}
		}

		buffer_.push_back( text ) ;
	}
}

void
ConsoleUI::bufferize_title
(
	QString title
)
{
	buffer_.push_back( color_text( title, CUI_White ) ) ;
	buffer_.push_back( "" ) ;
}

QString
ConsoleUI::color_text
(
	QString text,
	CUI_TextColor color,
	bool add_hashtag
)
{
	QString hashtag = "" ;
	QString tmp = "\x1B[1m" ; // bold
	// QString tmp = "\x1B[4m" ; // underline

	switch( color )
	{
		case CUI_Red : tmp += "\x1B[31m" ; break ;
		case CUI_Green : tmp += "\x1B[32m" ; break ;
		case CUI_Yellow : tmp += "\x1B[33m" ; break ;
		case CUI_Blue : tmp += "\x1B[34m" ; break ;
		case CUI_Magenta : tmp += "\x1B[35m" ; break ;
		case CUI_Cyan : tmp += "\x1B[36m" ;	break ;
#ifdef Q_OS_UNIX
		case CUI_White : tmp += "\x1B[37m" ; break ;
#endif
#ifdef Q_OS_WIN
		case CUI_White : tmp += "\x0" ; break ;
#endif
	}

	if( add_hashtag == true )
	{
		hashtag = "#" ;
	}

	tmp += hashtag + text ;
	tmp += "\x1B[0m" ; // reset
	text = tmp + hashtag ;

	return text ;
}

void
ConsoleUI::console_size
()
{
#ifdef Q_OS_UNIX
	UNIX_console_size() ;
#endif

#ifdef Q_OS_WIN
	WIN_console_size() ;
#endif
}

QString
ConsoleUI::display_array
(
	const std::array< unsigned int, CC_Flag::CC_Flag_Size > & array,
	bool shortcut
)
{
	QString result = "" ;

	if( shortcut == false )
	{
		bufferize_text( color_text( "Total number of lines: ", CUI_White ) + QString::number( array[ 0 ] ) ) ;
		bufferize_text() ;

		bufferize_text( color_text( "Number of comment line: ", CUI_White ) + QString::number( array[ 1 ] ) ) ;
		bufferize_text( color_text( "Number of mixed line: ", CUI_White ) + QString::number( array[ 2 ]  ) ) ;
		bufferize_text( color_text( "Number of pure code line: ", CUI_White ) + QString::number( array[ 3 ]  ) ) ;
		bufferize_text() ;
	}
	else
	{
		result += color_text( "TOT: ", CUI_White ) + QString::number( array[ 0 ] ) + " " ;
		result += color_text( "COM: ", CUI_White ) + QString::number( array[ 1 ] ) + " " ;
		result += color_text( "MIX: ", CUI_White ) + QString::number( array[ 2 ] ) + " " ;
		result += color_text( "COD: ", CUI_White ) + QString::number( array[ 3 ] ) + " " ;
	}

	return result ;
}

void
ConsoleUI::display_buffer
()
{
	unsigned int count = 0 ;
	bool endByEnter = true ;

	while( ! buffer_.empty() )
	{
		std::cout << buffer_.first().toStdString() ;
		count++ ;

		if( count < _rows_ )
		{
			std::cout << std::endl ;
		}
		else
		{
			endByEnter = false ;
		}

		buffer_.erase( buffer_.begin() ) ;

		while( ( ! buffer_.empty() ) &&
			   ( count > _rows_ - 1 ) &&
			   ( getchar() != 10 ) ) ;
	}

	if( endByEnter == false )
	{
		std::cout << std::endl ;
		std::cout << std::endl ;
	}
}

void
ConsoleUI::display_name
(
	QString name,
	bool isFile,
	unsigned int level
)
{
	unsigned int pos = 0 ;

#ifdef Q_OS_UNIX
	pos = name.toStdString().find_last_of( '/' ) + 1 ;
#endif

#ifdef Q_OS_WIN
	pos = ;
#endif

	name = name.right( name.size() - pos ) ;

	if( !isFile )
	{
		name = color_text( name, CUI_Blue ) ;
	}

	QString prefix = " " ;
	if( level != 0 )
	{
		prefix += "|   " ;
	}

	for( unsigned int i = 1 ; i < level ; ++i )
	{
		prefix += "|   " ;
	}

	prefix += "|---" ;
	bufferize_text( prefix + name ) ;
}

void
ConsoleUI::display_report
(
	CC_Report * report
)
{
	double dbl = report->average ;
	bufferize_text( color_text( "Average: ", CUI_White ) + QString::number( dbl ) ) ;

	dbl = report->variance ;
	bufferize_text( color_text( "Variance: ", CUI_White ) + QString::number( dbl ) ) ;

	dbl = report->divergence ;
	bufferize_text( color_text( "Divergence: ", CUI_White ) + QString::number( dbl ) ) ;

	bufferize_text() ;
}

void
ConsoleUI::display_tree
(
	const CC_Folder * folder,
	unsigned int level
)
{
	QList< CC_File * > list_files = folder->list_files ;
	for( int i = 0 ; i < list_files.size() ; ++i )
	{
		display_name( list_files[ i ]->name, true, level ) ;
	}

	QList< CC_Folder * > list_folders = folder->list_folders ;
	for( int i = 0 ; i < list_folders.size() ; ++i )
	{
		display_name( list_folders[ i ]->name, false, level ) ;
		display_tree( list_folders[ i ], level + 1 ) ;
	}
}

QStringList
ConsoleUI::filter_command
(
	QString command
)
{
	// Split the whole line into words
	QStringList list = command.split( ' ' ) ;

	// to go throught the list and erase all empty element
	QStringList::iterator it = list.begin() ;

	while( it != list.end() )
	{
		// it was two spaces next to each other
		if( it->isEmpty() )
		{
			list.erase( it ) ;
		}
		else
		{
			++it ;
		}
	}

	return list ;
}

void
ConsoleUI::process
()
{
	bool close = false ;
	std::string tmp = "" ;
	QString command = "" ;
	QStringList param_list ;

	// display the welcome message
	if( welcomed_ == true )
	{
		welcome() ;
		display_buffer() ;
	}

	while( ! close )
	{
		// avoid the remain of parameters
		param_list.clear() ;

		// get the command line
		std::cout << color_text( "CC:> ", CUI_Green, false ).toStdString() ;

		std::getline( std::cin, tmp ) ;
		command = QString( tmp.c_str() ).simplified() ;

		tmp = "" ;

		// check the command
		if( !( command.isEmpty() ) )
		{
			// create a list of parameters
			param_list = filter_command( command ) ;
			command = "" ;

			// check which function need to be called
			if( param_list.front() == "commands" )
			{
				commands( param_list ) ;
			}
			// COMMANDS
			else if( param_list.front() == "analyze" )
			{
				analyze( param_list ) ;
			}
			else if( param_list.front() == "directory" )
			{
				directory( param_list ) ;
			}
			else if( param_list.front() == "help" )
			{
				help( param_list ) ;
			}
			else if( param_list.front() == "info" )
			{
				info( param_list ) ;
			}
			else if( param_list.front() == "language" )
			{
				language( param_list ) ;
			}
			else if( param_list.front() == "move" )
			{
				move( param_list ) ;
			}
			else if( param_list.front() == "preparation" )
			{
				preparation( param_list ) ;
			}
			else if( param_list.front() == "report" )
			{
				report( param_list ) ;
			}
			else if( param_list.front() == "tree" )
			{
				tree( param_list ) ;
			}
			// QUIT
			else if( param_list.front() == "quit" )
			{
				close = true ;
			}
			else
			{
				std::cout << "This command is not defined. Use 'commands' to list them." << std::endl ;
			}
		}
	}

	// display ending message
	std::cout << std::endl ;
	std::cout << "\t" << "Goodbye, see you later !" << std::endl ;
	std::cout << std::endl ;
}

void
ConsoleUI::welcome
()
{
	bufferize_text() ;
	buffer_.push_back( align_line( "/================\\", CUI_CENTER ) ) ;
	buffer_.push_back( align_line( "|    COMCHECK    |", CUI_CENTER ) ) ;
	buffer_.push_back( align_line( "\\================/", CUI_CENTER ) ) ;
	bufferize_text() ;

	bufferize_text( "ComCheck is a little tool used for analysing source code files and counting how many comments are written in these files, regardless of programming language." ) ;
	bufferize_text() ;

	bufferize_text( "If you don't know how to use it, maybe the command 'help' could do its job !" ) ;
	bufferize_text() ;

	display_buffer() ;
}

#ifdef Q_OS_UNIX
void
ConsoleUI::UNIX_console_size
(
	int signum
)
{
	Q_UNUSED( signum ) ;

	ioctl( STDOUT_FILENO, TIOCGWINSZ, &_w_ ) ;
	ConsoleUI::_cols_ = _w_.ws_col ;
	ConsoleUI::_rows_ = _w_.ws_row ;
}
#endif

#ifdef Q_OS_WIN
void
ConsoleUI::WIN_console_size
()
{
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &_csbi_ ) ;
	ConsoleUI::_cols_ = _csbi_.srWindow.Right - _csbi_.srWindow.Left ;
	ConsoleUI::_rows_ = _csbi_.srWindow.Bottom - _csbi_.srWindow.Top ;
}
#endif
