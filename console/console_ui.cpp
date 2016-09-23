#include "console_ui.hpp"

ConsoleUI::ConsoleUI
(
	QObject * parent,
	bool welcomed
)
: IUI( parent )
, welcomed_( welcomed )
, cols_( 0 )
, lines_( 0 )
, rows_( 0 )
{

	find_console() ;

	if( cols_ == 0 || rows_ == 0 )
	{
		std::cout << "Impossible to get console size." << std::endl ;
	}
}

ConsoleUI::~ConsoleUI
()
{

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
	std::string text
)
{

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
ConsoleUI::find_console
()
{
#ifdef Q_OS_UNIX

	struct winsize w ;
	ioctl( STDOUT_FILENO, TIOCGWINSZ, &w ) ;

	cols_ = w.ws_col ;
	rows_ = w.ws_row ;

#endif

#ifdef Q_OS_WIN

	CONSOLE_SCREEN_BUFFER_INFO csbi ;

	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi ) ;
	columns_ = csbi.srWindow.Right - csbi.srWindow.Left + 1 ;
	lines_ = csbi.dwCursorPosition.Y ;
	rows_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1 ;

#endif
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
