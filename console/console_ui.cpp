#include "console_ui.hpp"

ConsoleUI::ConsoleUI
(
	QObject * parent
)
: IUI( parent )
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

	/*
	std::cout << "List of all commands:" << std::endl ;
	std::cout << std::endl ;

	std::cout << "\t" << "To list every possibility you have: commands" << std::endl ;
	std::cout << "\t" << "To help you out: help" << std::endl ;
	std::cout << "\t" << "To close the execution: quit" << std::endl ;
	*/
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
	if( param_list.isEmpty() )
	{
		// display main help
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
	welcome() ;

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
	std::cout << "\t\t\t" << "/==========================================\\" << std::endl ;
	std::cout << "\t\t\t" << "|                 COMCHECK                 |" << std::endl ;
	std::cout << "\t\t\t" << "\\==========================================/" << std::endl ;
	std::cout << std::endl ;

	std::cout << "\t" << "" << std::endl ;

	std::cout << std::endl ;

	std::cout << "\t" << "If you don't know how it works, maybe the command 'help' could do its job." << std::endl ;
	std::cout << std::endl ;
}
