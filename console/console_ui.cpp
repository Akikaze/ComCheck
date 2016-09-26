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
	QObject * parent,
	bool welcomed
)
: IUI( parent )
, buffer_( QStringList() )
, welcomed_( welcomed )
{

#ifdef Q_OS_UNIX
	// clean the console
	system( "clear" ) ;

	// handle the signal of WINdow CHange
	signal( SIGWINCH, UNIX_console_size ) ;
#endif

#ifdef Q_OS_WIN
	// clean the console
	system( "cls" ) ;
#endif

	console_size() ;

	if( _cols_ == 0 || _rows_ == 0 )
	{
		std::cout << "Impossible to get console size." << std::endl ;
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
		while( ( unsigned int )( text.length() ) > _cols_ )
		{
			unsigned int pos = _cols_ - 1 ;

			while( text[ pos ] != ' ' )
			{
				pos-- ;
			}

			buffer_.push_back( text.left( pos ) ) ;
			text.remove( 0, pos + 1 ) ;
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
	CUI_TextColor color
)
{
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

	tmp += text ;
	tmp += "\x1B[0m" ; // reset
	text = tmp ;

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
		std::cout << color_text( "CC:> ", CUI_Green ).toStdString() ;
		std::getline( std::cin, tmp ) ;
		command = QString( tmp.c_str() ).simplified() ;

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
			else if( param_list.front() == "help" )
			{
				help( param_list ) ;
			}
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
