#include "console_ui.hpp"

unsigned int ConsoleUI::_cols_ = 0 ;
unsigned int ConsoleUI::_rows_ = 0 ;
QStringList ConsoleUI::_text_ = QStringList() ;

#ifdef Q_OS_UNIX
struct winsize ConsoleUI::_w_ = struct winsize ;
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
, welcomed_( welcomed )
{

#ifdef Q_OS_UNIX
	// clean the console
	system( "clear" ) ;

	// handle the signal of WINdow CHange
	signal( SIGWINCH, UNIX_console_size ) ;

	// get size of the console
	UNIX_console_size() ;
#endif

#ifdef Q_OS_WIN
	// clean the console
	system( "cls" ) ;

	// get size of the console
	WIN_console_size() ;
#endif

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
	if( alignement != CUI_LEFT )
	{
		QString blank = "" ;
		unsigned int spaces = _cols_ - line.length() ;

		if( alignment == CUI_CENTER )
		{
			spaces / 2 ;
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
ConsoleUI::display_paragraph
(
	std::string paragraph
)
{
	std::cout << paragraph << std::endl ;
}

void
ConsoleUI::display_text
(
	std::string text,
	CUI_TextAlignment alignment
)
{
	bool change = false ;
	QStringList division = QString( text.c_str() ).split( ' ' ) ;
	QString last = _text_.last() ;

	while( !change && !( division.empty() ) )
	{
		last += division.front() + ' ' ;
		division.erase( division.begin() ) ;

		if( !( division.empty() ) )
		{
			change = ( ( last.size() + ( division.front() ).size() ) + 1 > _cols_ ) ;
		}
	}



}

void
ConsoleUI::display_title
(
	std::string title
)
{
	std::cout << title << std::endl ;
	std::cout << std::endl ;
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
	}

	while( ! close )
	{
		// avoid the remain of parameters
		param_list.clear() ;

		// get the command line
		std::cout << "CC:> " ;
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
	std::cout << std::endl ;
	std::cout << "\t\t" << "/==========================================\\" << std::endl ;
	std::cout << "\t\t" << "|                 COMCHECK                 |" << std::endl ;
	std::cout << "\t\t" << "\\==========================================/" << std::endl ;
	std::cout << std::endl ;

	// default size of a terminal
	// -->                                                                               <--

	std::cout << "ComCheck is a little tool used for analyzing source code files and counting how" << std::endl ;
	std::cout << "many comments are written in these files, regardless of programming language." << std::endl ;
	std::cout << std::endl ;

	std::cout << "If you don't know how to use it, maybe the command 'help' could do its job." << std::endl ;
	std::cout << std::endl ;
}

#ifdef Q_OS_UNIX
void
ConsoleUI::UNIX_console_size
()
{
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
	ConsoleUI::_cols_ = _csbi_.srWindow.Right - _csbi_.srWindow.Left + 1 ;
	ConsoleUI::_rows_ = _csbi_.srWindow.Bottom - _csbi_.srWindow.Top + 1 ;
}
#endif
