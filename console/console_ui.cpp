#include "console_ui.hpp"

ConsoleUI::ConsoleUI
(
	QObject * parent,
	bool welcomed
)
: IUI( parent )
, welcomed_( welcomed )
{

}

ConsoleUI::~ConsoleUI
()
{

}

void
ConsoleUI::commands
(
	QStringList param_list
)
{
	Q_UNUSED( param_list ) ;

	// display like man
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
ConsoleUI::help
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( param_list.isEmpty() )
	{
		// default size of a terminal
		// -->                                                                               <--

		std::cout << std::endl ;
		std::cout << "\t" << TITLE( "What is ComCheck?" ) << std::endl ;
		std::cout << std::endl ;

		std::cout << "ComCheck is a little tool used for analyzing source code files and counting how" << std::endl ;
		std::cout << "many comments are written in these files, regardless of programming language." << std::endl ;
		std::cout << std::endl ;

		std::cout << "Obviously, C++ comments are not written like HTML comments which don't either" << std::endl ;
		std::cout << "look like PHP comments. But what is common between every langage is the fact" << std::endl ;
		std::cout << "that they require files which contain lines." << std::endl ;
		std::cout << "This is why ComCheck looks every folder of a project and lists every files" << std::endl ;
		std::cout << "before checking, for each file, the type of each line (code, comment or mix)." << std::endl ;
		std::cout << std::endl ;

		std::cout << "But, for determining if a line is a comment, ComCheck needs to know how are" << std::endl ;
		std::cout << "written comments in every langage. And this is really hard for a static system." << std::endl ;
		std::cout << "To be able to understand even future langage still not invented, ComCheck uses" << std::endl ;
		std::cout << "a system of modules that you can plug directly in ComCheck. By default, ComCheck" << std::endl ;
		std::cout << "brings a module for C++. But if you know how to use C++ and QtCreator, you can" << std::endl ;
		std::cout << "create your own module for a specific language." << std::endl ;
		std::cout << std::endl ;

		std::cout << "A module defines the file extension, to avoid looking for JAVA comments in Ruby" << std::endl ;
		std::cout << "files, and how to get the type of a specific line. Those modules are stored in" << std::endl ;
		std::cout << "dynamic librairies (.so or .a and .dl according to your operating system)." << std::endl ;
		std::cout << std::endl ;

		std::cout << "\t" << TITLE( "How to use ComCheck?" ) << std::endl ;
		std::cout << std::endl ;
	}
	else
	{
		// display help for a specific command
	}
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
